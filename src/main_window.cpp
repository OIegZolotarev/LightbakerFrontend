/*
    LightBaker3000 Frontend project,
    (c) 2022-2023 CrazyRussian
*/

#include "main_window.h"
#include "application.h"

#include "common_resources.h"
#include "console_output_panel.h"
#include "debug_panel.h"
#include "imgui_helpers.h"
#include "imgui_internal.h"
#include "imgui_popups.h"
#include "loader_thread.h"
#include "properties_editor.h"
#include "scene_objects_panel.h"
#include "ui_common.h"
#include "ui_styles_manager.h"

#include "hammer_fgd.h"
#include "r_editor_grid.h"

#include "gl_screenspace_2d_renderer.h"
#include "popup_loadfile_dialog.h"
#include "scene_renderer.h"
#include "secondary_window.h"
#include "viewport.h"
#include "viewports_orchestrator.h"
#include "wad_textures.h"
#include <list>

bool DEBUG_3D_SELECTION = false;

const char *strDockspace     = "DockSpace";
ImGuiID     gIDMainDockspace = 0;

SDL_Cursor *g_EmptyCursor;
char        g_IMGuiIniPath[1024];

MainWindow::MainWindow(const char *title, glm::vec2 defaultSize)

{
    m_iWindowWidth = (defaultSize.x);

    m_iWindowHeight = (defaultSize.y);

    m_strTitle = title;

    // InitStuff();

    m_vPanels.push_back(ObjectPropertiesEditor::Instance());
    m_vPanels.push_back(new SceneObjectPanel);
    m_vPanels.push_back(new ConsoleOutputPanel(&m_Console));
    m_vPanels.push_back(new DebugPanel);

    m_pBackgroudColorSetting1 = Application::GetPersistentStorage()->GetSetting(ApplicationSettings::BackgroundColor1);
    m_pBackgroudColorSetting2 = Application::GetPersistentStorage()->GetSetting(ApplicationSettings::BackgroundColor2);
    m_pUseGradientBackground =
        Application::GetPersistentStorage()->GetSetting(ApplicationSettings::UseGradientBackground);

    // SecondaryWindow *pWindow = new SecondaryWindow("Test-test");
}

void MainWindow::InitStuff()
{
    InitBackend();

    m_pSceneRenderer = new SceneRenderer(this);
    AddEventHandler(m_pSceneRenderer);

    InitCommonResources();
    InitCommands();
    InitTimers();

    GL_CheckForErrors();
}

MainWindow::~MainWindow()
{
    delete m_pBackgroundMesh;
    delete m_pBackgroundShader;

    delete TextureManager::Instance();

    // FreeGLTextures();

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

void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                     const GLchar *message, const void *userParam)
{
    // Convert enums into a humen readable text
    // See: https://www.opengl.org/registry/specs/ARB/debug_output.txt

    const char *sourceText = "Unknown";
    switch (source)
    {
    case GL_DEBUG_SOURCE_API_ARB:
        // The GL
        sourceText = "API";
        break;
        // The GLSL shader compiler or compilers for other extension - provided languages
    case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
        sourceText = "Shader compiler";
        break;
        // The window system, such as WGL or GLX
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
        sourceText = "Window system";
        break;
        // External debuggers or third-party middleware libraries
    case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
        sourceText = "Third party";
        break;
        // The application
    case GL_DEBUG_SOURCE_APPLICATION_ARB:
        sourceText = "Application";
        break;
        // Sources that do not fit to any of the ones listed above
    case GL_DEBUG_SOURCE_OTHER_ARB:
        sourceText = "Other";
        break;
    }

    const char *typeText = "Unknown";
    switch (type)
    {
        // Events that generated an error
    case GL_DEBUG_TYPE_ERROR_ARB:
        typeText = "Error";
        break;
        // Behavior that has been marked for deprecation
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
        typeText = "Deprecated behavior";
        break;
        // Behavior that is undefined according to the specification
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
        typeText = "Undefined behavior";
        break;
        // Implementation-dependent performance warnings
    case GL_DEBUG_TYPE_PERFORMANCE_ARB:
        typeText = "Performance";
        break;
        // Use of extensions or shaders in a way that is highly vendor - specific
    case GL_DEBUG_TYPE_PORTABILITY_ARB:
        typeText = "Portability";
        break;
        // Types of events that do not fit any of the ones listed above
    case GL_DEBUG_TYPE_OTHER_ARB:
        typeText = "Other";
        break;
    }

    const char *severityText = "Unknown";
    switch (severity)
    {
        // Any GL error; dangerous undefined behavior; any GLSL or ARB shader compiler and linker errors;
    case GL_DEBUG_SEVERITY_HIGH_ARB:
        severityText = "High";
        break;
        // Severe performance warnings; GLSL or other shader compiler and linker warnings; use of currently deprecated
        // behavior
    case GL_DEBUG_SEVERITY_MEDIUM_ARB:
        severityText = "Medium";
        break;
        // Performance warnings from redundant state changes; trivial undefined behavior
    case GL_DEBUG_SEVERITY_LOW_ARB:
        severityText = "Low";
        break;
    }

    if (severity == 33387)
        return;

    // Unused params
    (void)id;
    (void)length;
    (void)userParam;

    // Replace LogDebug with your logging function
    Con_Printf("[OpenGL:source='%s', type='%s', severity='%s'] %s\n", sourceText, typeText,
             severityText, message);

}

void MainWindow::InitBackend()
{
    // initiate SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        //	EPICFAIL("[ERROR] %s\n", SDL_GetError());
        // return -1;
    }

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
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

#ifdef GL_DEBUG
    // Debug context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

    m_pGLContext = SDL_GL_CreateContext(m_pSDLWindow);
    SDL_GL_MakeCurrent(m_pSDLWindow, m_pGLContext);

    // enable VSync
    SDL_GL_SetSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        printf("[ERROR] Couldn't initialize glad\n");
    }
    else
        printf("[INFO] GLAD Initialized\n");

#ifdef GL_DEBUG

    #define GL_DEBUG_OUTPUT 0x92E0

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallbackARB(DebugMessageCallback, 0);
#endif

    glViewport(0, 0, m_iWindowWidth, m_iWindowHeight);

    // setup Dear ImGui context
    IMGUI_CHECKVERSION();
    m_pImGUIContext = ImGui::CreateContext();
    ImGuiIO &io     = ImGui::GetIO();
    (void)io;

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NoMouseCursorChange;
    io.MouseDrawCursor = true;

    // ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

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
    InitImGUISDL2Platform();

    ImGui_ImplOpenGL3_Init("#version 330");

    ImGuiHelpers::Init();

    int32_t cursorData[2] = {0, 0};
    // g_EmptyCursor         = SDL_CreateCursor((Uint8 *)cursorData, (Uint8 *)cursorData, 8, 8, 4, 4);
    // SDL_SetCursor(g_EmptyCursor);

    // Force backend to initialize
    GLBackend::Instance();

    m_Console.AddLog("GL_RENDERER: %s\n", glGetString(GL_RENDERER));
    m_Console.AddLog("GL_VERSION: %s\n", glGetString(GL_VERSION));

    InitBackgroundRenderer();
    TextureManager::Instance()->OnGLInit();

    InitViewports();
    GL_CheckForErrors();
}

void MainWindow::InitImGUISDL2Platform()
{
    ImGui_ImplSDL2_InitForOpenGL(m_pSDLWindow, m_pGLContext);
}

int MainWindow::GetState()
{
    return m_windowState;
}

bool MainWindow::HandleEvent(SDL_Event &event)
{
    return CommonHandleEvent(event);
}

void MainWindow::InitBackgroundRenderer()
{
    m_pBackgroundShader = GLBackend::Instance()->QueryShader("res/glprogs/background.glsl", {});

    // TODO: try some more intersting ways described here:
    // https://stackoverflow.com/questions/2588875/whats-the-best-way-to-draw-a-fullscreen-quad-in-opengl-3-2

    m_pBackgroundMesh = new DrawMesh();

    m_pBackgroundMesh->Begin(GL_TRIANGLE_STRIP);
    m_pBackgroundMesh->Color3f(1, 0, 0);
    m_pBackgroundMesh->Vertex2f(-1, -1);

    m_pBackgroundMesh->Color3f(1, 0, 0);
    m_pBackgroundMesh->Vertex2f(+1, -1);

    m_pBackgroundMesh->Color3f(0, 0, 0);
    m_pBackgroundMesh->Vertex2f(-1, +1);

    m_pBackgroundMesh->Color3f(0, 0, 0);
    m_pBackgroundMesh->Vertex2f(+1, +1);

    m_pBackgroundMesh->End();
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

    int oldViewport[4];
    for (int i = 0; i < 4; i++)
        oldViewport[i] = m_i3DViewport[i];

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

    for (int i = 0; i < 4; i++)
    {
        if (m_i3DViewport[i] != oldViewport[i])
        {
            ViewportsOrchestrator::Instance()->FlagRepaintAll();
            break;
        }
    }

    ImGui::End();

    return gIDMainDockspace;
}

bool MainWindow::IsMainWindow()
{
    return true;
}

void MainWindow::InitViewports()
{
    auto vo = ViewportsOrchestrator::Instance();

    if (!vo->LoadViewports())
    {
        vo->AddNewViewport("Main", this, nullptr);
    }
}

void MainWindow::CloneViewport(Viewport *pViewport)
{
    auto       vo      = ViewportsOrchestrator::Instance();
    static int counter = 1;

    std::string name = std::format("Viewport {0}", counter++);

    vo->AddNewViewport(name.c_str(), this, nullptr);
}

Viewport *MainWindow::GetViewport(int index)
{
    return nullptr;
    //
    //     auto it = m_lstViewports.begin();
    //     std::advance(it, index);
    //     return *it;
}

void MainWindow::SetTitle(std::string &fileName)
{
    m_strTitle = std::format("LightBaker3000 FrontEnd, build #{1} - {0}", fileName, Application::Q_buildnum());
    SDL_SetWindowTitle(m_pSDLWindow, m_strTitle.c_str());
}

void MainWindow::UpdateStatusbar(int updateFlags)
{
    auto sr     = GetSceneRenderer();
    auto scene  = sr->GetScene();
    auto bitSet = [&](int flag) { return (updateFlags & (1 << flag)) > 0; };

    if (bitSet(StatusbarField::GameConfig))
    {
        auto conf = scene->UsedGameConfiguration();
        auto ptr  = conf.lock();

        if (ptr)
            m_statusBarData.gameName = std::string("Conf.: ") + ptr->Description();
        else
            m_statusBarData.gameName = "Conf.: <unavaible>";
    }

    if (bitSet(StatusbarField::Position))
    {
        glm::vec3 pos            = sr->GetRenderPos();
        m_statusBarData.position = std::format("Cam: {0:.3f} {1:.3f} {2:.3f}", pos.x, pos.y, pos.z);
    }

    if (bitSet(StatusbarField::ObjectDescription))
    {
        m_statusBarData.objectDescription = "<none selected>";
    }

    if (bitSet(StatusbarField::ObjectSize))
    {
        m_statusBarData.objectSize = "<none selected>";
    }

    if (bitSet(StatusbarField::GridStep))
    {
        int gs = GridRenderer::Instance()->GridStep();

        m_statusBarData.gridStep = std::format("Grid: {0} units", gs);
    }
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

void MainWindow::IterateUpdate()
{
    BT_PROFILE("MainWindow::IterateUpdate()");
    GL_CheckForErrors();

    SDL_GL_MakeCurrent(m_pSDLWindow, m_pGLContext);

    GLBackend::Instance()->NewFrame(); GL_CheckForErrors();
    Application::Instance()->CheckIfBakngFinished();
    GL_CheckForErrors();

    UpdateTimers();
    GL_CheckForErrors();
    GL_BeginFrame();
    GL_CheckForErrors();

    ViewportsOrchestrator::Instance()->RenderViewports(this, m_TimersData.frame_delta / 1000);
    GL_CheckForErrors();
    LoaderThread::Instance()->ExecuteEndCallbacks(10);
    GL_CheckForErrors();

    RenderGUI();
    GL_CheckForErrors();

    SDL_GL_SwapWindow(m_pSDLWindow);
}

void MainWindow::InitTimers()
{
    m_TimersData.timestamp_now          = SDL_GetPerformanceCounter();
    m_TimersData.timestamp_last         = SDL_GetPerformanceCounter();
    m_TimersData.frames_until_init      = 3;
    m_TimersData.fps_accum              = 0;
    m_TimersData.num_frames_this_second = 0;
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

void MainWindow::InitCommands()
{
    auto callbackLoadModel = [](std::string &fileName) {
        Application::GetMainWindow()->GetSceneRenderer()->LoadModel(fileName.c_str(), true);
    };

    Application::CommandsRegistry()->RegisterCommand(new CCommand(
        GlobalCommands::LoadFile, "Load...", 0, GetCommonIcon(CommonTextures::LoadFile), CMD_ON_MAINTOOLBAR, [&]() {
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
                        auto fs = FileSystem::Instance();

                        auto file = fs->ExtractFileName(it.first.c_str());
                        auto path = fs->ExtractFilePath(it.first.c_str());

                        file = std::format("{0}. {1}", ++index, file);

                        if (ImGui::MenuItem(file.c_str(), path.c_str()))
                        {
                            auto & filePath = it.first;

                            if (fs->FileExists(filePath.c_str()))
                                m_pSceneRenderer->LoadModel(filePath.c_str(), LRF_LOAD_ALL);
                            else
                            {
                                // MessageBox("File does not exists anymore and will be removed from list");
                                Application::GetPersistentStorage()->RemoveMRUItem(filePath);
                            }
                        }
                    }
                }

                ImGui::EndMenu();
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Exit"))
            {
                Application::Instance()->Terminate();
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

            COMMAND_ITEM(GlobalCommands::OpenProgramOptions);

            COMMAND_ITEM(GlobalCommands::ReloadAllShaders);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Windows"))
        {
            COMMAND_ITEM(GlobalCommands::ResetLayout);
            COMMAND_ITEM(GlobalCommands::OpenNewWindow);

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
    static int size = 21;

    ImGui::SetNextWindowPos(ImVec2(0, menuHeight));
    ImGui::SetNextWindowSize(ImVec2((float)m_iWindowWidth, 0));

    ImGui::Begin("##MainÅoolbar", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

//     auto toolbarCommands = Application::CommandsRegistry()->GetMainToolbarCommands();
// 
//     for (auto &it : toolbarCommands)
//     {
//         auto cmd = Application::CommandsRegistry()->GetCommandByInternalIndex(it);
//         cmd->RenderImGUI(23);
//     }

    float r = ImGui::GetWindowHeight();

    for (int i = (int)ToolbarIcons::Add; i < (int)ToolbarIcons::Undo; i++)
    {
        ImGuiHelpers::ButtonWithToolbarIcon((ToolbarIcons)i, "###Hello there!", size);        
        ImGui::SameLine();

        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
        ImGui::SameLine();
    }

    ImGui::End();

    ImGui::Begin("Tweak size");

        ImGui::InputInt("Icons size", &size);
    ImGui::End();

    return r;
}

void MainWindow::RenderGUI()
{
    BT_PROFILE("MainWindow::RenderGUI()");

    glViewport(0, 0, m_iWindowWidth, m_iWindowHeight);

    // start the Dear ImGui frame
    ImGui::SetCurrentContext(m_pImGUIContext);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    static int delayInit = 2;

    //    printf("ImGui::NewFrame()\n");
    ImGui::NewFrame();

    if (m_bUpdateImGuiStyleNextFrame)
        UIStyles::Manager::Instance()->ApplyCurrentStyle();

    if (delayInit > 0)
    {
        delayInit--;

        auto pers = Application::GetPersistentStorage();
        if (delayInit == 0)
        {
            if (pers->IsFreshFile())
                InitDocks();

            ViewportsOrchestrator::Instance()->FlagRepaintAll();

            Application::Instance()->FlagDelayedInitDone();
        }
    }

    float menuHeight    = RenderMainMenu();
    float toolbarHeight = RenderMainToolbar(menuHeight);

    DockSpaceOverViewport(toolbarHeight, ImGuiDockNodeFlags_PassthruCentralNode, 0);

    for (auto &it : m_vPanels)
        it->InvokeRender();

    PopupsManager::Instance()->RenderPopups();

    if (Application::IsWaitingForBakerToFinish())
    {
        float yBannerOffset = toolbarHeight + menuHeight + 8;
        DrawBakingInProgressBanner(yBannerOffset);
    }

    DrawLoadingBanner();

    DrawStatusBar();

#ifdef _DEBUG
    ImGui::ShowDemoWindow();
#endif

    ViewportsOrchestrator::Instance()->DisplayViewports(this);

    ImGui::Render();

    // Update and Render additional Platform Windows
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void MainWindow::DrawStatusBar()
{
    auto viewport = ImGui::FindViewportByID(m_defaultDockSides.idDockLeftBottom);

    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;

    if (ImGui::BeginViewportSideBar("MENU", viewport, ImGuiDir_Down, 20, window_flags))
    {
        if (ImGui::BeginMenuBar())
        {
            ImGui::Text("%s", m_statusBarData.gameName.c_str());

            ImGui::Separator();
            ImGui::Text("%s", m_statusBarData.position.c_str());

            ImGui::Separator();
            ImGui::Text("%s", m_statusBarData.objectDescription.c_str());

            ImGui::Separator();
            ImGui::Text("%s", m_statusBarData.objectSize.c_str());

            ImGui::Separator();
            ImGui::Text("%s", m_statusBarData.gridStep.c_str());

            ImGui::EndMenuBar();
        }

        ImGui::End();
    }
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

void MainWindow::GL_BeginFrame()
{
    glViewport(m_i3DViewport[0], m_i3DViewport[1], m_i3DViewport[2], m_i3DViewport[3]);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);

    glClearDepth(1);

    GL_CheckForErrors();

    glDisable(GL_MULTISAMPLE);

    GL_CheckForErrors();

    // glClearColor(0.25, .25, .25, 1);

    ClearBackground(true);
    GL_CheckForErrors();


    glClear(GL_DEPTH_BUFFER_BIT);

    GLScreenSpace2DRenderer::Instance()->NewFrame(m_i3DViewport);
    GL_CheckForErrors();
    
}

void MainWindow::ClearBackground(bool rebindShader)
{
    auto col1 = m_pBackgroudColorSetting1->GetColorRGB();
    auto col2 = m_pBackgroudColorSetting2->GetColorRGB();

    glClearColor(col1[0], col1[1], col1[2], 1);
    glClear(GL_DEPTH_BUFFER_BIT);
    GL_CheckForErrors();

    if (m_pUseGradientBackground->GetAsBool())
    {
        glDepthMask(0);
        GL_CheckForErrors();

        if (rebindShader)
        {
            m_pBackgroundShader->Bind();
            m_pBackgroundMesh->Bind();
        }

        for (auto &it : m_pBackgroundShader->Uniforms())
        {
            switch (it->Kind())
            {
            case UniformKind::Color:
                it->SetFloat4({col1.xyz, 1});
                GL_CheckForErrors();
                break;
            case UniformKind::Color2:
                it->SetFloat4({col2.xyz, 1});
                GL_CheckForErrors();
                break;
            default:
                __debugbreak();
                break;
            }
        }

        GL_CheckForErrors();
        m_pBackgroundMesh->Draw();
        GL_CheckForErrors();

        glDepthMask(1);
        GL_CheckForErrors();
    }
    else
    {
        glClear(GL_COLOR_BUFFER_BIT);
        GL_CheckForErrors();
    }
}

void MainWindow::UpdateTimers()
{
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
    int             target_fps = 60;
    static uint64_t next_tick  = 0;
    uint64_t        this_tick  = SDL_GetTicks64();
    if (this_tick < next_tick)
    {
        SDL_Delay((uint32_t)(next_tick - this_tick));
    }
    // this_tick = SDL_GetTicks64(); // WTF?
    next_tick = this_tick + (1000 / target_fps);
#else
    int             target_fps = m_pSettings->Get(IDS_FPS_LIMIT)->GetAsInt();
    static uint32_t next_tick  = 0;
    uint32_t        this_tick  = SDL_GetTicks();
    if (this_tick < next_tick)
    {
        SDL_Delay((uint32_t)(next_tick - this_tick));
    }
    // this_tick = SDL_GetTicks64(); // WTF?
    next_tick = this_tick + (1000 / target_fps);
#endif
}
