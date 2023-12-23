/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "r_camera.h"
#include "event_handler.h"
#include "gl_framebuffer_object.h"
#include "mathlib.h"
#include "platform_window.h"
#include "scene_renderer.h"
#include "viewport_rendermodes.h"
#include <nlohmann/json.hpp>

BETTER_ENUM(AnchoringCorner, int, TopLeft, TopRight, BottomRight, BottomLeft);

class Viewport : public IEventHandler
{
    // Window title and keys
    std::string m_strName;
    std::string m_strNamePopupKey;

    IPlatformWindow *m_pPlatformWindow;

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
    void   HandlePicker();
    size_t m_hoveredObjectId;

    glm::vec2 m_ClientAreaPos;

public:
    Viewport(const char *title, IPlatformWindow *pHostWindow, Viewport *pCopyFrom);
    ~Viewport();

    void RenderFrame(float flFrameDelta);
    void RenderGuizmo();

    // Displaying
    void DisplayRenderedFrame();

    int HandleEvent(bool bWasHandled, SDL_Event &e, float flFrameDelta) override;

    // Getters
    Camera *         GetCamera();
    RenderMode       GetRenderMode();
    IPlatformWindow *GetPlatformWindow();

    void        OutputDebug();
    const char *Name();
    glm::vec2   GetClientArea();

    void             SaveState(nlohmann::json &persistentData);
    static Viewport *LoadState(nlohmann::json &persistentData);

    void RegisterEventHandlerAtHost();
    bool IsVisible();
    glm::vec2 GetClientAreaPosAbs();

    void FlagUpdate()
    {
        m_bNeedUpdate = true;
    }
};
