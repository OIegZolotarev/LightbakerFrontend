/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "event_handler.h"
#include "gl_framebuffer_object.h"
#include "mathlib.h"
#include "platform_window.h"
#include "r_camera_controller.h"
#include "scene_renderer.h"
#include "viewport_rendermodes.h"
#include <nlohmann/json.hpp>

BETTER_ENUM(AnchoringCorner, int, TopLeft, TopRight, BottomRight, BottomLeft);

enum ViewportMouseHover
{
    NotHovered = 0,
    HoveredButObstructedWithUI,
    Hovered
};

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
    bool m_bHovered      = false;
    bool m_bHoveredImGUI = false;
    bool m_bFocused      = false;
    bool m_bForceUndock  = false;
    bool m_bDocked       = false;
    bool m_bVisible      = true;

    bool m_bNeedUpdate = true;

    void DoCloneViewport();

    // Rendering
    CameraController *   m_pCamera;
    GLFramebufferObject *m_pFBO;
    RenderMode           m_RenderMode = RenderMode::Lightshaded;
    int                  ReadPixel(unsigned int x, unsigned int y);

    // Mouse utils
    bool PointInClientRect(glm::vec2 pt);

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

    int HandleEvent(bool bWasHandled, const SDL_Event &e, const float flFrameDelta) override;

    // Getters
    CameraController *GetCamera();
    RenderMode        GetRenderMode();
    IPlatformWindow * GetPlatformWindow();

    void        OutputDebug();
    const char *Name();
    glm::vec2   GetClientArea();

    // Serialization
    void             SaveState(nlohmann::json &persistentData);
    static Viewport *LoadState(nlohmann::json &persistentData);

    void      RegisterEventHandlerAtHost();
    glm::vec2 GetClientAreaPosAbs();

    void FlagUpdate();

    size_t GetHoveredObjectID();

    // Visibility
    void SetVisible(bool flag);
    bool IsVisible();

    // Mouse
    glm::vec2          CalcRelativeMousePos(bool yAtTop = true);
    ViewportMouseHover GetMouseHoveringStatus();

    const glm::vec3 ScreenToWorld(glm::vec2 viewportCoords, float depthFraction, bool yAtBottom) const;

    void LookAt(const sceneCameraDescriptor_t &it);
};
