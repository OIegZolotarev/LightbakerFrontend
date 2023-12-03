/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "gl_framebuffer_object.h"


#define GL_DEPTH_STENCIL_ATTACHMENT 0x821A

GLFramebufferObject::GLFramebufferObject(size_t w, size_t h, FBOType type, GLuint forcedColorTexture)
    : m_Type(type), m_Width(w), m_Height(h)
{
    glGenFramebuffers(1, &m_uiFBOHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_uiFBOHandle);

    if (forcedColorTexture != -1)
        m_uiColorTexture = forcedColorTexture;
    else
        glGenTextures(1, &m_uiColorTexture);

    SetupColorAttachment(m_uiColorTexture);

    m_uiDepthTexture = 0;
    // glGenTextures(1, &m_uiDepthTexture);
    // SetupColorAttachment(m_uiDepthTexture);

    SetupDepthRenderbufferAttachment();

    if (!(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE))
    {
        Application::EPICFAIL("glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFramebufferObject::SetupColorAttachment(GLuint texture)
{
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
}

void GLFramebufferObject::SetupDepthTextureAttachment(GLuint texture)
{
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Width, m_Height, 0, GL_DEPTH_STENCIL,
                 GL_UNSIGNED_INT_24_8, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
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

GLuint GLFramebufferObject::TextureHandle()
{
    return m_uiColorTexture;
}
