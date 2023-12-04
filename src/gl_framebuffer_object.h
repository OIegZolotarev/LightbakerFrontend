/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "common.h"


enum class AttachmentTypes
{
    RGBA = 0,
    R32UI,
    ListEnd
};

class GLFramebufferObject
{
    GLuint m_uiFBOHandle;

    GLTexture *m_pColorTexture;
    GLTexture *m_pDepthTexture;

    GLuint m_uiDepthRenderbuffer;

    int m_uiDimensions;
    int m_Width;
    int m_Height;

    void SetupColorAttachment(int n, AttachmentTypes typeHint);
    void SetupDepthTextureAttachment();
    void SetupDepthRenderbufferAttachment();

public:
    GLFramebufferObject(int width, int height, int nTargets = 1, AttachmentTypes * attachmentHints = 0);
    ~GLFramebufferObject();

    void Enable();
    void Disable();
       

    GLuint Width();

    GLuint Height();
    GLTexture *ColorTexture();
};
