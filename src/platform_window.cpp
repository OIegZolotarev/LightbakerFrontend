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
