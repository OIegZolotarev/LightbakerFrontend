/*
        LightBaker3000 Frontend project,
        (c) 2023 CrazyRussian
*/

#include "platform_window.h"
#include "common.h"
#include "imgui_internal.h"
#include "selection_3d.h"
#include "ui_common.h"
#include "application.h"

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
            return true;
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
        Application::Instance()->Terminate();
        return false;
        break;
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

    // m_pSceneRenderer->LoadModel(dropped_filedir, LRF_LOAD_ALL);

    SDL_free(dropped_filedir); // Free dropped_filedir memory
}

bool IPlatformWindow::PropagateControlsEvent(SDL_Event &event)
{
    bool bWasHandled = false;

    for (auto &it : EventHandlers())
    {
        int result = it->HandleEvent(bWasHandled, event, m_TimersData.frame_delta / 1000);

        bWasHandled = bWasHandled | result & EVENT_HANDLED;

        if (result & EVENT_CONSUMED)
            break;
    }

    return bWasHandled;
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
