/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "gl_framebuffer_object.h"


#define GL_DEPTH_STENCIL_ATTACHMENT 0x821A

GLFramebufferObject::GLFramebufferObject(int w, int h, FBOType type): m_Type(type), m_Width(w), m_Height(h)
{
    glGenFramebuffers(1, &m_uiFBOHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_uiFBOHandle);

    m_pColorTexture = new GLTexture("*FBOColor", TextureSource::FrameBufferColor, false);
    m_pDepthTexture = new GLTexture("*FBODepth", TextureSource::FrameBufferDepth, false);


    SetupColorAttachment(); 
    SetupDepthRenderbufferAttachment();

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFramebufferObject::SetupColorAttachment()
{
    m_pColorTexture->GenerateGLHandle();
    m_pColorTexture->Bind(0, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pColorTexture->GLTextureNum(), 0);
}

void GLFramebufferObject::SetupDepthTextureAttachment()
{
    m_pDepthTexture->GenerateGLHandle();
    m_pDepthTexture->Bind(0, 0);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Width, m_Height, 0, GL_DEPTH_STENCIL,
                 GL_UNSIGNED_INT_24_8, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_pDepthTexture->GLTextureNum(), 0);
}

void GLFramebufferObject::SetupDepthRenderbufferAttachment()
{
    glGenRenderbuffers(1, &m_uiDepthRenderbuffer);

    glBindRenderbuffer(GL_RENDERBUFFER, m_uiDepthRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,
                                 m_uiDepthRenderbuffer);
}

GLFramebufferObject::~GLFramebufferObject()
{
    glDeleteFramebuffers(1, &m_uiFBOHandle);
}

void GLFramebufferObject::Enable()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_uiFBOHandle);
}

void GLFramebufferObject::Disable()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint GLFramebufferObject::Width()
{
    return m_Width;
}

GLuint GLFramebufferObject::Height()
{
    return m_Height;
}

GLTexture *GLFramebufferObject::ColorTexture()
{
    return m_pColorTexture;
}

