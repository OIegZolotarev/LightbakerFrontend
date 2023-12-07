/*
        LightBaker3000 Frontend project,
        (c) 2023 CrazyRussian
*/


#include "application.h"
#include "secondary_window.h"

int SecondaryWindow::threadProc(void *data)
{
    SecondaryWindow *pWindow = (SecondaryWindow *) data;
    return pWindow->ThreadProc();
}

SecondaryWindow::SecondaryWindow(std::string title) : m_strTitle(title)
{
    SDL_WindowFlags window_flags =
        (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MAXIMIZED);
    m_pSDLWindow = SDL_CreateWindow(m_strTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,
                                    600, window_flags);



    std::string threadName = std::format("SecodaryWindow (%s) thread", title.c_str());
    m_Thread = SDL_CreateThread(threadProc, threadName.c_str(), this);
}

SecondaryWindow::~SecondaryWindow()
{
    if (m_pGLContext) SDL_GL_DeleteContext(m_pGLContext);
    if (m_pSDLWindow) SDL_DestroyWindow(m_pSDLWindow);
}

int SecondaryWindow::ThreadProc()
{
    // limit to which minimum size user can resize the window
    SDL_SetWindowMinimumSize(m_pSDLWindow, 500, 300);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

    m_pGLContext = SDL_GL_CreateContext(m_pSDLWindow);

    float f = 0;

    while (true)
    {
        
        glClearColor(0, ((sin(f) / 6.28) + 1) / 2.0f, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        
        SDL_GL_SwapWindow(m_pSDLWindow);
        f += 0.01;
    }

    return 0;
}
