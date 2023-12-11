/*
        LightBaker3000 Frontend project,
        (c) 2023 CrazyRussian
*/


#pragma once
#include "common.h"


class SecondaryWindow: public IPlatformWindow
{
    std::string m_strTitle;
public:
    SecondaryWindow(std::string t);
    ~SecondaryWindow();
    
    void LoopStep();

    void IterateUpdate() override;
    bool HandleEvent(SDL_Event &event) override;
};
