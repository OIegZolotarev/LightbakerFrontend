/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/

#include "main_window.h"
#include "Camera.h"
#include "application.h"
#include "common_resources.h"
#include "console_output_panel.h"
#include "debug_panel.h"
#include "imgui_internal.h"
#include "imgui_popups.h"
#include "loader_thread.h"
#include "properties_editor.h"
#include "scene_objects_panel.h"
#include "ui_common.h"
#include "ui_styles_manager.h"

#include "goldsource_bsp_disk_structs.h"
#include "grid_renderer.h"
#include "hammer_fgd.h"
#include "imgui_helpers.h"
#include "wad_textures.h"
#include "popup_loadfile_dialog.h"

bool DEBUG_3D_SELECTION = false;

const char *strDockspace = "DockSpace";
ImGuiID gIDMainDockspace = 0;

SDL_Cursor *g_EmptyCursor;
char g_IMGuiIniPath[1024];

MainWindow::MainWindow(const char *title, glm::vec2 defaultSize)
    : m_iWindowWidth(defaultSize.x), m_iWindowHeight(defaultSize.y)
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

    m_pBackgroudColorSetting1 = Application::GetPersistentStorage()->GetSetting(ApplicationSettings::BackgroundColor1);
    m_pBackgroudColorSetting2 = Application::GetPersistentStorage()->GetSetting(ApplicationSettings::BackgroundColor2);
    m_pUseGradientBackground =
        Application::GetPersistentStorage()->GetSetting(ApplicationSettings::UseGradientBackground);
}

MainWindow::~MainWindow()
{
    FreeGLTextures();
    FreeVector(m_vEventHandlers);
    delete m_pSceneRenderer;

    ClearPointersVector(m_vPanels);

    ImGui::DestroyContext(m_pImGUIContext);

    delete PopupsManager::Instance();
    delete GoldSource::WADPool::Instance();

    SDL_DestroyWindow(m_pSDLWindow);
    SDL_GL_DeleteContext(m_pGLContext);
    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_Quit();
}

void MainWindow::InitBackend()
{
    // initiate SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        //	EPICFAIL("[ERROR] %s\n", SDL_GetError());
        // return -1;
    }

    std::string glsl_version = "";
    glsl_version             = "#version 330";

#ifdef NICE_LINES
    int r = 2;
    r     = SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    r     = SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
#endif

    SDL_WindowFlags window_flags =
        (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MAXIMIZED);
    m_pSDLWindow = SDL_CreateWindow(m_strTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_iWindowWidth,
                                    m_iWindowHeight, window_flags);

    // limit to which minimum size user can resize the window
    SDL_SetWindowMinimumSize(m_pSDLWindow, 500, 300);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

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
    ImGuiIO &io     = ImGui::GetIO();
    (void)io;

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NoMouseCursorChange;
    io.MouseDrawCursor = true;

    // char *p = SDL_GetPrefPath("QuiteOldOrange", "LightBaker3000Frontend");
    char *p = SDL_GetPrefPath(SDL_ORGANIZATION, SDL_APP_NAME);
    sprintf_s(g_IMGuiIniPath, sizeof(g_IMGuiIniPath), "%s/imgui.ini", p);

    io.IniFilename = g_IMGuiIniPath;

    // setup Dear ImGui style
    // ImGui::StyleColorsClassic();

    UIStyles::Manager::Instance()->ApplyCurrentStyle();

    ImGuiStyle *style = &ImGui::GetStyle();

    style->FrameBorderSize = 1;
    style->TabBorderSize   = 1;
    style->WindowRounding  = 4;

    // setup platform/renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(m_pSDLWindow, m_pGLContext);
    ImGui_ImplOpenGL3_Init(glsl_version.c_str());

    ImGuiHelpers::Init();

    int32_t cursorData[2] = {0, 0};
    g_EmptyCursor         = SDL_CreateCursor((Uint8 *)cursorData, (Uint8 *)cursorData, 8, 8, 4, 4);
    SDL_SetCursor(g_EmptyCursor);

    // Force backend to initialize
    GLBackend::Instance();

    m_Console.AddLog("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
    m_Console.AddLog("GL_VERSION: %s\n", glGetString(GL_VERSION));
}

void MainWindow::InitDocks()
{
    ImGuiViewport *viewport = ImGui::GetMainViewport();

    auto dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

    ImGui::DockBuilderRemoveNode(gIDMainDockspace); // clear any previous layout
    ImGui::DockBuilderAddNode(gIDMainDockspace, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(gIDMainDockspace, viewport->Size);

    auto idDockUp = ImGui::DockBuilderSplitNode(gIDMainDockspace, ImGuiDir_Up, 0.2f, nullptr, &gIDMainDockspace);
    ImGui::DockBuilderSplitNode(idDockUp, ImGuiDir_Right, 0.5f, &m_defaultDockSides.idDockUpLeft,
                                &m_defaultDockSides.idDockUpRight);

    auto idDockDown = ImGui::DockBuilderSplitNode(gIDMainDockspace, ImGuiDir_Down, 0.3f, nullptr, &gIDMainDockspace);
    ImGui::DockBuilderSplitNode(idDockDown, ImGuiDir_Right, 0.5f, &m_defaultDockSides.idDockBottomLeft,
                                &m_defaultDockSides.idDockBottomRight);

    auto idDockLeft = ImGui::DockBuilderSplitNode(gIDMainDockspace, ImGuiDir_Left, 0.2f, nullptr, &gIDMainDockspace);
    ImGui::DockBuilderSplitNode(idDockLeft, ImGuiDir_Up, 0.5f, &m_defaultDockSides.idDockLeftTop,
                                &m_defaultDockSides.idDockLeftBottom);

    auto idDockRight = ImGui::DockBuilderSplitNode(gIDMainDockspace, ImGuiDir_Right, 0.25f, nullptr, &gIDMainDockspace);
    ImGui::DockBuilderSplitNode(idDockRight, ImGuiDir_Up, 0.5f, &m_defaultDockSides.idDockRightTop,
                                &m_defaultDockSides.idDockRightBottom);

    // m_idDockUp = ImGui::DockBuilderSplitNode(gIDMainDockspace, ImGuiDir_Up, 0.2f, nullptr, &gIDMainDockspace);
    // m_idDockDown = ImGui::DockBuilderSplitNode(gIDMainDockspace, ImGuiDir_Down, 0.3f, nullptr, &gIDMainDockspace);
    // m_idDockLeft = ImGui::DockBuilderSplitNode(gIDMainDockspace, ImGuiDir_Left, 0.2f, nullptr, &gIDMainDockspace);
    // m_idDockRight = ImGui::DockBuilderSplitNode(gIDMainDockspace, ImGuiDir_Right, 0.25f, nullptr, &gIDMainDockspace);

    ImGui::DockBuilderFinish(gIDMainDockspace);
}

ImGuiID MainWindow::DockSpaceOverViewport(float heightAdjust, ImGuiDockNodeFlags dockspace_flags,
                                          const ImGuiWindowClass *window_class)
{
    ImGuiViewport *viewport = ImGui::GetMainViewport();

    ImVec2 pos  = ImVec2(viewport->WorkPos.x, viewport->WorkPos.y + heightAdjust);
    ImVec2 size = ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - heightAdjust);

    ImGui::SetNextWindowPos(pos);
    ImGui::SetNextWindowSize(size);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags host_window_flags = 0;
    host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
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

    // ImGui::ShowStyleSelector("Select style");

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

void MainWindow::SetTitle(std::string &fileName)
{
    m_strTitle = std::format("LightBaker3000 FrontEnd, build #{1} - {0}", fileName, Application::Q_buildnum());
    SDL_SetWindowTitle(m_pSDLWindow, m_strTitle.c_str());
}

void MainWindow::UpdateDocks()
{
    ImGui::DockBuilderFinish(gIDMainDockspace);
}

defaultDockSides_s *MainWindow::GetDockSides()
{
    return &m_defaultDockSides;
}

int MainWindow::GetFPS()
{
    return m_TimersData.actual_fps;
}

int *MainWindow::Get3DGLViewport()
{
    return m_i3DViewport;
}

void MainWindow::MainLoop()
{
    m_TimersData.timestamp_now          = SDL_GetPerformanceCounter();
    m_TimersData.timestamp_last         = SDL_GetPerformanceCounter();
    m_TimersData.frames_until_init      = 3;
    m_TimersData.fps_accum              = 0;
    m_TimersData.num_frames_this_second = 0;

    while (!m_bTerminated)
    {
        GLBackend::Instance()->NewFrame();
        Application::Instance()->CheckIfBakngFinished();
        m_bTerminated = !HandleEvents(!m_bTerminated);

        UpdateTimers();
        GL_BeginFrame();

// Multiple viewports        
//         glEnable(GL_SCISSOR_TEST);
//         glScissor(m_i3DViewport[0], m_i3DViewport[1], m_i3DViewport[2] / 2, m_i3DViewport[3] / 2);
//         glViewport(m_i3DViewport[0], m_i3DViewport[1], m_i3DViewport[2] / 2, m_i3DViewport[3] / 2);
//         m_pSceneRenderer->RenderScene();
// 
//         
//         glScissor(m_i3DViewport[0] + m_i3DViewport[2] / 2, m_i3DViewport[1], m_i3DViewport[2] / 2, m_i3DViewport[3] / 2);
//         glViewport(m_i3DViewport[0] + m_i3DViewport[2] / 2, m_i3DViewport[1], m_i3DViewport[2] / 2, m_i3DViewport[3] / 2);
//         m_pSceneRenderer->RenderScene();
// 
//         glDisable(GL_SCISSOR_TEST);

        m_pSceneRenderer->RenderScene();

        LoaderThread::Instance()->ExecuteEndCallbacks(10);

        RenderGUI();

        // LimitToTargetFPS();
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

SDL_Window *MainWindow::Handle()
{
    return m_pSDLWindow;
}

float MainWindow::FrameDelta()
{
    return m_TimersData.frame_delta / 1000;
}

DebugConsole *MainWindow::Console()
{
    return &m_Console;
}

SceneRenderer *MainWindow::GetSceneRenderer()
{
    return m_pSceneRenderer;
}

bool MainWindow::PropagateControlsEvent(SDL_Event &event)
{
    bool bWasHandled = false;

    for (auto &it : m_vEventHandlers)
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
    auto callbackLoadModel = [](std::string &fileName) {
        Application::GetMainWindow()->GetSceneRenderer()->LoadModel(fileName.c_str(), true);
    };

    Application::CommandsRegistry()->RegisterCommand(
        new CCommand(GlobalCommands::LoadFile, "Load...", 0, GetCommonIcon(CommonTextures::LoadFile), CMD_ON_MAINTOOLBAR,
            [&]() 
            { 
                auto lfd = LoadFileDialog::Instance();

                lfd->SetTitle("Load map\\scene");
                lfd->SetFilters(".obj,.bsp");
                lfd->SetOnSelectCallback(callbackLoadModel);

                PopupsManager::Instance()->ShowPopup(PopupWindows::LoadfileDialog);             
            }));

    Application::CommandsRegistry()->RegisterCommand(
        new CCommand(GlobalCommands::AddDirectLight, "+Direct", 0, GetCommonIcon(CommonTextures::DirectLight),
                     CMD_ON_MAINTOOLBAR, [&]() {
                         glm::vec3 newLightPos = m_pSceneRenderer->GetNewLightPos();
                         m_pSceneRenderer->GetScene()->AddNewLight(newLightPos, LightTypes::Direct);
                     }));

    Application::CommandsRegistry()->RegisterCommand(new CCommand(
        GlobalCommands::AddOmniLight, "+Omni", 0, GetCommonIcon(CommonTextures::OmniLight), CMD_ON_MAINTOOLBAR, [&]() {
            glm::vec3 newLightPos = m_pSceneRenderer->GetNewLightPos();
            m_pSceneRenderer->GetScene()->AddNewLight(newLightPos, LightTypes::Omni);
        }));

    Application::CommandsRegistry()->RegisterCommand(new CCommand(
        GlobalCommands::AddSpotLight, "+Spot", 0, GetCommonIcon(CommonTextures::SpotLight), CMD_ON_MAINTOOLBAR, [&]() {
            glm::vec3 newLightPos = m_pSceneRenderer->GetNewLightPos();
            m_pSceneRenderer->GetScene()->AddNewLight(newLightPos, LightTypes::Spot);
        }));

    Application::CommandsRegistry()->RegisterCommand(
        new CCommand(GlobalCommands::Bake, "Bake", 0, GetCommonIcon(CommonTextures::Bake), CMD_ON_MAINTOOLBAR, []() {
            // Application::Instance()->ExecuteBaking();
            PopupsManager::Instance()->ShowPopup(PopupWindows::LightBaker3KConfig);
        }));

    Application::CommandsRegistry()->RegisterCommand(
        new CCommand(GlobalCommands::ToggleGround, "Toggle ground", 0, 0, 0, []() {
            auto pers       = Application::Instance()->GetPersistentStorage();
            auto showGround = pers->GetSetting(ApplicationSettings::ShowGround);

            showGround->SetBool(!showGround->GetAsBool());
        }));

    Application::CommandsRegistry()->RegisterCommand(
        new CCommand(GlobalCommands::DeleteSelection, "Delete", "Delete", 0, 0,
                     [&]() { m_pSceneRenderer->GetScene()->DoDeleteSelection(); }));

    Application::CommandsRegistry()->RegisterCommand(
        new CCommand(GlobalCommands::ResetLayout, "Reset layout", 0, 0, 0, [&]() {
            InitDocks();

            for (auto &it : m_vPanels)
            {
                it->InvalidatePosition();
            }
        }));

    Application::CommandsRegistry()->RegisterCommand(
        new CCommand(GlobalCommands::Undo, "Undo", "Left Ctrl-Z", 0, 0,
                     [&]() { GetSceneRenderer()->GetScene()->GetEditHistory()->Undo(); }));

    Application::CommandsRegistry()->RegisterCommand(
        new CCommand(GlobalCommands::Redo, "Redo", "Left Ctrl-Y", 0, 0,
                     [&]() { GetSceneRenderer()->GetScene()->GetEditHistory()->Redo(); }));

    Application::CommandsRegistry()->RegisterCommand(
        new CCommand(GlobalCommands::SceneScale, "Scene scale", "", 0, 0,
                     [&]() { PopupsManager::Instance()->ShowPopup(PopupWindows::SceneScaleDialog); }));

    Application::CommandsRegistry()->RegisterCommand(new CCommand(GlobalCommands::DebugSelection, "Debug 3D selection",
                                                                  "", 0, 0,
                                                                  [&]() { DEBUG_3D_SELECTION = !DEBUG_3D_SELECTION; }));

    Application::CommandsRegistry()->RegisterCommand(
        new CCommand(GlobalCommands::OpenProgramOptions, "Preferences...", "", 0, 0,
                     [&]() { PopupsManager::Instance()->ShowPopup(PopupWindows::ProgramOptions); }));
}

float MainWindow::RenderMainMenu()
{
    auto registry = Application::CommandsRegistry();
#define COMMAND_ITEM registry->RenderCommandAsMenuItem

    float size = 0;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            COMMAND_ITEM(GlobalCommands::LoadFile);

            ImGui::Separator();

            if (ImGui::BeginMenu("Recent"))
            {
                auto &mru = Application::GetPersistentStorage()->GetMRUFiles();

                if (mru.size() == 0)
                {
                    ImGui::MenuItem(" ( none ) ");
                }
                else
                {
                    int index = 0;

                    for (auto &it : mru)
                    {
                        auto file = FileSystem::Instance()->ExtractFileName(it.first.c_str());
                        auto path = FileSystem::Instance()->ExtractFilePath(it.first.c_str());

                        file = std::format("{0}. {1}", ++index, file);

                        if (ImGui::MenuItem(file.c_str(), path.c_str()))
                        {
                            m_pSceneRenderer->LoadModel((char *)it.first.c_str(), LRF_LOAD_ALL);
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
            COMMAND_ITEM(GlobalCommands::SceneScale);
            ImGui::Separator();

            COMMAND_ITEM(GlobalCommands::Undo);
            COMMAND_ITEM(GlobalCommands::Redo);

            ImGui::Separator();
            COMMAND_ITEM(GlobalCommands::AddOmniLight);
            COMMAND_ITEM(GlobalCommands::AddSpotLight);
            COMMAND_ITEM(GlobalCommands::AddDirectLight);

            ImGui::Separator();
            COMMAND_ITEM(GlobalCommands::DeleteSelection);

            ImGui::Separator();
            COMMAND_ITEM(GlobalCommands::DecreaseGridStep);
            COMMAND_ITEM(GlobalCommands::IncreaseGridStep);

            ImGui::Separator();
            COMMAND_ITEM(GlobalCommands::DumpLightmapMesh);
            COMMAND_ITEM(GlobalCommands::DumpLightmapUV);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
#ifdef _DEBUG
            COMMAND_ITEM(GlobalCommands::DebugSelection);
#endif
            COMMAND_ITEM(GlobalCommands::ToggleGround);

            if (ImGui::BeginMenu("Render mode"))
            {
                RenderMode renderMode = GetSceneRenderer()->GetRenderMode();

                std::tuple<GlobalCommands, RenderMode> modes[] = {
                    {GlobalCommands::LightshadedRenderMode, RenderMode::Lightshaded},
                    {GlobalCommands::UnshadedRenderMode, RenderMode::Unshaded},
                    {GlobalCommands::GroupShadedRenderMode, RenderMode::Groups},
                    {GlobalCommands::WireframeLightshadedRenderMode, RenderMode::WireframeShaded},
                    {GlobalCommands::WireframeUnshadedRenderMode, RenderMode::WireframeUnshaded}};

                for (int i = 0; i < ARRAYSIZE(modes); i++)
                {
                    COMMAND_ITEM(std::get<0>(modes[i]), renderMode == std::get<1>(modes[i]));
                }

                ImGui::EndMenu();
            }

            COMMAND_ITEM(GlobalCommands::OpenProgramOptions);

            COMMAND_ITEM(GlobalCommands::ReloadAllShaders);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Windows"))
        {
            COMMAND_ITEM(GlobalCommands::ResetLayout);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("About..."))
            {
                // m_pPopupsManager->ShowPopup(PopupWindows::AboutApplicationDialog);
            }

            // 			if (ImGui::MenuItem("Style editor..."))
            // 			{
            // 				//m_pPopupsManager->ShowPopup(PopupWindows::StyleEditor);
            // 			}
            //
            // 			if (ImGui::MenuItem("Style selector..."))
            // 			{
            // 				//m_pPopupsManager->ShowPopup(PopupWindows::StyleChooser);
            // 			}

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

    for (auto &it : toolbarCommands)
    {
        auto cmd = Application::CommandsRegistry()->GetCommandByInternalIndex(it);
        cmd->RenderImGUI(23);
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
            if (pers->IsFreshFile())
                InitDocks();
            Application::Instance()->FlagDelayedInitDone();
        }
    }

    float menuHeight    = RenderMainMenu();
    float toolbarHeight = RenderMainToolbar(menuHeight);

    DockSpaceOverViewport(toolbarHeight, ImGuiDockNodeFlags_PassthruCentralNode, 0);

    ObjectPropertiesEditor::Instance()->RenderGuizmo();

    for (auto &it : m_vPanels)
        it->InvokeRender();

    PopupsManager::Instance()->RenderPopups();

    if (Application::IsWaitingForBakerToFinish())
    {
        float yBannerOffset = toolbarHeight + menuHeight + 8;
        DrawBakingInProgressBanner(yBannerOffset);
    }

    DrawLoadingBanner();

    auto viewport = ImGui::FindViewportByID(m_defaultDockSides.idDockLeftBottom);

    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;

    if (ImGui::BeginViewportSideBar("MENU", viewport, ImGuiDir_Down, 20, window_flags))
    {
        if (ImGui::BeginMenuBar())
        {
            auto camera = GetSceneRenderer()->GetCamera();
            std::string tip;
            camera->FormatControlsTip(tip);
            ImGui::Text(tip.c_str());
            ImGui::EndMenuBar();
        }

        ImGui::End();
    }

#ifdef _DEBUG
    ImGui::ShowDemoWindow();
#endif

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MainWindow::DrawLoadingBanner()
{
    taskprogressinfo_t info;
    LoaderThread::Instance()->FillTaskProgressInfo(&info);

    if (!info.hasBackgroundTasks)
        return;

    static ImVec2 size = {0, 0};

    ImGui::SetNextWindowPos(ImVec2(m_iWindowWidth / 2 - size.x / 2, m_iWindowHeight / 2 - size.y / 2));

    auto txtColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);

    float timestampSeconds = m_TimersData.timestamp_now / 1000000.f;

    float bias = (sin(timestampSeconds / 2) + 1) / 2;

    float rangeMin = 0.75f;
    float rangeMax = 1.f;

    float shade = rangeMin + (rangeMax - rangeMin) * bias;

    txtColor.x *= shade;
    txtColor.y *= shade;
    txtColor.z *= shade;

    ImGui::Begin("##LoadingBanner", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

    ImGui::PushStyleColor(ImGuiCol_Text, txtColor);
    ImGui::Text(info.operationDescription.c_str());
    ImGui::PopStyleColor();

    float progress = (float)info.currentTaskPerformedSteps / (float)info.currentTaskTotalSteps;

    ImGui::Text(info.elementDescription.c_str());
    ImGui::ProgressBar(progress);

    ImGui::SetWindowSize({m_iWindowWidth / 10.f * 7, 100});
    size = ImGui::GetWindowSize();

    ImGui::End();
}

void MainWindow::DrawBakingInProgressBanner(float yBannerOffset)
{
    ImGui::SetNextWindowPos(ImVec2(10, yBannerOffset));

    auto txtColor = ImGui::GetStyleColorVec4(ImGuiCol_Text);

    float timestampSeconds = m_TimersData.timestamp_now / 1000000.f;

    float bias = (sin(timestampSeconds / 2) + 1) / 2;

    float rangeMin = 0.75f;
    float rangeMax = 1.f;

    float shade = rangeMin + (rangeMax - rangeMin) * bias;

    txtColor.x *= shade;
    txtColor.y *= shade;
    txtColor.z *= shade;

    float progress = Application::Instance()->GetBakingProgress();

    ImGui::PushStyleColor(ImGuiCol_Text, txtColor);

    ImGui::Begin("##BakingInProgressBanner", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Baking in progress: %.02f%%", progress);
    ImGui::End();

    ImGui::PopStyleColor();
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
            if (ImGui::GetIO().WantCaptureKeyboard && bIgnorableEvent)
                continue;
            break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            if (ImGui::GetIO().WantCaptureMouse && bIgnorableEvent)
                continue;
            break;
        }

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

            if (Application::CommandsRegistry()->OnKeyDown())
                break;

            if (PropagateControlsEvent(event))
                break;
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

        case (SDL_DROPFILE): {
            char *dropped_filedir;

            // In case if dropped file
            dropped_filedir = event.drop.file;
            // Shows directory of dropped file

            m_pSceneRenderer->LoadModel(dropped_filedir, LRF_LOAD_ALL);

            SDL_free(dropped_filedir); // Free dropped_filedir memory
        }
        }
    }

    return loop;
}

void MainWindow::GL_BeginFrame()
{
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);

    glClearDepth(1);

    // 	if (settingMultisample->GetAsBool())
    // 		glEnable(GL_MULTISAMPLE);
    // 	else
    //
    glDisable(GL_MULTISAMPLE);

    // glClearColor(0.25, .25, .25, 1);

    // ClearBackground();

    glClear(GL_DEPTH_BUFFER_BIT);

    glViewport(m_i3DViewport[0], m_i3DViewport[1], m_i3DViewport[2], m_i3DViewport[3]);
}

void MainWindow::ClearBackground()
{
    auto col1 = m_pBackgroudColorSetting1->GetColorRGB();
    auto col2 = m_pBackgroudColorSetting2->GetColorRGB();

    glClearColor(col1[0], col1[1], col1[2], 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (m_pUseGradientBackground->GetAsBool())
    {
        //          glMatrixMode(GL_PROJECTION);
        //          glLoadIdentity();
        //
        //          glMatrixMode(GL_MODELVIEW);
        //          glLoadIdentity();
        //
        //          glDisable(GL_DEPTH_TEST);
        //          glDisable(GL_TEXTURE_2D);
        //          glDisable(GL_CULL_FACE);
        //
        //          glBegin(GL_QUADS);
        //
        //  #define ONE 1
        //
        //          glColor3fv((float *)&col2);
        //          glVertex2f(-ONE, -ONE);
        //          glVertex2f(ONE, -ONE);
        //
        //          glColor3fv((float *)&col1);
        //
        //          glVertex2f(ONE, ONE);
        //          glVertex2f(-ONE, ONE);
        //
        //          glEnd();
        //
        //          glEnable(GL_CULL_FACE);
        //          glEnable(GL_TEXTURE_2D);
        //          glEnable(GL_DEPTH_TEST);

        m_pSceneRenderer->GetCamera()->Apply();
    }
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
    m_TimersData.timestamp_now  = SDL_GetPerformanceCounter();

    m_TimersData.frame_delta = (double)((m_TimersData.timestamp_now - m_TimersData.timestamp_last) * 1000 /
                                        (double)SDL_GetPerformanceFrequency());
    m_TimersData.fps_accum += m_TimersData.frame_delta;

    if (m_TimersData.fps_accum > 1000)
    {
        m_TimersData.actual_fps             = m_TimersData.num_frames_this_second;
        m_TimersData.num_frames_this_second = 0;
        m_TimersData.fps_accum              = 0;
    }

    m_TimersData.num_frames_this_second++;
}

void MainWindow::LimitToTargetFPS()
{
#ifndef LINUX
    int target_fps            = 60;
    static uint64_t next_tick = 0;
    uint64_t this_tick        = SDL_GetTicks64();
    if (this_tick < next_tick)
    {
        SDL_Delay((uint32_t)(next_tick - this_tick));
    }
    // this_tick = SDL_GetTicks64(); // WTF?
    next_tick = this_tick + (1000 / target_fps);
#else
    int target_fps            = m_pSettings->Get(IDS_FPS_LIMIT)->GetAsInt();
    static uint32_t next_tick = 0;
    uint32_t this_tick        = SDL_GetTicks();
    if (this_tick < next_tick)
    {
        SDL_Delay((uint32_t)(next_tick - this_tick));
    }
    // this_tick = SDL_GetTicks64(); // WTF?
    next_tick = this_tick + (1000 / target_fps);
#endif
}
