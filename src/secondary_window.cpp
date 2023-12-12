/*
        LightBaker3000 Frontend project,
        (c) 2023 CrazyRussian
*/


#include "application.h"
#include "secondary_window.h"
#include "viewports_orchestrator.h"

SecondaryWindow::SecondaryWindow(std::string title) : m_strTitle(title)
{
    SDL_WindowFlags window_flags =
        (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MAXIMIZED);
    m_pSDLWindow = SDL_CreateWindow(m_strTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
                                    600, window_flags);

    std::string threadName = std::format("SecodaryWindow (%s) thread", title.c_str());
 
    IPlatformWindow *w = Application::GetMainWindow();

    m_pGLContext = w->GLContext();

    InitImGuiContext();

    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    //m_pGLContext = SDL_GL_CreateContext(m_pSDLWindow);
}

SecondaryWindow::~SecondaryWindow()
{
    if (m_pGLContext) SDL_GL_DeleteContext(m_pGLContext);
    if (m_pSDLWindow) SDL_DestroyWindow(m_pSDLWindow);
}

void SecondaryWindow::LoopStep()
{
    Application::GetMainWindow()->ClearBackground();
    
    ImGui::SetCurrentContext(m_pImGUIContext);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();

    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    
    
    ViewportsOrchestrator::Instance()->DisplayViewports(this);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void SecondaryWindow::IterateUpdate()
{
   SDL_GL_MakeCurrent(m_pSDLWindow, m_pGLContext);
   LoopStep();   
   SDL_GL_SwapWindow(m_pSDLWindow);
}

bool SecondaryWindow::HandleEvent(SDL_Event &event)
{
    ImGui::SetCurrentContext(m_pImGUIContext);
    ImGui_ImplSDL2_ProcessEvent(&event);
    return true;
}

void SecondaryWindow::InitImGuiContext()
{
    // Save current context to prevent crashing
    auto currentCont = ImGui::GetCurrentContext();

    m_pImGUIContext = ImGui::CreateContext();

    ImGui::SetCurrentContext(m_pImGUIContext);

    ImGuiIO &io     = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_NoMouseCursorChange;
    io.MouseDrawCursor = true;

    char *p = SDL_GetPrefPath(SDL_ORGANIZATION, SDL_APP_NAME);
    m_strImGuiIniPath = std::format("{0}/imgui_secondary.ini", p);

    io.IniFilename = m_strImGuiIniPath.c_str();

    ImGui_ImplSDL2_InitForOpenGL(m_pSDLWindow, m_pGLContext);
    ImGui_ImplOpenGL3_Init("#version 330");
    
    // Restore current context
    ImGui::SetCurrentContext(currentCont);
}

