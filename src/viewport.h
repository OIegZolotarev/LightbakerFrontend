/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "mathlib.h"
#include "scene_renderer.h"
#include "camera.h"
#include "gl_framebuffer_object.h"

BETTER_ENUM(AnchoringCorner, int, TopLeft, TopRight, BottomRight, BottomLeft);

class Viewport
{   
    glm::vec2 m_Position;
    glm::vec2 m_Extents;

    class Camera* m_Camera;
    GLFramebufferObject *m_pFBO;

    RenderMode m_RenderMode;
public:
    Viewport(AnchoringCorner anchoringBits);
    ~Viewport();

    void RenderFrame();
    void DisplayRenderedFrame();
};
