/*
        LightBaker3000 Frontend project,
        (c) 2023 CrazyRussian
*/


#pragma once
#include "common.h"


class SecondaryWindow
{
    std::string m_strTitle;
    SDL_Window *m_pSDLWindow;
    SDL_GLContext m_pGLContext;

    SDL_Thread* m_Thread;

    static int threadProc(void *data);

public:
    SecondaryWindow(std::string t);
    ~SecondaryWindow();
    

int ThreadProc();
};
