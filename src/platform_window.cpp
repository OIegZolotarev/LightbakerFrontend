/*
        LightBaker3000 Frontend project,
        (c) 2023 CrazyRussian
*/

#include "common.h"
#include "platform_window.h"

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
