/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "mathlib.h"
#include "gl_texture.h"
#include "gl_backend.h"


class GLScreenSpace2DRenderer
{
    int       m_pGLViewport[4] = {0};
    DrawMesh *m_pDrawMesh = nullptr;

    GLScreenSpace2DRenderer();

    ShaderProgram *m_p2DShader;
    ShaderProgram *m_p2DShaderTint;
public:    
    ~GLScreenSpace2DRenderer();

    static GLScreenSpace2DRenderer *Instance();

    void NewFrame(int * pGLViewport);

    void DrawQuad(glm::vec2 pos, glm::vec2 size, GLTexture *pTexture);
    void DrawQuadTinted(glm::vec2 pos, glm::vec2 size, GLTexture *pTexture, glm::vec4 tint);
};
