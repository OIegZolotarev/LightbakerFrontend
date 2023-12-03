/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "common.h"

// Type hint, not used really (yet)
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

    int m_uiDimensions;
    int m_Width;
    int m_Height;

    void SetupColorAttachment(GLuint texture);
    void SetupDepthTextureAttachment(GLuint texture);
    void SetupDepthRenderbufferAttachment();

public:
    GLFramebufferObject(int width, int height, FBOType type, GLuint forcedColorTexture);
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
