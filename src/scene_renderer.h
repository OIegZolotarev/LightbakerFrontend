/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

#include "common.h"
#include "main_window.h"
#include "lights.h"
#include "model_obj.h"
#include "edit_history.h"

class SceneRenderer: public IEventHandler
{	
public:
	SceneRenderer(class MainWindow* pTargetWindow);
	~SceneRenderer();

	class Camera* GetCamera();

	void	RenderScene();
	int		HandleEvent(bool bWasHandled, SDL_Event& e) override;
	float	FrameDelta();
	void LoadModel(const char* dropped_filedir, bool keepLight = false);

	void AddLightToSceneWithSerialNumber(lightDef_t& it, int sn);

	bool	IsModelLoaded();
	
	std::string GetModelFileName();
	std::string GetModelTextureName();

	void ReloadModel();

	void DrawBillboard(glm::vec3 pos, glm::vec2 size, gltexture_t* texture, glm::vec3 tint);
	void DrawBillboardSelection(glm::vec3 pos, glm::vec2 size, gltexture_t* texture, int index);

	lightDefWPtr_t GetLightWeakRef(lightDef_s* param1);
	
	void ExportModelForCompiling(const char* path);
	void AddNewLight(LightTypes type);

	std::vector<lightDefPtr_t>& GetSceneObjects();

	void HintSelected(lightDefWPtr_t weakRef);
	lightDefWPtr_t GetSelection();


	void DeleteLight(lightDefWPtr_t l);


	void DoDeleteSelection();
	CEditHistory* GetEditHistory() const;
private:
	void Debug_DrawGround();

	class Camera*		m_pCamera;
	class MainWindow*	m_pTargetWindow;

	CEditHistory* m_pEditHistory;

	ModelOBJ* m_pSceneModel = nullptr;

	std::vector<lightDefPtr_t> m_vecSceneLightDefs;

	DrawMesh*	m_pUnitBoundingBox;
	DrawMesh*	m_pIntensitySphere;
	
	ModelOBJ*	m_pDirectionModel;
	DrawMesh*	m_pDirectionArrow;


	void DrawBoundingBoxAroundLight(lightDefWPtr_t pObject);
	lightDefWPtr_t m_pCurrentSelection;

	int m_serialNubmerCounter = 1;

	int AllocSerialNumber();
public:
	lightDefWPtr_t GetLightBySerialNumber(int serialNumber);
	void DeleteLightWithSerialNumber(int serialNumber);
};

