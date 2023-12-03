/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "common.h"

enum class FBOType
{
    RGBA = 0,
    Depth,
    CubemapRGBA,
    CubemapRGBA32,
    CubemapR32F
};

class GLFramebufferObject
{
    FBOType m_Type;

    GLuint m_uiFBOHandle;

    GLuint m_uiColorTexture;
    GLuint m_uiDepthTexture;
    GLuint m_uiDepthRenderbuffer;

    size_t m_uiDimensions;
    size_t m_Width;
    size_t m_Height;

    void SetupColorAttachment(GLuint texture);
    void SetupDepthTextureAttachment(GLuint texture);
    void SetupDepthRenderbufferAttachment();

public:
    GLFramebufferObject(size_t width, size_t height, FBOType type, GLuint forcedColorTexture);
    ~GLFramebufferObject();

    void Enable();
    void Disable();

    GLuint TextureHandle();

    GLuint Width()
    {
        return m_Width;
    }

    GLuint Height()
    {
        return m_Height;
    }
};
