#pragma once

#include "common.h"
#include "ui_common.h"

struct timersData
{
	double frame_delta = 0;
	int actual_fps = 0;

	Uint64 timestamp_now = 0;
	Uint64 timestamp_last = 0;

	int frames_until_init = 3;

	double fps_accum = 0;
	int num_frames_this_second = 0;
};



class MainWindow
{

	enum ToolbarIcons
	{
		AddOmni = 0,
		AddSpot,
		AddDirect,
		BakeScene,
		MaxIcons
	};

	GLuint m_ToolbarIcons[ToolbarIcons::MaxIcons];

	bool RenderToolbarIcon(GLuint iconId);

	float RenderMainMenu();
	void RenderMainToolbar(float menuHeight);

	std::string m_strTitle;
	SDL_Window* m_pSDLWindow;

	int	m_iWindowHeight;
	int m_iWindowWidth;

	SDL_GLContext m_pGLContext;
	ImGuiContext* m_pImGUIContext;

	timersData m_TimersData;
	bool m_bTerminated;

	bool HandleEvents(bool loop);

	void UpdateTimers();
	void GL_BeginFrame();	
	void RenderGUI();
	void LimitToTargetFPS();

public:
	MainWindow(const char* title, glm::vec2 defaultSize);
	~MainWindow();

	void MainLoop();


};

