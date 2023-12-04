/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "mathlib.h"
#include "scene_renderer.h"
#include "camera.h"
#include "gl_framebuffer_object.h"
#include "viewport_rendermodes.h"
#include "event_handler.h"

BETTER_ENUM(AnchoringCorner, int, TopLeft, TopRight, BottomRight, BottomLeft);

class Viewport: public IEventHandler
{   

    std::string m_strName;

    glm::vec2 m_DisplayWidgetPosition;
    glm::vec2 m_FrameBufferSize;
    glm::vec2 m_ClientAreaSize;

    Camera* m_pCamera;
    GLFramebufferObject *m_pFBO;

    RenderMode m_RenderMode = RenderMode::Lightshaded;

    bool m_bHovered = false;

    glm::vec2 CalcRelativeMousePos();
    bool      PointInClientRect(glm::vec2 pt);

    int ReadPixel(unsigned int x, unsigned int y); 

public:
    Viewport(AnchoringCorner anchoringBits);
    ~Viewport();

    void RenderFrame(float flFrameDelta);
    void DisplayRenderedFrame();

    void DisplayViewportUI();

    void UpdateDisplayWidgetPos();

    
    int HandleEvent(bool bWasHandled, SDL_Event &e, float flFrameDelta) override;

    Camera *GetCamera()
    {
    return m_pCamera;
    }
    RenderMode GetRenderMode();


    void OutputDebug();
    const char *Name();
    glm::vec2   GetClientArea();
};
