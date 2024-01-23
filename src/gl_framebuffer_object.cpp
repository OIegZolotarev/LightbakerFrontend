/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "gl_framebuffer_object.h"
#include "text_utils.h"


#define GL_DEPTH_STENCIL_ATTACHMENT 0x821A

GLFramebufferObject::GLFramebufferObject(int w, int h, int nTargets, AttachmentTypes * types): m_Width(w), m_Height(h)
{
    assert(nTargets < 8);

    glGenFramebuffers(1, &m_uiFBOHandle);
    glBindFramebuffer(GL_FRAMEBUFFER, m_uiFBOHandle);

    m_pColorTexture = new GLTexture(VA("*FBOColor[%d targets]", nTargets), TextureSource::FrameBufferColor, false, nTargets);    
    m_pColorTexture->GenerateGLHandle();


    m_pDepthTexture = new GLTexture("*FBODepth", TextureSource::FrameBufferDepth, false);

    for (int i = 0; i < nTargets; i++)
    {
        AttachmentTypes type = AttachmentTypes::RGBA;
        if (types)
            type = types[i];

        SetupColorAttachment(i, type);
    }
    
    
    GLenum buffers[] = {GL_COLOR_ATTACHMENT0, 
                        GL_COLOR_ATTACHMENT1,
                        GL_COLOR_ATTACHMENT2,
                        GL_COLOR_ATTACHMENT3,
                        GL_COLOR_ATTACHMENT4,
                        GL_COLOR_ATTACHMENT5,
                        GL_COLOR_ATTACHMENT6,
                        GL_COLOR_ATTACHMENT7};

    if (nTargets > 1)
        glDrawBuffers(nTargets, buffers);
    
    SetupDepthRenderbufferAttachment();

    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);  
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFramebufferObject::SetupColorAttachment(int n, AttachmentTypes typeHint)
{    
    m_pColorTexture->Bind(0, n);

    GLenum internalFormat = 0;
    GLenum format = 0;

    switch (typeHint)
    {
    case AttachmentTypes::RGBA:
        internalFormat = GL_RGBA;
        format         = GL_RGBA;
        break;
    case AttachmentTypes::RGB:
        internalFormat = GL_RGB;
        format         = GL_RGB;
        break;
    case AttachmentTypes::R32UI:
        internalFormat = GL_R32UI;
        format         = GL_RGB_INTEGER;
        break;
    case AttachmentTypes::RG32UI:
        internalFormat = GL_RG32UI;
        format         = GL_RGB_INTEGER;
        break;
    case AttachmentTypes::ListEnd:
        break;
    default:
        break;
    
    }
        
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + n, GL_TEXTURE_2D, m_pColorTexture->GLTextureNum(n), 0);
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

