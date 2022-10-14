#include "ui_common.h"
#include "main_window.h"
#include "application.h"
#include "common_resources.h"
#include "imgui_internal.h"
#include "properties_editor.h"
#include "Camera.h"
#include "imgui_popups.h"
#include "scene_objects_panel.h"
#include "console_output_panel.h"
#include "debug_panel.h"


const char* strDockspace = "DockSpace";
ImGuiID gIDMainDockspace = 0;

SDL_Cursor* g_EmptyCursor;
char g_IMGuiIniPath[1024];

MainWindow::MainWindow(const char* title, glm::vec2 defaultSize): m_iWindowWidth(defaultSize.x),m_iWindowHeight(defaultSize.y)
{
	m_strTitle = title;

	InitBackend();
	InitCommonResources();

	m_pSceneRenderer = new SceneRenderer(this);
	m_vEventHandlers.push_back(m_pSceneRenderer);

	InitCommands();

	m_vPanels.push_back(ObjectPropertiesEditor::Instance());
	m_vPanels.push_back(new SceneObjectPanel);
	m_vPanels.push_back(new ConsoleOutputPanel(&m_Console));
	m_vPanels.push_back(new DebugPanel);
}

MainWindow::~MainWindow()
{
	FreeVector(m_vEventHandlers);
	delete m_pSceneRenderer;

	ClearPointersVector(m_vPanels);

}

void MainWindow::InitBackend()
{
	// initiate SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		//	EPICFAIL("[ERROR] %s\n", SDL_GetError());
		//return -1;
	}

	std::string glsl_version = "";
	glsl_version = "#version 330";

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
	m_pSDLWindow = SDL_CreateWindow(m_strTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_iWindowWidth, m_iWindowHeight, window_flags);

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

	// Force backend to initialize
	GLBackend::Instance();

	m_Console.AddLog("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
	m_Console.AddLog("GL_VERSION: %s\n", glGetString(GL_VERSION));

}

void MainWindow::InitDocks()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	auto dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

	ImGui::DockBuilderRemoveNode(gIDMainDockspace); // clear any previous layout
	ImGui::DockBuilderAddNode(gIDMainDockspace, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
	ImGui::DockBuilderSetNodeSize(gIDMainDockspace, viewport->Size);

	
	auto idDockUp = ImGui::DockBuilderSplitNode(gIDMainDockspace, ImGuiDir_Up, 0.2f, nullptr, &gIDMainDockspace);
	ImGui::DockBuilderSplitNode(idDockUp, ImGuiDir_Right, 0.5f, &m_defaultDockSides.idDockUpLeft, &m_defaultDockSides.idDockUpRight);

	auto idDockDown = ImGui::DockBuilderSplitNode(gIDMainDockspace, ImGuiDir_Down, 0.3f, nullptr, &gIDMainDockspace);
	ImGui::DockBuilderSplitNode(idDockDown, ImGuiDir_Right, 0.5f, &m_defaultDockSides.idDockBottomLeft, &m_defaultDockSides.idDockBottomRight);

	auto idDockLeft = ImGui::DockBuilderSplitNode(gIDMainDockspace, ImGuiDir_Left, 0.2f, nullptr, &gIDMainDockspace);
	ImGui::DockBuilderSplitNode(idDockLeft, ImGuiDir_Up, 0.5f, &m_defaultDockSides.idDockLeftTop, &m_defaultDockSides.idDockLeftBottom);

	auto idDockRight = ImGui::DockBuilderSplitNode(gIDMainDockspace, ImGuiDir_Right, 0.25f, nullptr, &gIDMainDockspace);
	ImGui::DockBuilderSplitNode(idDockRight, ImGuiDir_Up, 0.5f, &m_defaultDockSides.idDockRightTop, &m_defaultDockSides.idDockRightBottom);

	//m_idDockUp = ImGui::DockBuilderSplitNode(gIDMainDockspace, ImGuiDir_Up, 0.2f, nullptr, &gIDMainDockspace);
	//m_idDockDown = ImGui::DockBuilderSplitNode(gIDMainDockspace, ImGuiDir_Down, 0.3f, nullptr, &gIDMainDockspace);
	//m_idDockLeft = ImGui::DockBuilderSplitNode(gIDMainDockspace, ImGuiDir_Left, 0.2f, nullptr, &gIDMainDockspace);
	//m_idDockRight = ImGui::DockBuilderSplitNode(gIDMainDockspace, ImGuiDir_Right, 0.25f, nullptr, &gIDMainDockspace);

	ImGui::DockBuilderFinish(gIDMainDockspace);
}

ImGuiID MainWindow::DockSpaceOverViewport(float heightAdjust, ImGuiDockNodeFlags dockspace_flags, const ImGuiWindowClass* window_class)
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImVec2 pos = ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + heightAdjust);
	ImVec2 size = ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - heightAdjust);

	ImGui::SetNextWindowPos(pos);
	ImGui::SetNextWindowSize(size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags host_window_flags = 0;
	host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
	host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		host_window_flags |= ImGuiWindowFlags_NoBackground;

	char label[32];
	sprintf_s(label, IM_ARRAYSIZE(label), "DockSpaceViewport_%08X", viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowBgAlpha(0);
	ImGui::Begin(label, NULL, host_window_flags);
	ImGui::PopStyleVar(3);

	gIDMainDockspace = ImGui::GetID(strDockspace);


	ImGui::DockSpace(gIDMainDockspace, ImVec2(0.0f, 0.0f), dockspace_flags, window_class);


	auto c = ImGui::DockBuilderGetCentralNode(gIDMainDockspace);

	if (c)
	{
		m_i3DViewport[0] = (int)c->Pos.x;
		m_i3DViewport[1] = (int)m_iWindowHeight - (int)(c->Pos.y + c->Size.y);
		m_i3DViewport[2] = (int)c->Size.x;
		m_i3DViewport[3] = (int)c->Size.y;
	}
	else
	{
		m_i3DViewport[0] = 0;
		m_i3DViewport[1] = 0;
		m_i3DViewport[2] = m_iWindowWidth;
		m_i3DViewport[3] = m_iWindowWidth;
	}


	ImGui::End();

	return gIDMainDockspace;
}

void MainWindow::UpdateDocks()
{
	ImGui::DockBuilderFinish(gIDMainDockspace);
}

int MainWindow::GetFPS()
{
	return m_TimersData.actual_fps;
}

int* MainWindow::Get3DGLViewport()
{
	return m_i3DViewport;
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
		Application::Instance()->CheckIfBakngFinished();
		m_bTerminated = !HandleEvents(!m_bTerminated);
		
		UpdateTimers();
		GL_BeginFrame();
		
		m_pSceneRenderer->RenderScene();

		RenderGUI();

		LimitToTargetFPS();
		SDL_GL_SwapWindow(m_pSDLWindow);
	}
}

int MainWindow::Width()
{
	return m_iWindowWidth;
}

int MainWindow::Height()
{
	return m_iWindowHeight;
}

SDL_Window* MainWindow::Handle()
{
	return m_pSDLWindow;
}

float MainWindow::FrameDelta()
{
	return m_TimersData.frame_delta / 1000;
}

DebugConsole* MainWindow::Console()
{
	return &m_Console;
}

SceneRenderer* MainWindow::GetSceneRenderer()
{
	return m_pSceneRenderer;
}

bool MainWindow::PropagateControlsEvent(SDL_Event& event)
{

	bool bWasHandled = false;

	for (auto& it : m_vEventHandlers)
	{
		int result = it->HandleEvent(bWasHandled, event);

		bWasHandled = bWasHandled | result & EVENT_HANDLED;

		if (result & EVENT_CONSUMED)
			break;

	}

	return bWasHandled;
}

void MainWindow::InitCommands()
{
	Application::CommandsRegistry()->RegisterCommand(new CCommand(GlobalCommands::LoadFile, "Load...", GetCommonIcon(CommonIcons::LoadFile), CMD_ON_MAINTOOLBAR, 
	[&]()
	{
			PopupsManager::Instance()->ShowPopup(PopupWindows::LoadfileDialog);
	}));

	Application::CommandsRegistry()->RegisterCommand(new CCommand(GlobalCommands::AddDirectLight, "+Direct", GetCommonIcon(CommonIcons::DirectLight), CMD_ON_MAINTOOLBAR,
		[&]()
		{
			m_pSceneRenderer->AddNewLight(LightTypes::Direct);
		}));

	Application::CommandsRegistry()->RegisterCommand(new CCommand(GlobalCommands::AddOmniLight, "+Omni", GetCommonIcon(CommonIcons::OmniLight), CMD_ON_MAINTOOLBAR,
		[&]()
		{
			m_pSceneRenderer->AddNewLight(LightTypes::Omni);
		}));

	Application::CommandsRegistry()->RegisterCommand(new CCommand(GlobalCommands::AddSpotLight, "+Spot", GetCommonIcon(CommonIcons::SpotLight), CMD_ON_MAINTOOLBAR,
		[&]()
		{
			m_pSceneRenderer->AddNewLight(LightTypes::Spot);
		}));

	Application::CommandsRegistry()->RegisterCommand(new CCommand(GlobalCommands::Bake, "Bake", GetCommonIcon(CommonIcons::Bake), CMD_ON_MAINTOOLBAR,
		[]()
		{
			//Application::Instance()->ExecuteBaking();
			PopupsManager::Instance()->ShowPopup(PopupWindows::LightBaker3KConfig);
		}));

	Application::CommandsRegistry()->RegisterCommand(new CCommand(GlobalCommands::ToggleGround, "Toggle ground", 0, 0,
		[]()
		{	
			auto pers = Application::Instance()->GetPersistentStorage();
			auto showGround = pers->GetSetting(ApplicationSettings::ShowGround);

			showGround->value.asBool = !showGround->value.asBool;

		}));


	Application::CommandsRegistry()->RegisterCommand(new CCommand(GlobalCommands::DeleteSelection, "Delete", 0, 0,
		[&]()
		{
			m_pSceneRenderer->DoDeleteSelection();
		}));

	Application::CommandsRegistry()->RegisterCommand(new CCommand(GlobalCommands::ResetLayout,"Reset layout", 0, 0,
		[&]()
		{

			InitDocks();

			for (auto& it : m_vPanels)
			{
				it->InvalidatePosition();
			}
		}));

}



float MainWindow::RenderMainMenu()
{
	float size = 0;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			Application::CommandsRegistry()->RenderCommandAsMenuItem(GlobalCommands::LoadFile);

			ImGui::Separator();

			if (ImGui::BeginMenu("Recent"))
			{
				auto& mru = Application::GetPersistentStorage()->GetMRUFiles();

				if (mru.size() == 0)
				{
					ImGui::MenuItem(" ( none ) ");
				}
				else
				{
					for (auto& it : mru)
					{
						if (ImGui::MenuItem(it.c_str()))
						{
							m_pSceneRenderer->LoadModel((char*)it.c_str());
						}
					}
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Exit"))
			{				
				m_bTerminated = true;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			Application::CommandsRegistry()->RenderCommandAsMenuItem(GlobalCommands::AddOmniLight);
			Application::CommandsRegistry()->RenderCommandAsMenuItem(GlobalCommands::AddSpotLight);
			Application::CommandsRegistry()->RenderCommandAsMenuItem(GlobalCommands::AddDirectLight);

			ImGui::Separator();
			Application::CommandsRegistry()->RenderCommandAsMenuItem(GlobalCommands::DeleteSelection);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			Application::CommandsRegistry()->RenderCommandAsMenuItem(GlobalCommands::ToggleGround);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			Application::CommandsRegistry()->RenderCommandAsMenuItem(GlobalCommands::ResetLayout);


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

float MainWindow::RenderMainToolbar(float menuHeight)
{
	ImGui::SetNextWindowPos(ImVec2(0, menuHeight));
	ImGui::SetNextWindowSize(ImVec2((float)m_iWindowWidth, 0));

	ImGui::Begin("##MainÅoolbar", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	auto toolbarCommands = Application::CommandsRegistry()->GetMainToolbarCommands();

	for (auto& it : toolbarCommands)
	{
		auto cmd = Application::CommandsRegistry()->GetCommandByInternalIndex(it);
		cmd->RenderImGUI(40);
	}

	float r = ImGui::GetWindowHeight();

	ImGui::End();

	return r;
}

void MainWindow::RenderGUI()
{
	glViewport(0, 0, m_iWindowWidth, m_iWindowHeight);

	// start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_pSDLWindow);

	static int delayInit = 2;

	ImGui::NewFrame();

	if (delayInit > 0)
	{
		delayInit--;

		auto pers = Application::GetPersistentStorage();
		if (delayInit == 0)
		{
			if (pers->IsFreshFile()) InitDocks();
			Application::Instance()->FlagDelayedInitDone();
		}
	}
	
	auto it = ImGui::FindOrCreateWindowSettings("Scene objects");


	float		menuHeight = RenderMainMenu();
	float		toolbarHeight = RenderMainToolbar(menuHeight);
	
	DockSpaceOverViewport(toolbarHeight, ImGuiDockNodeFlags_PassthruCentralNode, 0);
	
	ObjectPropertiesEditor::Instance()->RenderGuizmo();
	

	for (auto& it : m_vPanels)
		it->InvokeRender();
		
	PopupsManager::Instance()->RenderPopups();
	
#ifdef DEBUG
	ImGui::ShowDemoWindow();
#endif


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool MainWindow::RenderToolbarIcon(GLuint iconId)
{
	ImGui::SameLine();
	return ImGui::ImageButton((ImTextureID)iconId, ImVec2(16, 16));
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
				SelectionManager::Instance()->UnSelect();
				break;
			}
			// TODO: implement
			if (PropagateControlsEvent(event)) break;
			break;

		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:

			if (event.button.button & SDL_BUTTON_LEFT)
			{
				if (!ImGui::GetHoveredID())
				{
					if (SelectionManager::Instance()->SelectHoveredObject())
					{
						break;
					}
				}
			}
			

		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEWHEEL:
		case SDL_KEYUP:
			// TODO: implement

			

			PropagateControlsEvent(event);
			break;

		case (SDL_DROPFILE):
		{
			char* dropped_filedir;

			// In case if dropped file
			dropped_filedir = event.drop.file;
			// Shows directory of dropped file

			m_pSceneRenderer->LoadModel(dropped_filedir);

			SDL_free(dropped_filedir);    // Free dropped_filedir memory
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

	glViewport(m_i3DViewport[0], 
			   m_i3DViewport[1], 
			   m_i3DViewport[2], 
			   m_i3DViewport[3]);
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
