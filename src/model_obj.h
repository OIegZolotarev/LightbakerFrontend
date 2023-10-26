/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once
#include "file_system.h"
#include "gl_texture.h"
#include "scene_renderer.h"
#include "gl_backend.h"
#include "mtl_library.h"
#include "mod_obj_data.h"
#include "scene_entity.h"

class ModelOBJ: public SceneEntity
{
public:
	ModelOBJ(FileData* pFileData);
	~ModelOBJ();

	void DrawDebug();

	std::vector<lightDef_t>& ParsedLightDefs();

	std::string GetModelFileName();
	std::string GetModelTextureName();

	void ReloadTextures();
	void Export(const char* fileName);

	void ClearLightDefinitions();
	void AddLight(lightDefPtr_t& it);

	void SetLightmapDimensions(int w, int h);

	DrawMesh* GetDrawMesh();
	void BuildDrawMesh();

	float GetSceneScale();

	// Selectable object
	void OnHovered() override;
	void OnMouseMove(glm::vec2 delta) override;
	void OnSelect() override;
	void OnUnSelect() override;
	void OnUnhovered() override;
	void RenderForSelection(int objectId, class SceneRenderer*) override;
	
	// Scene entity
	void RenderBoundingBox() override;
	void RenderDebug() override;
	void RenderLightshaded() override;
	void RenderUnshaded() override;

private:
	size_t m_VertSize = 0;
	size_t m_UVSize = 0;

	unsigned int	m_LightsParsed = 0;
	mobjmaterial_t* mCurrentMaterial = nullptr;

	std::vector<float>	m_vecVertsData;
	std::vector<float>	m_vecUVData;
	std::vector<float>	m_vecNormalsData;

	std::vector<mobjface_t>		m_vecFaces;
	std::vector<mobjmesh_t>		m_vMeshes;
	std::vector<mobjegroup_t>	m_vGroups;
	std::vector<lightDef_t>		m_vParsedLightDefs;
	

	void ParseData(FileData* pFileData);
	void ParseCommand(std::string & buffer);
	
	void ParseNormal(std::string& buffer);
	void ParseUV(std::string& buffer);
	void ParseVertex(std::string& buffer);
	void ParseFace(std::string& buffer);
	void ParseGroup(std::string& buffer);

	void ParseLightDef(std::string& buffer);
	int m_LightmapDimensions[2];
	glm::vec3 m_EnvColor;

	void ExportLightDefs(FILE* fp);
	void ExportVerticles(FILE* fp);
	void ExportNormals(FILE* fp);
	void ExportUV(FILE* fp);
	void ExportFaces(FILE* fp);

	std::string m_strModelName;
	std::string m_strDiffuseName;
	
	
	DrawMesh mesh;

	float m_flSceneScale = 1.f;

	mobjgroup_s* CurrentGroup()
	{
		if (m_vGroups.size() == 0)
			return nullptr;

		return &m_vGroups[m_vGroups.size() - 1];
	}

	class MaterialTemplateLibrary* m_pMaterialLib;
	void AddMaterial(mobjmaterial_t* mCurrentMaterial);

	unsigned int mCurrentMeshId = 0;
};
