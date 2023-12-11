/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "gl_screenspace_2d_renderer.h"

GLScreenSpace2DRenderer::GLScreenSpace2DRenderer()
{
    m_p2DShader = GLBackend::Instance()->QueryShader("res/glprogs/screenspace_2d.glsl", {"DIFFUSEMAP"});
    m_p2DShaderTint =
        GLBackend::Instance()->QueryShader("res/glprogs/screenspace_2d.glsl", {"DIFFUSEMAP", "TINT_COLOR"});

    m_pDrawMesh = new DrawMesh(DrawMeshFlags::Dynamic);
}

GLScreenSpace2DRenderer::~GLScreenSpace2DRenderer()
{
    delete m_p2DShaderTint;
    delete m_p2DShader;
    delete m_pDrawMesh;
}

GLScreenSpace2DRenderer *GLScreenSpace2DRenderer::Instance()
{
    static GLScreenSpace2DRenderer *sInstance = new GLScreenSpace2DRenderer;
    return sInstance;
}

void GLScreenSpace2DRenderer::NewFrame(int *pGLViewport)
{
    for (int i = 0; i < 4; i++)
        m_pGLViewport[i] = pGLViewport[i];
}

void GLScreenSpace2DRenderer::DrawQuad(glm::vec2 pos, glm::vec2 size, GLTexture *pTexture)
{
    m_p2DShader->Bind();

    for (auto & it: m_p2DShader->Uniforms())
    {
        switch (it->Kind())
        {
        case UniformKind::Viewport:
            it->SetInt4(*(glm::ivec4*)&m_pGLViewport);
            break;        
        }
    }


    pTexture->Bind(0, 0);
    m_pDrawMesh->Begin(GL_TRIANGLE_STRIP);
        
    // clang-format off
    m_pDrawMesh->TexCoord2f(0,1);
    m_pDrawMesh->Vertex2f(pos.x          , pos.y);
    m_pDrawMesh->TexCoord2f(1,1);
    m_pDrawMesh->Vertex2f(pos.x + size.x , pos.y);
    m_pDrawMesh->TexCoord2f(0,0);
    m_pDrawMesh->Vertex2f(pos.x          , pos.y + size.y);
    m_pDrawMesh->TexCoord2f(1,0);
    m_pDrawMesh->Vertex2f(pos.x + size.x , pos.y + size.y);
    // clang-format on

    m_pDrawMesh->End();
    m_pDrawMesh->BindAndDraw();
}

void GLScreenSpace2DRenderer::DrawQuadTinted(glm::vec2 pos, glm::vec2 size, GLTexture *pTexture, glm::vec4 tint)
{
    m_p2DShaderTint->Bind();

    for (auto &it : m_p2DShaderTint->Uniforms())
    {
        switch (it->Kind())
        {
        case UniformKind::Viewport:
            it->SetInt4(*(glm::ivec4 *)&m_pGLViewport);
            break;        
        }
    }

    pTexture->Bind(0, 0);
    m_pDrawMesh->Begin(GL_TRIANGLE_STRIP);

    m_pDrawMesh->Color4fv((float *)&tint);

    // clang-format off    
    m_pDrawMesh->Vertex2f(pos.x          , pos.y);
    m_pDrawMesh->Vertex2f(pos.x + size.x , pos.y);
    m_pDrawMesh->Vertex2f(pos.x          , pos.y + size.y);
    m_pDrawMesh->Vertex2f(pos.x + size.x , pos.y + size.y);
    // clang-format on

    m_pDrawMesh->End();
    m_pDrawMesh->BindAndDraw();
}
