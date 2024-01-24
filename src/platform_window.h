/*
        LightBaker3000 Frontend project,
        (c) 2023 CrazyRussian
*/

#pragma once

#include "common.h"

class IEventHandler;

struct timersData
{
    double frame_delta            = 0;
    int    actual_fps             = 0;
    uint64_t timestamp_now        = 0;
    uint64_t timestamp_last       = 0;
    int    frames_until_init      = 3;
    double fps_accum              = 0;
    int    num_frames_this_second = 0;
};

class IPlatformWindow
{
protected:
    SDL_Window *m_pSDLWindow = nullptr;    
    SDL_GLContext m_pGLContext;

    ImGuiContext *m_pImGUIContext = nullptr;

    int m_iWindowHeight = 800;
    int m_iWindowWidth = 600;

    std::string m_strTitle;

    // Window state
    bool m_bHasMouse;
    int  m_windowState = -1;

    bool CheckImGuiEvent(SDL_Event &event);
    bool CommonHandleEvent(SDL_Event &event);
        
    void HandleKeyDown(SDL_Event &event);
    bool HandleWindowStateEvent(SDL_Event &e);
    void HandleDropfileEvent(SDL_Event &event);

    bool PropagateControlsEvent(SDL_Event &event);

    timersData m_TimersData;

    virtual bool IsMainWindow();

    bool m_bUpdateImGuiStyleNextFrame;

    bool HasMouseCursorInside();

    

    ImGuiID DockSpaceOverViewport(float heightAdjust, ImGuiDockNodeFlags dockspace_flags,
                                              const ImGuiWindowClass *window_class);
    int     m_i3DViewport[4];

private:
    std::list<IEventHandler *> m_vEventHandlers;
    bool m_bTerminated = false;
    ImGuiID                    m_DockSpaceId = 0;

public:
    IPlatformWindow() = default;
    virtual ~IPlatformWindow();

    size_t GetId();

    void AddEventHandler(IEventHandler *pHandler);
    const std::list<IEventHandler *> &EventHandlers();
    virtual void                      InitImGUISDL2Platform();;
    
    /*
        IterateUpdate - runs single iteration of updates loop       
    */
    virtual void IterateUpdate() = 0;
    
    /*
        HandleEvent - should return false if window is closed or app is exiting 
        otherwise should return true    
    */
    virtual bool HandleEvent(SDL_Event & event)      = 0;
        
    void SetTerminated(bool flag);
    bool IsTerminated();

    SDL_GLContext & GLContext();


    void RemoveEventHandler(IEventHandler * handler);
    const char *GetDescription();
    bool        HasMouseInside();
    glm::ivec2  CenterPointRel();
    SDL_Window *SDLHandle();
    glm::ivec2  CenterPointGlobal();

    void ScheduleImGuiStyleUpdate();
    void UpdateCursorVisibility(bool m_bMouseCursorVisible);
};