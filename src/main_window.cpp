#include "ui_common.h"
#include "main_window.h"
#include "application.h"

SDL_Cursor* g_EmptyCursor;
char g_IMGuiIniPath[1024];

MainWindow::MainWindow(const char* title, glm::vec2 defaultSize): m_iWindowWidth(defaultSize.x),m_iWindowHeight(defaultSize.y)
{
	// initiate SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		//	EPICFAIL("[ERROR] %s\n", SDL_GetError());
		//return -1;
	}

	std::string glsl_version = "";
	glsl_version = "#version 150";

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL| SDL_WINDOW_RESIZABLE| SDL_WINDOW_ALLOW_HIGHDPI);
	m_pSDLWindow = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,defaultSize.x,defaultSize.y,window_flags);

	// limit to which minimum size user can resize the window
	SDL_SetWindowMinimumSize(m_pSDLWindow, 500, 300);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	m_pGLContext = SDL_GL_CreateContext(m_pSDLWindow);

#ifdef WIN32
	SDL_GL_MakeCurrent(m_pSDLWindow, m_pGLContext);
#endif

	// enable VSync
	SDL_GL_SetSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		printf("[ERROR] Couldn't initialize glad\n");
	}
	else
		printf("gladInitialized\n");

	glViewport(0, 0, m_iWindowWidth, m_iWindowHeight);

	// setup Dear ImGui context
	IMGUI_CHECKVERSION();
	m_pImGUIContext = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NoMouseCursorChange;
	io.MouseDrawCursor = true;

	char* p = SDL_GetPrefPath("QuiteOldOrange", "LightBaker3000Frontend");
	sprintf_s(g_IMGuiIniPath, sizeof(g_IMGuiIniPath), "%s/imgui.ini", p);

	io.IniFilename = g_IMGuiIniPath;

	// setup Dear ImGui style
	ImGui::StyleColorsClassic();

	ImGuiStyle* style = &ImGui::GetStyle();

	style->FrameBorderSize = 1;
	style->TabBorderSize = 1;
	style->WindowRounding = 4;


	// setup platform/renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(m_pSDLWindow, m_pGLContext);
	ImGui_ImplOpenGL3_Init(glsl_version.c_str());


	int32_t cursorData[2] = { 0, 0 };
	g_EmptyCursor = SDL_CreateCursor((Uint8*)cursorData, (Uint8*)cursorData, 8, 8, 4, 4);
	SDL_SetCursor(g_EmptyCursor);
}

MainWindow::~MainWindow()
{

}

void MainWindow::MainLoop()
{
	m_TimersData.timestamp_now = SDL_GetPerformanceCounter();
	m_TimersData.timestamp_last = SDL_GetPerformanceCounter();
	m_TimersData.frames_until_init = 3;
	m_TimersData.fps_accum = 0;
	m_TimersData.num_frames_this_second = 0;

	while (!m_bTerminated)
	{
		m_bTerminated = !HandleEvents(!m_bTerminated);
		
		UpdateTimers();
		GL_BeginFrame();
		
		//m_pSceneRenderer->RenderScene();

		RenderGUI();

		LimitToTargetFPS();
		SDL_GL_SwapWindow(m_pSDLWindow);
	}
}

float MainWindow::RenderMainMenu()
{
	float size = 0;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load model...", "Ctrl-O"))
			{
				// TODO
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Recent"))
			{
				ImGui::MenuItem("( none )");

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit"))
			{				
				m_bTerminated = true;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About..."))
			{
				//m_pPopupsManager->ShowPopup(PopupWindows::AboutApplicationDialog);
			}

			if (ImGui::MenuItem("Style editor..."))
			{
				//m_pPopupsManager->ShowPopup(PopupWindows::StyleEditor);
			}

			if (ImGui::MenuItem("Style selector..."))
			{
				//m_pPopupsManager->ShowPopup(PopupWindows::StyleChooser);
			}

			ImGui::EndMenu();
		}

		size = ImGui::GetWindowHeight();

		ImGui::EndMainMenuBar();
	}

	return size;
}

void MainWindow::RenderGUI()
{
	glViewport(0, 0, m_iWindowWidth, m_iWindowHeight);

	// start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_pSDLWindow);

	ImGui::NewFrame();

	float menuHeight = RenderMainMenu();

	static bool bShowDebug = true;

	if (bShowDebug)
	{
		ImGui::Begin("Debug", &bShowDebug);
		ImGui::Text("FPS: %d\n", m_TimersData.actual_fps);
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool MainWindow::HandleEvents(bool loop)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{		
		ImGui_ImplSDL2_ProcessEvent(&event);
		
		bool bIgnorableEvent = true;

		switch (event.type)
		{
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			bIgnorableEvent = false;
		}

		if (ImGui::GetIO().WantCaptureMouse && bIgnorableEvent)
			continue;

		if (ImGui::GetIO().WantCaptureKeyboard && bIgnorableEvent)
			continue;

		switch (event.type)
		{
		case SDL_QUIT:
			loop = false;
			break;

		case SDL_WINDOWEVENT:
			switch (event.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
			case SDL_WINDOWEVENT_SIZE_CHANGED:
			case SDL_WINDOWEVENT_MINIMIZED:
			case SDL_WINDOWEVENT_MAXIMIZED:
			case SDL_WINDOWEVENT_RESTORED:
				SDL_GetWindowSize(m_pSDLWindow, &m_iWindowWidth, &m_iWindowHeight);
				glViewport(0, 0, m_iWindowWidth, m_iWindowHeight);
				break;
			}
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				break;
			}
			// TODO: implement
// 			m_pCommandsRegistry->OnKeyDown();
// 			if (PropagateControlsEvent(event)) break;
			break;

		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEWHEEL:
		case SDL_KEYUP:
			// TODO: implement
			//PropagateControlsEvent(event);
			break;

		case (SDL_DROPFILE):
		{
			// TODO: implement
		}
		}
	}		
	
	return loop;
}

void MainWindow::GL_BeginFrame()
{
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1);

// 	if (settingMultisample->GetAsBool())
// 		glEnable(GL_MULTISAMPLE);
// 	else
// 		
	glDisable(GL_MULTISAMPLE);
	glClearColor(0.25, .25, .25, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MainWindow::UpdateTimers()
{	
	if (m_TimersData.frames_until_init > 0)
	{
		m_TimersData.frames_until_init--;

		if (m_TimersData.frames_until_init == 0)
		{
			// DelayedInit()
		}

	}

	m_TimersData.timestamp_last = m_TimersData.timestamp_now;
	m_TimersData.timestamp_now = SDL_GetPerformanceCounter();

	m_TimersData.frame_delta = (double)((m_TimersData.timestamp_now - m_TimersData.timestamp_last) * 1000 / (double)SDL_GetPerformanceFrequency());
	m_TimersData.fps_accum += m_TimersData.frame_delta;
	
	if (m_TimersData.fps_accum > 1000)
	{
		m_TimersData.actual_fps = m_TimersData.num_frames_this_second;
		m_TimersData.num_frames_this_second = 0;
		m_TimersData.fps_accum = 0;
	}

	m_TimersData.num_frames_this_second++;
}

void MainWindow::LimitToTargetFPS()
{
#ifdef WIN32
	int target_fps = 60;
	static uint64_t next_tick = 0;
	uint64_t this_tick = SDL_GetTicks64();
	if (this_tick < next_tick)
	{
		SDL_Delay((uint32_t)(next_tick - this_tick));
	}
	//this_tick = SDL_GetTicks64(); // WTF?
	next_tick = this_tick + (1000 / target_fps);
#else
	int target_fps = m_pSettings->Get(IDS_FPS_LIMIT)->GetAsInt();
	static uint32_t next_tick = 0;
	uint32_t this_tick = SDL_GetTicks();
	if (this_tick < next_tick)
	{
		SDL_Delay((uint32_t)(next_tick - this_tick));
	}
	//this_tick = SDL_GetTicks64(); // WTF?
	next_tick = this_tick + (1000 / target_fps);
#endif
}
