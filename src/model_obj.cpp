/*
	LightBaker3000 Frontend project,
	(c) 2022-2023 CrazyRussian
*/


#define _CRT_SECURE_NO_WARNINGS

#include "camera.h"
#include "model_obj.h"
#include "application.h"
#include "common_resources.h"
#include <intsafe.h>
#include "gl_backend.h"
#include <algorithm>
#include "text_utils.h"
#include "mod_obj_asynch_loader.h"
#include "mod_obj_asynch_exporter.h"

#define WHITE_PNG "res/textures/white.png"
#define DUMMY_PNG "res/textures/dummy.png"

ModelOBJ::ModelOBJ(const char* fileName)
{

	ModObjAsynchLoader* loaderTask = new ModObjAsynchLoader(this, fileName);
	LoaderThread::Instance()->ScheduleTask(loaderTask);
	//BuildDrawMesh();

	m_strModelName = fileName;

}

ModelOBJ::~ModelOBJ()
{
	for (auto& it : m_ModelData.meshes)
	{
		FreeGLTexture(it.diffuse_texture);

		for(int i = 0 ; i < MAX_LIGHT_STYLES; i++)
			FreeGLTexture(it.lightmap_texture[i]);
	}

	FreeVector(m_ModelData.faces);
	FreeVector(m_ModelData.normals);
	FreeVector(m_ModelData.uvs);
	FreeVector(m_ModelData.verts);
	FreeVector(m_ModelData.lightDefs);
	FreeVector(m_ModelData.groups);

}

void ModelOBJ::DrawDebug()
{

#if 1
	mesh.Bind();

	glEnable(GL_TEXTURE_2D);

	for (auto& it : m_ModelData.meshes)
	{
		if (it.diffuse_texture)
			glBindTexture(GL_TEXTURE_2D, it.diffuse_texture->gl_texnum);
		else
			glBindTexture(GL_TEXTURE_2D, 0);

		mesh.Draw(it.first_face, it.num_faces);
	}

	mesh.Unbind();
#else
 	if (m_ModelData.faces.size() == 0)
 		return;
 
 	if (m_ModelData.verts.size() == 0)
 		return;
 			
 	size_t currentMaterial = m_ModelData.faces[0].material_id;
 	mobjmaterial_t* pMaterial = &m_vMaterials[currentMaterial];
 
 	bool bNoTextures = false;
 
 	if (pMaterial->lightmap_texture[0])
 	{
 		glBindTexture(GL_TEXTURE_2D, pMaterial->lightmap_texture[0]->gl_texnum);
 		bNoTextures = false;
 	}
 	else
 		bNoTextures = true;
 
 	if (bNoTextures)
 		glDisable(GL_TEXTURE_2D);
 
 	glColor3f(1, 1, 1);
 
 	glBegin(GL_TRIANGLES);
 	
 	for (auto& face : m_ModelData.faces)
 	{
 		if (face.material_id != currentMaterial)
 		{
 			currentMaterial = face.material_id;
 			mobjmaterial_t* pMaterial = &m_vMaterials[currentMaterial];
 
 			if (pMaterial->lightmap_texture[0])
 			{
 				glBindTexture(GL_TEXTURE_2D, pMaterial->lightmap_texture[0]->gl_texnum);
 				bNoTextures = false;
 			}
 			else
 			{
 				glDisable(GL_TEXTURE_2D);
 			}
 		}
 		
 		glTexCoord2fv(&m_ModelData.uvs[(face.uv- 1) * m_ModelData.uvSize]);
 
 		if (bNoTextures)
 		{
 			float* norm = &m_ModelData.normals[(face.norm - 1) * 3];
 			glColor3f(
 				(norm[0] + 1) / 2,
 				(norm[1] + 1) / 2,
 				(norm[2] + 1) / 2);
 		}
 		glVertex3fv(&m_ModelData.verts[(face.vert - 1) * m_ModelData.vertSize]);
 	}
 
 	glEnd();
 
 
 	glEnable(GL_TEXTURE_2D);
#endif
	
}

void ModelOBJ::ClearLightDefinitions()
{
	m_ModelData.lightDefs.clear();
}

void ModelOBJ::AddLight(lightDefPtr_t& it)
{
	m_ModelData.lightDefs.push_back(*it);
}

void ModelOBJ::SetLightmapDimensions(int w, int h)
{
	m_ModelData.lightmapDimensions[0] = w;
	m_ModelData.lightmapDimensions[1] = h;
}

void ModelOBJ::BuildDrawMesh()
{
	if (m_ModelData.faces.size() == 0)
		return;

	if (m_ModelData.verts.size() == 0)
		return;

	// Sort faces by mesh_id

	std::qsort(m_ModelData.faces.data(), m_ModelData.faces.size() / 3, sizeof(mobjface_t) * 3, [](const void* pa, const void* pb) -> int
		{
			const mobjface_t* faceA = static_cast<const mobjface_t*>(pa);
			const mobjface_t* faceB = static_cast<const mobjface_t*>(pb);

			if (faceA->materialId == faceB->materialId)
				return 0;

			else if (faceA->materialId > faceB->materialId)
				return 1;

			else if (faceA->materialId < faceB->materialId)
				return -1;

			return 0;
		});

	bool bNoTextures = false;

	size_t currentMaterial = 0;
	mobjmesh_t* pMaterial = nullptr;

	currentMaterial = m_ModelData.faces[0].materialId;
	pMaterial = &m_ModelData.meshes[currentMaterial];

	
	if (pMaterial->lightmap_texture[0])			
		bNoTextures = false;	
	else
		bNoTextures = true;

	
	pMaterial->first_face = mesh.CurrentElement();

	mesh.Begin(GL_TRIANGLES);
	mesh.Color4f(1, 1, 1, 1);

	for (auto& face : m_ModelData.faces)
	{
		if (face.materialId != currentMaterial)
		{
			pMaterial->num_faces = mesh.CurrentElement() - pMaterial->first_face;

			//Con_Printf("%d : First face: %d, num faces: %d\n", currentMaterial, pMaterial->first_face, pMaterial->num_faces);

			currentMaterial = face.materialId;
			pMaterial = &m_ModelData.meshes[currentMaterial];

			pMaterial->first_face = mesh.CurrentElement();

			//assert(pMaterial->material);

			if (pMaterial->lightmap_texture[0])
				bNoTextures = false;
			else
				bNoTextures = true;
		}

		if (m_ModelData.uvs.size() > 0 && face.uv > 0)
		{
			mesh.TexCoord2fv(&m_ModelData.uvs[(face.uv - 1) * m_ModelData.uvSize]);
		}
		else
		{
			float nullUv[2] = { 0,0 };
			mesh.TexCoord2fv(nullUv);
		}

		if (m_ModelData.normals.size() > 0 && face.norm > 0)
		{
			float* norm = &m_ModelData.normals[(face.norm - 1) * 3];
			mesh.Normal3fv(norm);
		}
		else
		{
			float nullNorm[3] = { 0,0,0 };
			mesh.Normal3fv(nullNorm);
		}

			float * p = &m_ModelData.verts[(face.vert - 1) * m_ModelData.vertSize];
			//Con_Printf("%f %f %f\n", p[0], p[1], p[2]);

			mesh.Vertex3fv(&m_ModelData.verts[(face.vert - 1) * m_ModelData.vertSize]);
		
	}

	pMaterial->num_faces = mesh.CurrentElement() - pMaterial->first_face;

	mesh.End();

	FlagDataLoaded();
}

float ModelOBJ::GetSceneScale()
{
	return m_ModelData.sceneScale;
}

DrawMesh* ModelOBJ::GetDrawMesh()
{
	return &mesh;
}

void ModelOBJ::OnHovered()
{
	
}

void ModelOBJ::OnMouseMove(glm::vec2 delta)
{
	
}

void ModelOBJ::OnSelect()
{
	//Con_Printf("ModelOBJ::OnSelect()\n");
}

void ModelOBJ::OnUnSelect()
{
	
}

void ModelOBJ::OnUnhovered()
{
	
}

void ModelOBJ::RenderForSelection(int objectId, SceneRenderer* pRenderer)
{
	auto sceneReneder = Application::Instance()->GetMainWindow()->GetSceneRenderer();
	auto shader = GLBackend::Instance()->GeometrySelectionShader();
	
	auto identity = glm::mat4(1);

	shader->Bind();

	shader->SetTransform(identity);
	shader->SetScale(sceneReneder->GetSceneScale());
	shader->SetDefaultCamera();
	shader->SetObjectId(objectId);

	mesh.Bind();
	mesh.Draw(0, m_ModelData.faces.size());	
	
	mesh.Unbind();
	shader->Unbind();
}

void ModelOBJ::RenderBoundingBox()
{
	throw std::logic_error("The method or operation is not implemented.");
}

void ModelOBJ::RenderDebug()
{
	DrawDebug();
}

void ModelOBJ::RenderLightshaded()
{
	auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();

	mesh.Bind();

	auto shader = GLBackend::Instance()->LightMappedSceneShader();
	shader->Bind();
	shader->SetScale(sceneRenderer->GetSceneScale());
	shader->SetDefaultCamera();

	glEnable(GL_TEXTURE_2D);

	//for (int i = 5 ;  i < m_ModelData.meshes.size() - 1; i++)
	for (auto& it : m_ModelData.meshes)
	{
		//auto it = m_ModelData.meshes[i];

		if (it.num_faces == 0)
		{
			continue;
		}

		glActiveTexture(GL_TEXTURE0);
		{
			if (it.material)
			{
				if (it.material->map_Kd)
					glBindTexture(GL_TEXTURE_2D, it.material->map_Kd->gl_texnum);
				else
					glBindTexture(GL_TEXTURE_2D, 0);
			}
			else
			{
				if (it.diffuse_texture)
					glBindTexture(GL_TEXTURE_2D, it.diffuse_texture->gl_texnum);
				else
					glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		glActiveTexture(GL_TEXTURE1);
		{
			if (it.lightmap_texture[0])
				glBindTexture(GL_TEXTURE_2D, it.lightmap_texture[0]->gl_texnum);
			else
				glBindTexture(GL_TEXTURE_2D, 0);
		}

		mesh.Draw(it.first_face, it.num_faces);

		//break;
	}

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);

	shader->Unbind();
	mesh.Unbind();
}

void ModelOBJ::RenderUnshaded()
{
	//throw std::logic_error("The method or operation is not implemented.");
}

mobjdata_t* ModelOBJ::GetModelData() 
{
	return &m_ModelData;
}

void ModelOBJ::AddLightsIntoScene()
{
	auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();
	

	for (auto def : m_ModelData.lightDefs)
	{
		sceneRenderer->AddLightToSceneWithSerialNumber(def, def.serial_number);
	}
}

void ModelOBJ::ReloadTextures()
{
	for (auto& it : m_ModelData.meshes)
	{
		if (it.diffuse_texture)
			GLReloadTexture(it.diffuse_texture);
		else
		{
			it.diffuse_texture = LoadGLTexture(it.diffuse_texture_path.c_str());
		
			if (it.diffuse_texture->width == -1)
				it.diffuse_texture = LoadGLTexture(DUMMY_PNG);
				
			
		}

		for (int i = 0; i < MAX_LIGHT_STYLES; i++)
		{
			if (it.lightmap_texture[i])
				GLReloadTexture(it.lightmap_texture[i]);
			else
			{
				it.lightmap_texture[i] = LoadGLTexture(it.lightmap_texture_path[i].c_str());

// 				if (it.lightmap_texture[i]->width == -1)
// 					it.lightmap_texture[i] = LoadGLTexture(WHITE_PNG);
			}
		}
			
	}
}

void ModelOBJ::Export(const char* fileName)
{
	ModObjAsynchExporter* exporter = new ModObjAsynchExporter(this, fileName);
	exporter->ExportSynch();
	delete exporter;

// 	return;
// 
// 	FILE* fp = nullptr;
// 	fp = fopen(fileName, "wt");
// 
// 	if (!fp)
// 		Application::EPICFAIL("Error while opening file %s for exporting!", fileName);
// 
// 	ExportLightDefs(fp);
// 	ExportVerticles(fp);
// 	
// 	if (m_ModelData.uvs.size() > 0)  ExportNormals(fp);
// 	if (m_ModelData.uvs.size() > 0) ExportUV(fp);
// 
// 	ExportFaces(fp);
// 
// 	fclose(fp);
}

std::string ModelOBJ::GetModelFileName()
{
	return m_strModelName;
}

std::string ModelOBJ::GetModelTextureName()
{
	return m_strDiffuseName;
}


