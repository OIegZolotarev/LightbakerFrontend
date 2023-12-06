/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "camera.h"
#include "event_handler.h"
#include "gl_framebuffer_object.h"
#include "mathlib.h"
#include "scene_renderer.h"
#include "viewport_rendermodes.h"

BETTER_ENUM(AnchoringCorner, int, TopLeft, TopRight, BottomRight, BottomLeft);

class Viewport : public IEventHandler
{
    // Window title and keys
    std::string m_strName;
    std::string m_strNamePopupKey;

    // Positioning
    glm::vec2 m_DisplayWidgetPosition;
    glm::vec2 m_FrameBufferSize;
    glm::vec2 m_ClientAreaSize;

    // State
    bool m_bHovered     = false;
    bool m_bForceUndock = false;
    bool m_bDocked      = false;
    bool m_bVisible     = true;

    bool m_bNeedUpdate = true;

    void DoCloneViewport();

    // Rendering
    Camera *             m_pCamera;
    GLFramebufferObject *m_pFBO;
    RenderMode           m_RenderMode = RenderMode::Lightshaded;
    int                  ReadPixel(unsigned int x, unsigned int y);

    // Mouse utils
    glm::vec2 CalcRelativeMousePos();
    bool      PointInClientRect(glm::vec2 pt);

    // Displaying
    void DisplayViewportUI(ImVec2 pos);
    void UpdateDisplayWidgetPos();

    // Object picking
    void HanlePicker();

public:
    Viewport(AnchoringCorner anchoringBits);
    ~Viewport();

    void RenderFrame(float flFrameDelta);

    // Displaying
    void DisplayRenderedFrame();

    int HandleEvent(bool bWasHandled, SDL_Event &e, float flFrameDelta) override;

    // Getters
    Camera *   GetCamera();
    RenderMode GetRenderMode();

    void        OutputDebug();
    const char *Name();
    glm::vec2   GetClientArea();

private:
    size_t m_hoveredObjectId;
};
