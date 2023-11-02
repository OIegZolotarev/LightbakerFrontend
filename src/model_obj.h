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
#include "lb3k_wrapper.h"

class ModelOBJ: public SceneEntity
{
public:
	ModelOBJ(const char* fileName);

	void LoadLMMesh();

	~ModelOBJ();

	void DrawDebug();

	std::string GetModelFileName();
	std::string GetModelTextureName();

	void ReloadTextures();

	void ReloadLightmapTextures();

	// Возвращает путь к файлу ЛМ-модели
	std::string & Export(const char* fileName, lightBakerSettings_t * lb3kOptions);

	void UpdateLMMesh();

	void ClearLightDefinitions();
	void AddLight(lightDefPtr_t& it);

	void SetLightmapDimensions(int w, int h);

	DrawMesh* GetDrawMesh();
	void BuildDrawMesh();

	void ValidateLightmap();

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

	void CommonDrawGeometryWithShader(ISceneShader* shader);

	void RenderUnshaded() override;
	void RenderGroupShaded() override;
	
	const char* Description() override;

	mobjdata_t* GetModelData();
	mobjdata_t* GetLMData();

	void AddLightsIntoScene();
	void FlagHasLMMesh();

	

private:
	mobjdata_t m_ModelData;
	mobjdata_t m_LightmapModelData;

	std::string m_strModelName;
	std::string m_strDiffuseName;
		
	DrawMesh mesh;
	bool m_hasLMMesh = false;
	std::string m_strLMModelPath;
	void PrepareLights();
};
