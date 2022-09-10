#pragma once

#include "common.h"
#include "ui_common.h"
#include "scene_renderer.h"
#include "console.h"

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
public:
	MainWindow(const char* title, glm::vec2 defaultSize);
	~MainWindow();

	void MainLoop();

	int		Width() { return m_iWindowWidth;  }
	int		Height() { return m_iWindowHeight; }
	
	SDL_Window* Handle()
	{
		return m_pSDLWindow;
	}

	float FrameDelta();

	DebugConsole* Console()
	{
		return &m_Console;
	}

	class SceneRenderer* GetSceneRenderer();

private:

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
	float RenderMainToolbar(float menuHeight);

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

	class SceneRenderer* m_pSceneRenderer;

	std::vector<IEventHandler*> m_vEventHandlers;

	bool PropagateControlsEvent(SDL_Event& e);
	void InitCommands();

	bool m_bShowConsole = true;
	DebugConsole m_Console;

	void InitBackend();

	ImGuiID DockSpaceOverViewport(float heightAdjust, ImGuiDockNodeFlags dockspace_flags, const ImGuiWindowClass* window_class);

};

