/*
        LightBaker3000 Frontend project,
        (c) 2023 CrazyRussian
*/

#pragma once

class IPlatformWindow
{
protected:
    SDL_Window *m_pSDLWindow;    
    SDL_GLContext m_pGLContext;

    ImGuiContext *m_pImGUIContext;
private:
    std::list<IEventHandler *> m_vEventHandlers;

    bool m_bTerminated = false;
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
};