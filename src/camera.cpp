#include "common.h"
#include "camera.h"



#include "application.h"


Camera::Camera(SceneRenderer * pSceneRenderer)
{
	m_Origin = { 0,0,0 };
	m_Angles = { 0,0,0 };
	
	m_matModelView = glm::mat4(1.f);
	m_matProjection = glm::mat4(1.f);

	SetupCommonKeystrokesCallbacks();
	SetupKeystrokes();
	m_pSceneRenderer = pSceneRenderer;
}

Camera::~Camera()
{
	ClearPointersVector(m_vKeyStrokesBlenderTouchpad);
}

void Camera::SetupKeystrokes()
{
	SetupKeystrokesBlenderTouchpad();
	SetupKeystrokesVHE();
}

void Camera::SetupKeystrokesBlenderTouchpad()
{
	CameraCommandKeyStroke* keystrokeRotate = new CameraCommandKeyStroke();
	keystrokeRotate->AddMouseKey(SDL_BUTTON_LMASK);
	keystrokeRotate->AddKeyboardKey("Left Alt");
	keystrokeRotate->SetCallback(callbackRotate);

	m_vKeyStrokesBlenderTouchpad.push_back(keystrokeRotate);

	// Panning

	CameraCommandKeyStroke* keystrokePan = new CameraCommandKeyStroke();
	keystrokePan->AddMouseKey(SDL_BUTTON_LMASK);
	keystrokePan->AddKeyboardKey("Left Shift");
	keystrokePan->AddKeyboardKey("Left Alt");
	keystrokePan->SetCallback(callbackPan);


	m_vKeyStrokesBlenderTouchpad.push_back(keystrokePan);

	// Zoom

	CameraCommandKeyStroke* keystrokeZoom = new CameraCommandKeyStroke();
	keystrokeZoom->AddMouseKey(SDL_BUTTON_LMASK);
	keystrokeZoom->AddKeyboardKey("Left Alt");
	keystrokeZoom->AddKeyboardKey("Left Ctrl");
	keystrokeZoom->SetCallback(callbackZoom);
	m_vKeyStrokesBlenderTouchpad.push_back(keystrokeZoom);

	// Wheel and +-	

	CameraCommandKeyStroke* keystrokeZoomIn = new CameraCommandKeyStroke();
	keystrokeZoomIn->AddKeyboardKey("Keypad +");
	keystrokeZoomIn->SetWheelDirection(1, false);
	keystrokeZoomIn->SetCallback(callbackZoomIn);
	m_vKeyStrokesBlenderTouchpad.push_back(keystrokeZoomIn);

	CameraCommandKeyStroke* keystrokeZoomOut = new CameraCommandKeyStroke();
	keystrokeZoomOut->AddKeyboardKey("Keypad -");
	keystrokeZoomOut->SetWheelDirection(-1, false);
	keystrokeZoomOut->SetCallback(callbackZoomOut);
	m_vKeyStrokesBlenderTouchpad.push_back(keystrokeZoomOut);
}

void Camera::SetupCommonKeystrokesCallbacks()
{
	//#define DEBUG_KEYSTROKES

	callbackRotate = pfnKeyStrokeCallback([&](bool bHit, SDL_Event& event) -> void
		{
#ifdef DEBUG_KEYSTROKES
			//Con_Printf("callbackRotate(%d)\n", bHit);
#endif

			if (bHit)
				m_Mode = CameraMouseModes::Rotate;
			else
				m_Mode = CameraMouseModes::None;
		});

	callbackPan = pfnKeyStrokeCallback([&](bool bHit, SDL_Event& event) -> void
		{

#ifdef DEBUG_KEYSTROKES
			//Con_Printf("callbackPan(%d) event == %d\n", bHit,event.type);
#endif

			if (bHit)
				m_Mode = CameraMouseModes::Pan;
			else
				m_Mode = CameraMouseModes::None;
		});

	callbackZoom = pfnKeyStrokeCallback([&](bool bHit, SDL_Event& event) -> void
		{

#ifdef DEBUG_KEYSTROKES
			//Con_Printf("callbackZoom(%d)\n", bHit);
#endif

			if (bHit)
				m_Mode = CameraMouseModes::Zoom;
			else
				m_Mode = CameraMouseModes::None;
		});

	callbackZoomIn = pfnKeyStrokeCallback([&](bool bHit, SDL_Event& event) -> void
		{

#ifdef DEBUG_KEYSTROKES
			//	Con_Printf("callbackZoomIn(%d)\n", bHit);
#endif

			if (bHit)
				DoZoomIn();
			else
				m_Mode = CameraMouseModes::None;
		});

	callbackZoomOut = pfnKeyStrokeCallback([&](bool bHit, SDL_Event& event) -> void
		{

#ifdef DEBUG_KEYSTROKES
			//Con_Printf("callbackZoomOut(%d)\n", bHit);
#endif

			if (bHit)
				DoZoomOut();
			else
				m_Mode = CameraMouseModes::None;
		});

	callbackToggleFPSNavigation = pfnKeyStrokeCallback([&](bool bHit, SDL_Event& event) -> void
		{

#ifdef DEBUG_KEYSTROKES
			//Con_Printf("callbackToggleFPSNavigation(%d)\n", bHit);
#endif
			if (bHit)
			{
				
				auto windowHandle = Application::GetMainWindow()->Handle();
				auto viewport = Application::GetMainWindow()->Get3DGLViewport();

				int winWidth = Application::GetMainWindow()->Width();
				int winHeight = Application::GetMainWindow()->Height();

				SDL_ShowCursor(SDL_DISABLE);

				SDL_RaiseWindow(windowHandle);
				SDL_WarpMouseInWindow(windowHandle, winWidth / 2, winHeight / 2);
							

				SDL_ShowCursor(SDL_ENABLE);



				m_bFPSNavigation = !m_bFPSNavigation;
			}
		});


	static const int moveSpeed = 200;

	callbackMoveForward = pfnKeyStrokeCallback([&](bool bHit, SDL_Event& event) -> void
		{
			if (bHit)
				SetForwardSpeed(moveSpeed);
			else
				SetForwardSpeed(0);
		});

	callbackMoveBackward = pfnKeyStrokeCallback([&](bool bHit, SDL_Event& event) -> void
		{
			if (bHit)
				SetForwardSpeed(-moveSpeed);
			else
				SetForwardSpeed(0);
		});

	callbackStrafeLeft = pfnKeyStrokeCallback([&](bool bHit, SDL_Event& event) -> void
		{
			if (bHit)
				SetStrafeSpeed(-moveSpeed);
			else
				SetStrafeSpeed(0);
		});


	callbackStrafeRight = pfnKeyStrokeCallback([&](bool bHit, SDL_Event& event) -> void
		{
			if (bHit)
				SetStrafeSpeed(moveSpeed);
			else
				SetStrafeSpeed(0);
		});

	callbackMoveup = pfnKeyStrokeCallback([&](bool bHit, SDL_Event& event) -> void
		{
			if (bHit)
				SetUpSpeed(moveSpeed);
			else
				SetUpSpeed(0);
		});

}

glm::vec3& Camera::GetAngles()
{
	return m_Angles;
}

glm::vec3& Camera::GetOrigin()
{
	return m_Origin;
}

void Camera::Apply()
{
	UpdateOrientation();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	SetupPerspectiveMatrix();
	
	glDepthRange(0, 1);
	glMatrixMode(GL_MODELVIEW);

	SetupModelViewMatrix();
}

void Camera::Reset()
{

}

void Camera::PopMatrices(int which, bool bApply)
{

}

void Camera::PushMatrices(int which)
{

}

float* Camera::GetViewMatrix()
{
	return &m_matModelView[0][0];
}

float* Camera::GetProjectionMatrix()
{
	return &m_matProjection[0][0];
}

int Camera::HandleEvent(bool bWasHandled, SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEMOTION:
		return MouseMotionEvent(event);
		break;
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
	case SDL_KEYUP:
	case SDL_KEYDOWN:
		return ButtonEvent(event);
		break;
	case SDL_MOUSEWHEEL:
		return MouseWheelEvent(event);
		break;

	}

	return 0;
}

void Camera::UpdateOrientation()
{
	if (m_bFPSNavigation)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);

		int winWidth = Application::GetMainWindow()->Width();
		int winHeight = Application::GetMainWindow()->Height();

		int mx = winWidth / 2;
		int my = winHeight / 2;

		float flFrameDelta = m_pSceneRenderer->FrameDelta();

		float xDelta = (x - mx);
		float yDelta = (y - my);

		m_Angles[PITCH] += yDelta / 10;
		m_Angles[YAW] += xDelta / 10;

		SDL_WarpMouseInWindow(Application::GetMainWindow()->Handle(), winWidth / 2, winHeight / 2);

		m_Origin += (m_MoveSpeeds[0] * flFrameDelta) * m_vForward + (m_MoveSpeeds[1] * flFrameDelta) * m_vRight + (m_MoveSpeeds[2] * flFrameDelta) * m_vUp;
	}

}

glm::vec3 Camera::GetRightVector()
{
	return m_vRight;
}

glm::vec3 Camera::GetUpVector()
{
	return m_vUp;
}

void Camera::SetupKeystrokesVHE()
{
	CameraCommandKeyStroke* keystrokeMoveForward = new CameraCommandKeyStroke();
	keystrokeMoveForward->AddKeyboardKey("W");
	keystrokeMoveForward->SetCallback(callbackMoveForward);
	m_vKeyStrokesVHE.push_back(keystrokeMoveForward);

	CameraCommandKeyStroke* keystrokeMoveBackward = new CameraCommandKeyStroke();
	keystrokeMoveBackward->AddKeyboardKey("S");
	keystrokeMoveBackward->SetCallback(callbackMoveBackward);
	m_vKeyStrokesVHE.push_back(keystrokeMoveBackward);

	CameraCommandKeyStroke* keystrokeStrafeRight = new CameraCommandKeyStroke();
	keystrokeStrafeRight->AddKeyboardKey("D");
	keystrokeStrafeRight->SetCallback(callbackStrafeRight);
	m_vKeyStrokesVHE.push_back(keystrokeStrafeRight);

	CameraCommandKeyStroke* keystrokeStrafeLeft = new CameraCommandKeyStroke();
	keystrokeStrafeLeft->AddKeyboardKey("A");
	keystrokeStrafeLeft->SetCallback(callbackStrafeLeft);
	m_vKeyStrokesVHE.push_back(keystrokeStrafeLeft);

	CameraCommandKeyStroke* keystrokeMoveup = new CameraCommandKeyStroke();
	keystrokeMoveup->AddKeyboardKey("Space");
	keystrokeMoveup->SetCallback(callbackMoveup);
	m_vKeyStrokesVHE.push_back(keystrokeMoveup);

	CameraCommandKeyStroke* keystrokeToggleFPSNavigation = new CameraCommandKeyStroke();
	keystrokeToggleFPSNavigation->AddKeyboardKey("Z");
	keystrokeToggleFPSNavigation->SetCallback(callbackToggleFPSNavigation);
	m_vKeyStrokesVHE.push_back(keystrokeToggleFPSNavigation);

	// Wheel and +-	

	CameraCommandKeyStroke* keystrokeZoomIn = new CameraCommandKeyStroke();
	keystrokeZoomIn->AddKeyboardKey("Keypad +");
	keystrokeZoomIn->SetWheelDirection(1, false);
	keystrokeZoomIn->SetCallback(callbackZoomIn);
	m_vKeyStrokesVHE.push_back(keystrokeZoomIn);

	CameraCommandKeyStroke* keystrokeZoomOut = new CameraCommandKeyStroke();
	keystrokeZoomOut->AddKeyboardKey("Keypad -");
	keystrokeZoomOut->SetWheelDirection(-1, false);
	keystrokeZoomOut->SetCallback(callbackZoomOut);
	m_vKeyStrokesVHE.push_back(keystrokeZoomOut);
}

void Camera::SetForwardSpeed(const int moveSpeed)
{
	m_MoveSpeeds[0] = moveSpeed;
}

void Camera::SetStrafeSpeed(const int moveSpeed)
{
	m_MoveSpeeds[1] = moveSpeed; 
}

void Camera::SetUpSpeed(const int moveSpeed)
{
	m_MoveSpeeds[2] = moveSpeed;
}

int Camera::MouseMotionEvent(SDL_Event& _event)
{
	auto event = _event.motion;
	
	float flFrameDelta = m_pSceneRenderer->FrameDelta();
	float flDist = std::max(glm::length(m_Origin),0.1f);

	if (isnan(flDist))
	{
		flDist = 10;
		m_Origin = glm::vec3(0, 0, 10);
		return 0;
	}

	switch (m_Mode)
	{
	case CameraMouseModes::Pan:
	{
		float xDelta = event.xrel * flFrameDelta * flDist;
		float yDelta = event.yrel * flFrameDelta * flDist;

		m_Origin += m_vUp * yDelta + m_vRight * -xDelta;
		return EVENT_FINISHED;
	}
	break;
	case CameraMouseModes::Rotate:
	{
		float xDelta = event.xrel * flFrameDelta * 20;
		float yDelta = event.yrel * flFrameDelta * 20;

		m_Angles[PITCH] += yDelta;
		m_Angles[YAW] += xDelta;

		return EVENT_FINISHED;

	}
	break;
	case CameraMouseModes::Zoom:
	{
		float xDelta = event.xrel * flFrameDelta * 100;
		float yDelta = event.yrel * flFrameDelta * 100;

		m_Origin += m_vForward * (xDelta + yDelta);
		return EVENT_FINISHED;
	}
	case CameraMouseModes::None:
		break;
	default:
		break;

	}

	return 0;
}

int Camera::ButtonEvent(SDL_Event& _event)
{
	auto event = _event.button;

	// TODO: selectable keystrokes maybe?
	KeystrokeList & keystrokeSet = m_vKeyStrokesVHE;

	if (keystrokeSet.empty())
		return 0;

	CameraCommandKeyStroke* best = 0;

	for (auto& it : keystrokeSet)
	{
		if (it->AreKeysPressed())
		{
			best = it;
		}
		else
			it->ExecuteCallback(false, _event);
	}

	if (best)
	{
		best->ExecuteCallback(true, _event);
		return EVENT_FINISHED;
	}
	else
		return 0;
}

int Camera::MouseWheelEvent(SDL_Event& _event)
{
	auto event = _event.motion;

	float flFrameDelta = m_pSceneRenderer->FrameDelta();

	float flDist = glm::length(m_Origin);

	if (isnan(flDist))
	{
		flDist = 10;
		m_Origin = glm::vec3(0, 0, 10);
		return 0;
	}

	switch (m_Mode)
	{
	case CameraMouseModes::Pan:
	{
		float xDelta = event.xrel * flFrameDelta * flDist;
		float yDelta = event.yrel * flFrameDelta * flDist;

		m_Origin += m_vUp * yDelta + m_vRight * -xDelta;

		return EVENT_FINISHED;
	}
	break;
	case CameraMouseModes::Rotate:
	{
		float xDelta = event.xrel * flFrameDelta * 20;
		float yDelta = event.yrel * flFrameDelta * 20;

		m_Angles[PITCH] += yDelta;
		m_Angles[YAW] += xDelta;
		
		return EVENT_FINISHED;
	}
	break;
	case CameraMouseModes::Zoom:
	{
		float xDelta = event.xrel * flFrameDelta * 100;
		float yDelta = event.yrel * flFrameDelta * 100;

		m_Origin += m_vForward * (xDelta + yDelta);
		
		return EVENT_FINISHED;
	}
	case CameraMouseModes::None:
		break;
	default:
		break;

	}

	return 0;
}

void Camera::SetupPerspectiveMatrix()
{
	glMatrixMode(GL_PROJECTION);

	double fov = glm::radians(m_flFov);

	auto vp = Application::GetMainWindow()->Get3DGLViewport();

	float width = vp[2];
	float height = vp[3];

	if (!width || !height)
		return;

	double aspect = width / height;
	double fov_y = 2 * atan(tan(fov / 2) / aspect); // Ìîÿ ôîðìóëà - ïëàâàåò?
	double dbg = glm::degrees(fov_y);
	
	m_matProjection = glm::perspective(fov_y, aspect, (double)m_flZNear, (double)m_flZFar);

	glLoadMatrixf((float*)&m_matProjection);
}

void Camera::SetupModelViewMatrix()
{
	glMatrixMode(GL_MODELVIEW);

	m_matModelView = glm::mat4(1);

	glm::mat4 ident;

//#define QUAKE_STYLE

#ifdef QUAKE_STYLE
 	ident = glm::mat4(1);
 	ident = glm::rotate(ident, glm::radians(90.f), glm::vec3(0, 0, 1));
 	m_matModelView *= ident;
 
 	ident = glm::mat4(1);
 	ident = glm::rotate(ident, -glm::radians(180.f), glm::vec3(0, 1, 0));
 	m_matModelView *= ident;

	ident = glm::mat4(1);
	ident = glm::rotate(ident, -glm::radians(m_Angles[ROLL]), glm::vec3(1, 0, 0));
	m_matModelView *= ident;

	ident = glm::mat4(1);
	ident = glm::rotate(ident, -glm::radians(m_Angles[PITCH]), glm::vec3(0, 1, 0));
	m_matModelView *= ident;

	ident = glm::mat4(1);
	ident = glm::rotate(ident, glm::radians(m_Angles[YAW]), glm::vec3(0, 0, 1));
	m_matModelView *= ident;
#else
	ident = glm::mat4(1);
	ident = glm::rotate(ident, glm::radians(m_Angles[PITCH]), glm::vec3(1, 0, 0));
	m_matModelView *= ident;

	ident = glm::mat4(1);
	ident = glm::rotate(ident, glm::radians(m_Angles[YAW]), glm::vec3(0, 1, 0));
	m_matModelView *= ident;


#endif

	m_matModelView = glm::translate(m_matModelView, -m_Origin);

	glLoadMatrixf((float*)&m_matModelView);

	auto matInv = glm::inverse(m_matModelView);

	m_vForward = -matInv[2];
	m_vRight = matInv[0];
	m_vUp = matInv[1];
}

CameraCommandKeyStroke::CameraCommandKeyStroke()
{
	for (int i = 0; i < 9; i++)
		m_KeyboardKeys[i] = -1;

	m_MouseKeys = 0;
}

CameraCommandKeyStroke::~CameraCommandKeyStroke()
{

}

void CameraCommandKeyStroke::AddKeyboardKey(const char* keyName)
{
	auto keyCode = SDL_GetKeyFromName(keyName);
	auto scanCode = SDL_GetScancodeFromKey(keyCode);

	for (auto& it : m_KeyboardKeys)
	{
		if (it == -1)
		{
			it = scanCode;
			return;
		}
	}

	Application::EPICFAIL("CameraCommandKeyStroke::AddKeyboardKey(): keystroke is too long");

}

void CameraCommandKeyStroke::AddMouseKey(int keyMask)
{
	m_MouseKeys |= keyMask;
}

bool CameraCommandKeyStroke::AreKeysPressed()
{
	const uint8_t* keyboardState = SDL_GetKeyboardState(0);
	const uint32_t mouseState = SDL_GetMouseState(0, 0);

	for (auto& it : m_KeyboardKeys)
	{
		if (it == -1)
			break;

		if (!keyboardState[it])
			return false;
	}

	if (m_MouseKeys)
	{
		if (mouseState & m_MouseKeys)
			return true;
	}
	else
		return true;

	return false;
}

void CameraCommandKeyStroke::SetCallback(pfnKeyStrokeCallback callback)
{
	m_Callback = callback;
}

void CameraCommandKeyStroke::ExecuteCallback(bool bHit, SDL_Event& event)
{
	m_Callback(bHit, event);
}

void CameraCommandKeyStroke::SetWheelDirection(int direction, bool needOtherKeys)
{
	m_WheelConditions = direction;
	m_bNeedOtherKeys = needOtherKeys;
}

bool CameraCommandKeyStroke::CanBeExecuteFromMouseWheel(int direction)
{
	bool sameSign = (m_WheelConditions ^ direction) >= 0;

	if (m_bNeedOtherKeys)
	{
		return sameSign && AreKeysPressed();
	}
	else
		return sameSign;
}


void Camera::DoZoomIn()
{	
	m_Origin += m_vForward * std::max(glm::length(m_Origin) / 2.f,1.f);
}

void Camera::DoZoomOut()
{	
	m_Origin -= m_vForward * std::max(glm::length(m_Origin) / 2.f,1.f);
}
