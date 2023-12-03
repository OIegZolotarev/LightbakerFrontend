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

    glm::vec2 m_Position;
    glm::vec2 m_Extents;
    glm::vec2 m_ClientAreaSize;

    Camera* m_pCamera;
    GLFramebufferObject *m_pFBO;

    RenderMode m_RenderMode = RenderMode::Lightshaded;

    bool m_bHovered = false;

public:
    Viewport(AnchoringCorner anchoringBits);
    ~Viewport();

    void RenderFrame(float flFrameDelta);
    void DisplayRenderedFrame();
    
    int HandleEvent(bool bWasHandled, SDL_Event &e, float flFrameDelta) override;

    Camera *GetCamera()
    {
    return m_pCamera;
    }
    RenderMode GetRenderMode();


    void OutputDebug();
    const char *Name();
};
