#pragma once

#include "common.h"
#include "main_window.h"
#include "lights.h"
#include "model_obj.h"


class SceneRenderer: public IEventHandler
{	
	void Debug_DrawGround();

	class Camera*		m_pCamera;
	class MainWindow* m_pTargetWindow;

	ModelOBJ* m_pSceneModel = nullptr;

	std::vector<lightDef_t> m_vecSceneLightDefs;

public:
	SceneRenderer(MainWindow * pTargetWindow);
	~SceneRenderer();

	void RenderScene();	
	int HandleEvent(bool bWasHandled, SDL_Event& e) override;
	float FrameDelta();
	void LoadModel(char* dropped_filedir);
};

