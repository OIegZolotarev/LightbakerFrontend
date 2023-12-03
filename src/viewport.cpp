/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "viewport.h"

 Viewport::Viewport(AnchoringCorner anchoringBits)
{
     // TODO: make each FBO sized up to full screen size?
     int width = 2048;
     int height = 2048;

     m_pFBO = new GLFramebufferObject(width, height, FBOType::RGBA, -1);
 }

Viewport::~Viewport()
{
    delete m_pFBO;
}

void Viewport::RenderFrame()
{
    m_pFBO->Enable();
    glClearColor(0, 1, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    m_pFBO->Disable();
}

void Viewport::DisplayRenderedFrame()
{
}
