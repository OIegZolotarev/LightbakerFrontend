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

    GLTexture *m_pColorTexture;
    GLTexture *m_pDepthTexture;

    GLuint m_uiDepthRenderbuffer;

    int m_uiDimensions;
    int m_Width;
    int m_Height;

    void SetupColorAttachment();
    void SetupDepthTextureAttachment();
    void SetupDepthRenderbufferAttachment();

public:
    GLFramebufferObject(int width, int height, FBOType type);
    ~GLFramebufferObject();

    void Enable();
    void Disable();
       

    GLuint Width();

    GLuint Height();
    GLTexture *ColorTexture();
};
