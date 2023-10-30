/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#pragma once

#include "common.h"
#include "lb3k_wrapper.h"
#include "serial_counter.h"
#include "scene_entity.h"
#include "model_obj.h"

enum class LevelFormat
{
	Unknown = 0,
	WavefrontOBJ,
	BSP
};

class Scene
{
	lightBakerSettings_s m_lightBakerParams;
	SerialCounter<unsigned int> m_ObjectsCounter;

	LevelFormat DetermineLevelFormatFromFileName(std::string levelName);

	LevelFormat m_LevelFormat = LevelFormat::Unknown;

	// Scene Entity 0 - level model
	std::list<SceneEntityPtr> m_SceneEntities;	
	
	
	//std::list<lightDefWPtr_t> m_SceneLightDefs;
	//std::shared_ptr<ModelOBJ>  m_pSceneModel;
		
	float m_flSceneScale = 1.0f;

	CEditHistory* m_pEditHistory;
	SceneEntityWeakPtr m_pCurrentSelection;

public:
	Scene(const char* levelName, int loadFlags);
	~Scene();

	
	void DoDeleteSelection();	
	void AddNewLight(glm::vec3 pos, LightTypes type);

	std::list<SceneEntityPtr>& GetSceneObjects();
	std::list<SceneEntityPtr>& GetLightDefs();

	CEditHistory* GetEditHistory() const;
	
	void HintSelected(SceneEntityWeakPtr weakRef);
	SceneEntityWeakPtr GetSelection();

	void RenderObjectsFor3DSelection();
	void RenderLightShaded();
	
	bool IsModelLoaded();

	std::string GetModelFileName();
	std::string GetModelTextureName();

	// Entity manipulation
	void AddEntityWithSerialNumber(SceneEntityPtr it, size_t sn);
	SceneEntityWeakPtr GetEntityBySerialNumber(size_t serialNumber);
	void DeleteEntityWithSerialNumber(size_t serialNumber);
	void DeleteEntity(SceneEntityWeakPtr l);

	// Scene scaling
	float GetSceneScale();
	void SetScale(float f);
	void RescaleLightPositions(float m_flScaleOriginal, float m_flScale);

	SceneEntityWeakPtr GetEntityWeakRef(SceneEntity* pEntity);
	void RenderUnshaded();
	void Reload(int loadFlags);
private:
	void ClearEntities();
	void LoadLevel(const char* levelName);
public:
	std::string ExportForCompiling(const char* newPath);
};