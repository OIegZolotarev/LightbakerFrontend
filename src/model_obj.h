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
	ModelOBJ(const char* fileName);
	~ModelOBJ();

	void DrawDebug();

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

	mobjdata_t* GetModelData();
	mobjdata_t* GetLMData();

	void AddLightsIntoScene();
private:
	mobjdata_t m_ModelData;
	mobjdata_t m_LightmapModelData;

	std::string m_strModelName;
	std::string m_strDiffuseName;
		
	DrawMesh mesh;
	bool m_hasLMMesh = false;
};
