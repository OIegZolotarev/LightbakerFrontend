#pragma once

#include "common.h"
#include "main_window.h"
#include "model_obj.h"

// Light flags from LB3k app

#define LF_EULER	(1<<0)
#define LF_DYN		(1<<1)
#define LF_XYZ		(1<<2)
#define LF_LINEAR	(1<<3)
#define LF_DISK		(1<<4)
#define LF_RECT		(1<<5)

typedef struct viewstate_s
{
	glm::vec3 origin;
	glm::vec3 angles;

	float fov;

	int viewport[4];

}viewstate_t;

class SceneRenderer: public IEventHandler
{	
	void Debug_DrawGround();

	class Camera*		m_pCamera;
	class MainWindow* m_pTargetWindow;

	ModelOBJ* m_pSceneModel = nullptr;

public:
	SceneRenderer(MainWindow * pTargetWindow);
	~SceneRenderer();

	void RenderScene();	
	int HandleEvent(bool bWasHandled, SDL_Event& e) override;
	float FrameDelta();
	void LoadModel(char* dropped_filedir);
};

