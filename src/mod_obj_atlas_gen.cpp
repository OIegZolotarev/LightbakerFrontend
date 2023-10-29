/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#include "application.h"
#include "mod_obj_atlas_gen.h"
#include "../include/stb/stb_image_write.h"

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

MeshDeclWrapper::MeshDeclWrapper(mobjdata_t* modelData, size_t groupId)
{
	mobjegroup_t* group = &modelData->groups[groupId];

	m_GroupId = groupId;
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

		size_t index = face->uv - 1;

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


xatlas::UvMeshDecl& MeshDeclWrapper::GetMeshDecl()
{
	return m_MeshDecl;
}

size_t MeshDeclWrapper::GetGroupId()
{
	return m_GroupId;
}

void MeshDeclWrapper::SetXAtlasGroupId(size_t meshId)
{
	m_XAtlasMeshId = meshId;
}

size_t MeshDeclWrapper::GetXAtlasGroupId()
{
	return m_XAtlasMeshId;
}

void GenerateAtlasTask::TransferNewUV(xatlas::Atlas* atlas)
{
	mobjdata_t* lmData = m_pModel->GetLMData();
	mobjdata_t* data = m_pModel->GetModelData();

	auto v = lmData->meshes;

	*lmData = *data;
	lmData->meshes = v;

	lmData->uvSize = 2;
	lmData->uvs.clear();
	lmData->uvs.resize(data->uvs.size() / data->uvSize * 2);

	auto wrapperIt = m_lstWrappers.begin();

	for (size_t i = 0; i < atlas->meshCount; i++, wrapperIt++)
	{
		auto mesh = &atlas->meshes[i];
		MeshDeclWrapper* wrapper = *wrapperIt;

		assert(wrapper->GetXAtlasGroupId() == i);

		mobjegroup_t* grp = &data->groups[wrapper->GetGroupId()];
		assert(grp->num_vertices == mesh->vertexCount);

		size_t j = 0;

		for (size_t idx = grp->first_vertex; idx < (grp->first_vertex + grp->num_vertices); idx++, j++)
		{
			auto & face = data->faces[idx];
			auto& lmFace = lmData->faces[idx];
			size_t uvOffset = (face.uv - 1) * lmData->uvSize;
			
			lmData->uvs[uvOffset] = mesh->vertexArray[j].uv[0] / atlas->width;
			lmData->uvs[uvOffset + 1] = mesh->vertexArray[j].uv[1] / atlas->height;

			lmFace.materialId = 0;
		}

		//mesh->vertexArray
	}

}

GenerateAtlasTask::GenerateAtlasTask(ModelOBJ* pModel)
{
	xatlas::Atlas* atlas = xatlas::Create();
	//xatlas::SetProgressCallback(atlas, ProgressCallback, &stopwatch);
	xatlas::SetPrint(Con_Printf, true);

	m_pModel = pModel;
	mobjdata_t* data = pModel->GetModelData();

	size_t meshCount = 0;

	for (size_t i = 0; i < data->groups.size(); i++)
	{
		if (!data->groups[i].num_vertices)
			continue;

		auto wrapper = new MeshDeclWrapper(data, i);

		xatlas::AddMeshError error = xatlas::AddUvMesh(atlas, wrapper->GetMeshDecl());
		if (error != xatlas::AddMeshError::Success) 
		{
			//xatlas::Destroy(atlas);
			// Do something
			delete wrapper;
			continue;
		}

		meshCount++;

		wrapper->SetXAtlasGroupId(meshCount - 1);
		m_lstWrappers.push_back(wrapper);
	}

	//atlas->width = 4096;
	//atlas->height = 4096;


	atlas->texelsPerUnit = 16;

	xatlas::SetProgressCallback(atlas, ProgressCallback, nullptr);
	xatlas::ComputeCharts(atlas);
	xatlas::PackCharts(atlas);


	TransferNewUV(atlas);

	// Cleanup.

	xatlas::Destroy(atlas);
}

GenerateAtlasTask::~GenerateAtlasTask()
{
	m_lstWrappers.clear();
}

ITaskStepResult* GenerateAtlasTask::ExecuteStep(LoaderThread* loaderThread)
{
	return nullptr;
}

void GenerateAtlasTask::OnCompletion()
{
	
}
