/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

#include "common.h"
#include "scene_renderer.h"
#include "event_handler.h"
#include <stack>

enum class CameraMouseModes
{
	Pan,
	Rotate,
	Zoom,
	None
};

#define PITCH 0
#define YAW 1
#define ROLL 2

#define PUSH_PROJECTION (1<<0)
#define PUSH_MODELVIEW (1<<1)
#define PUSH_BOTH (PUSH_PROJECTION | PUSH_MODELVIEW)

#define POP_PROJECTION (1<<0)
#define POP_MODELVIEW (1<<1)
#define POP_BOTH (POP_MODELVIEW | POP_PROJECTION)

typedef std::function<void(bool bHit, SDL_Event& event)> pfnKeyStrokeCallback;


class CameraCommandKeyStroke
{
	int m_iDebugTag = -1;

	int m_KeyboardKeys[9];
	int m_MouseKeys = 0;
	int m_WheelConditions;
	bool m_bNeedOtherKeys;

	glm::vec3 m_vecModeSpeed;

	pfnKeyStrokeCallback m_Callback;

public:

	void SetDebugTag(int tag) { m_iDebugTag = tag; };
	int GetDebugTag() { return m_iDebugTag; }

	CameraCommandKeyStroke();
	~CameraCommandKeyStroke();

	void AddKeyboardKey(const char* keyName);
	void AddMouseKey(int keyMask);

	bool AreKeysPressed();

	void SetCallback(pfnKeyStrokeCallback callback);
	void ExecuteCallback(bool bHit, SDL_Event& event);
	void SetWheelDirection(int direction, bool needOtherKeys);

	bool CanBeExecuteFromMouseWheel(int direction);

};

typedef std::vector<CameraCommandKeyStroke*> KeystrokeList;

class Camera: public IEventHandler
{
	CameraMouseModes m_Mode = CameraMouseModes::None;

	KeystrokeList m_vKeyStrokesBlenderTouchpad;
	KeystrokeList m_vKeyStrokesVHE;

	void SetupKeystrokes();

	void SetupCommonKeystrokesCallbacks();
	void SetupKeystrokesBlenderTouchpad();
	void SetupKeystrokesVHE();

	pfnKeyStrokeCallback callbackRotate;
	pfnKeyStrokeCallback callbackPan;
	pfnKeyStrokeCallback callbackZoom;
	pfnKeyStrokeCallback callbackZoomIn;
	pfnKeyStrokeCallback callbackZoomOut;

	pfnKeyStrokeCallback callbackToggleFPSNavigation;
	pfnKeyStrokeCallback callbackMoveForward;
	pfnKeyStrokeCallback callbackMoveBackward;
	pfnKeyStrokeCallback callbackStrafeLeft;
	pfnKeyStrokeCallback callbackStrafeRight;
	pfnKeyStrokeCallback callbackMoveup;

	void SetForwardSpeed(const int moveSpeed);
	void SetStrafeSpeed(const int moveSpeed);
	void SetUpSpeed(const int moveSpeed);
	
	bool  m_bFPSNavigation = false;

	float m_flFov = 110;
	float m_flZNear = 1.f;
	float m_flZFar = 4096.f;

	glm::vec3 m_vForward = { 1.f,0,0 };
	glm::vec3 m_vRight =   { 0,1,0	 };
	glm::vec3 m_vUp =	   { 0,0,1   };

	glm::vec3 m_Origin = {0,	0,	0};
	glm::vec3 m_Angles = {90,	0,	0};

	glm::mat4			  m_matProjection;
	glm::mat4			  m_matModelView;

	std::stack<glm::mat4> m_matModelViewStack;
	std::stack<glm::mat4> m_matProjectionStack;
	
	void SetupPerspectiveMatrix();
	void SetupModelViewMatrix();

	int MouseMotionEvent(SDL_Event& event);
	int ButtonEvent(SDL_Event& event);
	int MouseWheelEvent(SDL_Event& event);

	void DoZoomIn();
	void DoZoomOut();

	class SceneRenderer*		m_pSceneRenderer;

public:
	Camera(SceneRenderer * pSceneRenderer);
	~Camera();

	glm::vec3&	GetAngles();
	glm::vec3&	GetOrigin();

	void		Apply();
	void		Reset();

	void		PopMatrices(int which, bool bApply);
	void		PushMatrices(int which);

	float*		GetViewMatrix();
	float*		GetProjectionMatrix();

	int			HandleEvent(bool bWasHandled, SDL_Event& e) override;

	glm::vec3	GetRightVector();
	glm::vec3	GetUpVector();
	glm::vec3	GetForwardVector();

private:
	int m_MoveSpeeds[3];
	void UpdateOrientation();

	
};