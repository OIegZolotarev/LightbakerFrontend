#pragma once

#include "common.h"
#include "main_window.h"
#include "lights.h"
#include "model_obj.h"

class SceneRenderer: public IEventHandler
{	
public:
	SceneRenderer(class MainWindow* pTargetWindow);
	~SceneRenderer();

	class Camera* GetCamera();

	void	RenderScene();
	int		HandleEvent(bool bWasHandled, SDL_Event& e) override;
	float	FrameDelta();
	void LoadModel(const char* dropped_filedir);

	bool	IsModelLoaded();
	
	std::string GetModelFileName();
	std::string GetModelTextureName();

	void ReloadModel();

	void DrawBillboard(glm::vec3 pos, glm::vec2 size, gltexture_t* texture, glm::vec3 tint);
	void DrawBillboardSelection(glm::vec3 pos, glm::vec2 size, gltexture_t* texture, int index);

	lightDefWPtr_t GetLightWeakRef(lightDef_s* param1);
	
	void ExportModelForCompiling(const char* path);
	void AddNewLight(LightTypes type);

private:
	void Debug_DrawGround();

	class Camera*		m_pCamera;
	class MainWindow*	m_pTargetWindow;

	ModelOBJ* m_pSceneModel = nullptr;

	std::vector<lightDefPtr_t> m_vecSceneLightDefs;

	DrawMesh*	m_pUnitBoundingBox;
	DrawMesh*	m_pIntensitySphere;
	void DrawBoundingBoxAroundLight(lightDefWPtr_t pObject);
};

