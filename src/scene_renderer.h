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
	void	LoadModel(const char* dropped_filedir, bool keepLight = false);

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

	lightDefWPtr_t GetLightBySerialNumber(int serialNumber);
	void DeleteLightWithSerialNumber(int serialNumber);

	float	GetSceneScale();	
	void	SetSceneScale(float f);

	void RescaleLightPositions(float m_flScaleOriginal, float m_flScale);

	void FocusCameraOnObject(lightDefPtr_t& it);
private:
	void Debug_DrawGround();

	class Camera*		m_pCamera;
	class MainWindow*	m_pTargetWindow;

	

	// Move to document class? --->
	std::shared_ptr<ModelOBJ>  m_pSceneModel = nullptr;
	std::vector<lightDefPtr_t> m_vecSceneLightDefs;

	int m_serialNubmerCounter = 1;
	int AllocSerialNumber();
	float m_flSceneScale = 1.0f;
	
	CEditHistory* m_pEditHistory;


	// <----

	DrawMesh*	m_pUnitBoundingBox;
	DrawMesh*	m_pIntensitySphere;
	DrawMesh*	m_pSpotlightCone;
	
	ModelOBJ*	m_pDirectionModel;
	DrawMesh*	m_pDirectionArrow;


	void DrawLightHelperGeometry(lightDefWPtr_t pObject);
	lightDefWPtr_t m_pCurrentSelection;

};

