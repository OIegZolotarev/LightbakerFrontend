/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#include "application.h"
#include "mod_obj_atlas_gen.h"
#include "..\include\ImGuiFileDialog\stb\stb_image.h"

static void RandomColor(uint8_t* color)
{
	for (int i = 0; i < 3; i++)
		color[i] = uint8_t((rand() % 255 + 192) * 0.5f);
}

static void SetPixel(uint8_t* dest, int destWidth, int x, int y, const uint8_t* color)
{
	uint8_t* pixel = &dest[x * 3 + y * (destWidth * 3)];
	pixel[0] = color[0];
	pixel[1] = color[1];
	pixel[2] = color[2];
}

// https://github.com/miloyip/line/blob/master/line_bresenham.c
// License: public domain.
static void RasterizeLine(uint8_t* dest, int destWidth, const int* p1, const int* p2, const uint8_t* color)
{
	const int dx = abs(p2[0] - p1[0]), sx = p1[0] < p2[0] ? 1 : -1;
	const int dy = abs(p2[1] - p1[1]), sy = p1[1] < p2[1] ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2;
	int current[2];
	current[0] = p1[0];
	current[1] = p1[1];
	while (SetPixel(dest, destWidth, current[0], current[1], color), current[0] != p2[0] || current[1] != p2[1])
	{
		const int e2 = err;
		if (e2 > -dx) { err -= dy; current[0] += sx; }
		if (e2 < dy) { err += dx; current[1] += sy; }
	}
}

/*
https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling
Copyright Dmitry V. Sokolov

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/
static void RasterizeTriangle(uint8_t* dest, int destWidth, const int* t0, const int* t1, const int* t2, const uint8_t* color)
{
	if (t0[1] > t1[1]) std::swap(t0, t1);
	if (t0[1] > t2[1]) std::swap(t0, t2);
	if (t1[1] > t2[1]) std::swap(t1, t2);
	int total_height = t2[1] - t0[1];
	for (int i = 0; i < total_height; i++) {
		bool second_half = i > t1[1] - t0[1] || t1[1] == t0[1];
		int segment_height = second_half ? t2[1] - t1[1] : t1[1] - t0[1];
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? t1[1] - t0[1] : 0)) / segment_height;
		int A[2], B[2];
		for (int j = 0; j < 2; j++) {
			A[j] = int(t0[j] + (t2[j] - t0[j]) * alpha);
			B[j] = int(second_half ? t1[j] + (t2[j] - t1[j]) * beta : t0[j] + (t1[j] - t0[j]) * beta);
		}
		if (A[0] > B[0]) std::swap(A, B);
		for (int j = A[0]; j <= B[0]; j++)
			SetPixel(dest, destWidth, j, t0[1] + i, color);
	}
}

// May be called from any thread.
static bool ProgressCallback(xatlas::ProgressCategory category, int progress, void* userData)
{

	static std::mutex progressMutex;
	std::unique_lock<std::mutex> lock(progressMutex);
	
	Con_Printf("\r   %s [", xatlas::StringForEnum(category));
	for (int i = 0; i < 10; i++)
		Con_Printf(progress / ((i + 1) * 10) ? "*" : " ");
	Con_Printf("] %d%%", progress);
	fflush(stdout);
	
	return true;
}

void TestAtlasGeneration(ModelOBJ* pModel)
{
	xatlas::Atlas* atlas = xatlas::Create();
	// xatlas::SetProgressCallback(atlas, ProgressCallback, &stopwatch);

	mobjdata_t* data = pModel->GetModelData();
	xatlas::UvMeshDecl meshDecl;

	uint32_t* indicies = new uint32_t[data->faces.size()];

	for (size_t i = 0; i < data->faces.size(); i++)
		indicies[i] = data->faces[i].vert;


	meshDecl.vertexCount = data->uvs.size() / data->uvSize;
	meshDecl.vertexUvData = data->uvs.data();
	meshDecl.vertexStride = data->uvSize * sizeof(float);

	meshDecl.indexData = data->faces.data();
	meshDecl.indexFormat = xatlas::IndexFormat::UInt32;
	meshDecl.indexCount = data->faces.size();

	xatlas::AddMeshError error = xatlas::AddUvMesh(atlas, meshDecl);
	if (error != xatlas::AddMeshError::Success) {
		xatlas::Destroy(atlas);
		// return ERROR
	}

	//atlas->width = 4096;
	//atlas->height = 4096;


	xatlas::SetProgressCallback(atlas, ProgressCallback, nullptr);

	xatlas::ComputeCharts(atlas);
	xatlas::PackCharts(atlas);

	delete[] indicies;
	xatlas::Destroy(atlas);
}

MeshDeclWrapper::MeshDeclWrapper(mobjdata_t* modelData, size_t groupId)
{
	mobjegroup_t* group = &modelData->groups[groupId];

	m_nFacesCount = group->num_vertices / 3;
	
	m_nUVDataCount = group->num_vertices;
	m_pUVData = new glm::vec2[m_nUVDataCount];
	m_pFaceMaterialData = new uint32_t[m_nFacesCount];


	for (size_t i = 0; i < m_nUVDataCount; i++)
	{
		auto face = &modelData->faces[group->first_vertex + i];

		if (i % 3 == 0)
		{
			m_pFaceMaterialData[i / 3] = face->materialId;
		}

		size_t index = face->uv;

		float* ptr = &modelData->uvs[index * modelData->uvSize];
		
		m_pUVData[i].x = ptr[0];
		m_pUVData[i].y = ptr[1];
	}

	m_MeshDecl.vertexCount = m_nUVDataCount;
	m_MeshDecl.vertexUvData = m_pUVData;
	m_MeshDecl.vertexStride = sizeof(glm::vec2);

	m_MeshDecl.faceMaterialData = m_pFaceMaterialData;
}

MeshDeclWrapper::~MeshDeclWrapper()
{
	delete m_pUVData;
	delete m_pFaceMaterialData;
}


GenerateAtlasTask::GenerateAtlasTask(ModelOBJ* pModel)
{
	xatlas::Atlas* atlas = xatlas::Create();
	//xatlas::SetProgressCallback(atlas, ProgressCallback, &stopwatch);
	xatlas::SetPrint(Con_Printf, true);

	mobjdata_t* data = pModel->GetModelData();

	m_pWrappers = new MeshDeclWrapper* [data->groups.size()];

	for (size_t i = 1; i < data->groups.size(); i++)
	{
		m_pWrappers[i] = new MeshDeclWrapper(data, i);

		xatlas::AddMeshError error = xatlas::AddUvMesh(atlas, m_pWrappers[i]->GetMeshDecl());
		if (error != xatlas::AddMeshError::Success) {
			xatlas::Destroy(atlas);
		}
	}

	//atlas->width = 4096;
	//atlas->height = 4096;


	xatlas::SetProgressCallback(atlas, ProgressCallback, nullptr);
	xatlas::ComputeCharts(atlas);
	xatlas::PackCharts(atlas);


	RasterizeAtlas(atlas);

	// Cleanup.

	xatlas::Destroy(atlas);
}

void GenerateAtlasTask::RasterizeAtlas(xatlas::Atlas* pAtlas)
{
	if (atlas->width > 0 && atlas->height > 0) {
		Con_Printf("Rasterizing result...\n");
		// Dump images.
		std::vector<uint8_t> outputTrisImage, outputChartsImage;
		const uint32_t imageDataSize = atlas->width * atlas->height * 3;
		outputTrisImage.resize(atlas->atlasCount * imageDataSize);
		outputChartsImage.resize(atlas->atlasCount * imageDataSize);
		for (uint32_t i = 0; i < atlas->meshCount; i++) {
			const xatlas::Mesh& mesh = atlas->meshes[i];
			// Rasterize mesh triangles.
			const uint8_t white[] = { 255, 255, 255 };
			for (uint32_t j = 0; j < mesh.indexCount; j += 3) {
				int32_t atlasIndex = -1;
				bool skip = false;
				int verts[3][2];
				for (int k = 0; k < 3; k++) {
					const xatlas::Vertex& v = mesh.vertexArray[mesh.indexArray[j + k]];
					if (v.atlasIndex == -1) {
						skip = true;
						break;
					}
					atlasIndex = v.atlasIndex;
					verts[k][0] = int(v.uv[0]);
					verts[k][1] = int(v.uv[1]);
				}
				if (skip)
					continue; // Skip triangles that weren't atlased.
				uint8_t color[3];
				RandomColor(color);
				uint8_t* imageData = &outputTrisImage[atlasIndex * imageDataSize];
				RasterizeTriangle(imageData, atlas->width, verts[0], verts[1], verts[2], color);
				RasterizeLine(imageData, atlas->width, verts[0], verts[1], white);
				RasterizeLine(imageData, atlas->width, verts[1], verts[2], white);
				RasterizeLine(imageData, atlas->width, verts[2], verts[0], white);
			}
			// Rasterize mesh charts.
			for (uint32_t j = 0; j < mesh.chartCount; j++) {
				const xatlas::Chart* chart = &mesh.chartArray[j];
				uint8_t color[3];
				RandomColor(color);
				for (uint32_t k = 0; k < chart->faceCount; k++) {
					int verts[3][2];
					for (int l = 0; l < 3; l++) {
						const xatlas::Vertex& v = mesh.vertexArray[mesh.indexArray[chart->faceArray[k] * 3 + l]];
						verts[l][0] = int(v.uv[0]);
						verts[l][1] = int(v.uv[1]);
					}
					uint8_t* imageData = &outputChartsImage[chart->atlasIndex * imageDataSize];
					RasterizeTriangle(imageData, atlas->width, verts[0], verts[1], verts[2], color);
					RasterizeLine(imageData, atlas->width, verts[0], verts[1], white);
					RasterizeLine(imageData, atlas->width, verts[1], verts[2], white);
					RasterizeLine(imageData, atlas->width, verts[2], verts[0], white);
				}
			}
		}
		for (uint32_t i = 0; i < atlas->atlasCount; i++) {
			char filename[256];
			snprintf(filename, sizeof(filename), "example_uvmesh_tris%02u.tga", i);
			printf("Writing '%s'...\n", filename);
			stbi_write_tga(filename, atlas->width, atlas->height, 3, &outputTrisImage[i * imageDataSize]);
			snprintf(filename, sizeof(filename), "example_uvmesh_charts%02u.tga", i);
			printf("Writing '%s'...\n", filename);
			stbi_write_tga(filename, atlas->width, atlas->height, 3, &outputChartsImage[i * imageDataSize]);
		}
	}
}

GenerateAtlasTask::~GenerateAtlasTask()
{
	delete[] m_pWrappers;
}

ITaskStepResult* GenerateAtlasTask::ExecuteStep(LoaderThread* loaderThread)
{
	return nullptr;
}

void GenerateAtlasTask::OnCompletion()
{
	
}
