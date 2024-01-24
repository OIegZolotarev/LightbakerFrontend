/*
        LightBaker3000 Frontend project,
        (c) 2023 CrazyRussian
*/

#include "platform_window.h"
#include "application.h"
#include "common.h"
#include "imgui_internal.h"
#include "selection_3d.h"
#include "ui_common.h"
#include "viewports_orchestrator.h"

bool IPlatformWindow::CheckImGuiEvent(SDL_Event &event)
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        if (ImGui::GetIO().WantCaptureKeyboard)
            return true;
        break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        if (ImGui::GetIO().WantCaptureMouse)
        {

            // If we hovering any viewport - pass mouse events to it,
            //  kinda quirky behavior alas
            auto vp = ViewportsOrchestrator::Instance()->GetHoveredViewport();

            if (!vp)
                return true;
            else
                return false;
        }
        break;
    }

    return false;
}

bool IPlatformWindow::CommonHandleEvent(SDL_Event &event)
{
    ImGui::SetCurrentContext(m_pImGUIContext);
    ImGui_ImplSDL2_ProcessEvent(&event);

    if (CheckImGuiEvent(event))
        return true;

    switch (event.type)
    {
    case SDL_QUIT:
        return false;

    case SDL_WINDOWEVENT:
        return HandleWindowStateEvent(event);

    case SDL_KEYDOWN:
        HandleKeyDown(event);
        return true;

    case SDL_MOUSEMOTION:
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEWHEEL:
    case SDL_KEYUP:
        PropagateControlsEvent(event);
        return true;

    case SDL_DROPFILE:
        HandleDropfileEvent(event);
        return true;
    }

    return true;
}

void IPlatformWindow::HandleKeyDown(SDL_Event &event)
{
    switch (event.key.keysym.sym)
    {
    case SDLK_ESCAPE:
        SelectionManager::Instance()->UnSelect();
        return;
    }

    if (Application::CommandsRegistry()->OnKeyDown())
        return;

    if (PropagateControlsEvent(event))
        return;
}

bool IPlatformWindow::HandleWindowStateEvent(SDL_Event &e)
{
    bool bUpdateViewport = false;

    switch (e.window.event)
    {
    case SDL_WINDOWEVENT_CLOSE:
        if (IsMainWindow())
            Application::Instance()->Terminate();

        return false;
    case SDL_WINDOWEVENT_RESTORED:
        m_windowState   = 1;
        bUpdateViewport = true;
        break;
    case SDL_WINDOWEVENT_MAXIMIZED:
        m_windowState   = 2;
        bUpdateViewport = true;
        break;
    case SDL_WINDOWEVENT_MINIMIZED:
        m_windowState   = 0;
        bUpdateViewport = true;
        break;
    case SDL_WINDOWEVENT_ENTER:
        m_bHasMouse = true;
        break;
    case SDL_WINDOWEVENT_LEAVE:
        m_bHasMouse = false;
        break;
    case SDL_WINDOWEVENT_RESIZED:
    case SDL_WINDOWEVENT_SIZE_CHANGED:
        bUpdateViewport = true;
        break;
    }

    if (bUpdateViewport)
    {
        SDL_GetWindowSize(m_pSDLWindow, &m_iWindowWidth, &m_iWindowHeight);
        glViewport(0, 0, m_iWindowWidth, m_iWindowHeight);
    }

    return true;
}

void IPlatformWindow::HandleDropfileEvent(SDL_Event &event)
{
    char *dropped_filedir;

    // In case if dropped file
    dropped_filedir = event.drop.file;
    // Shows directory of dropped file

    auto sr = Application::GetMainWindow()->GetSceneRenderer();

    sr->LoadModel(dropped_filedir, LRF_LOAD_ALL);

    SDL_free(dropped_filedir); // Free dropped_filedir memory
}

bool IPlatformWindow::PropagateControlsEvent(SDL_Event &event)
{
    bool bWasHandled = false;

    for (auto &it : EventHandlers())
    {
        int result = it->HandleEvent(bWasHandled, event, m_TimersData.frame_delta / 1000);

        bWasHandled = bWasHandled || result & EVENT_HANDLED;

        if (result & EVENT_CONSUMED)
            break;
    }

    return bWasHandled;
}

bool IPlatformWindow::IsMainWindow()
{
    return false;
}

bool IPlatformWindow::HasMouseCursorInside()
{
    int mx, my;
    SDL_GetGlobalMouseState(&mx, &my);

    glm::ivec2 pos = {0, 0}, size = {0,0};

    SDL_GetWindowPosition(m_pSDLWindow, &pos.x, &pos.y);
    SDL_GetWindowSize(m_pSDLWindow, &size.x, &size.y);

    if (mx >= pos.x && mx < pos.x + size.x)
    {
        if (my >= pos.y && my < pos.y + size.y)
            return true;
    }

    return false;

}

ImGuiID IPlatformWindow::DockSpaceOverViewport(float heightAdjust, ImGuiDockNodeFlags dockspace_flags,
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

    m_DockSpaceId = ImGui::GetID(m_strTitle.c_str());

    ImGui::DockSpace(m_DockSpaceId, ImVec2(0.0f, 0.0f), dockspace_flags, window_class);

    // ImGui::ShowStyleSelector("Select style");

    auto c = ImGui::DockBuilderGetCentralNode(m_DockSpaceId);

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

    return m_DockSpaceId;
}

IPlatformWindow::~IPlatformWindow()
{
    m_vEventHandlers.clear();
}

size_t IPlatformWindow::GetId()
{
    return SDL_GetWindowID(m_pSDLWindow);
}

void IPlatformWindow::AddEventHandler(IEventHandler *pHandler)
{
    m_vEventHandlers.push_back(pHandler);
}

const std::list<IEventHandler *> &IPlatformWindow::EventHandlers()
{
    return m_vEventHandlers;
}

void IPlatformWindow::InitImGUISDL2Platform()
{
}

void IPlatformWindow::SetTerminated(bool flag)
{
    m_bTerminated = flag;
}

bool IPlatformWindow::IsTerminated()
{
    return m_bTerminated;
}

SDL_GLContext &IPlatformWindow::GLContext()
{
    return m_pGLContext;
}

void IPlatformWindow::RemoveEventHandler(IEventHandler *param1)
{
    m_vEventHandlers.remove_if([&](IEventHandler *t) { return t == param1; });
}

const char *IPlatformWindow::GetDescription()
{
    return m_strTitle.c_str();
}

bool IPlatformWindow::HasMouseInside()
{
    return m_bHasMouse;
}

glm::ivec2 IPlatformWindow::CenterPointRel()
{
    return {m_iWindowWidth / 2, m_iWindowHeight / 2};
}

SDL_Window *IPlatformWindow::SDLHandle()
{
    return m_pSDLWindow;
}

glm::ivec2 IPlatformWindow::CenterPointGlobal()
{
    int x, y;
    SDL_GetWindowPosition(m_pSDLWindow, &x, &y);

    return {x + m_iWindowWidth / 2, y + m_iWindowHeight / 2};
}

void IPlatformWindow::ScheduleImGuiStyleUpdate()
{
    m_bUpdateImGuiStyleNextFrame = true;
}

void IPlatformWindow::UpdateCursorVisibility(bool m_bMouseCursorVisible)
{
    m_bHasMouse = HasMouseCursorInside();

    if (m_bMouseCursorVisible)
    {
        if (m_bHasMouse)        
            m_pImGUIContext->IO.MouseDrawCursor = true;        
        else
            m_pImGUIContext->IO.MouseDrawCursor = false;
    }
    else 
            m_pImGUIContext->IO.MouseDrawCursor = false;
}
