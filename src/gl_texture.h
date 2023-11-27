/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/

#pragma once

#include "common.h"
#include "file_system.h"
#include "loader_thread.h"

enum class TextureSource
{
    CommonImage,
    GoldSourceMipTexture,
    GoldSourceWadFile,
    GoldSourceSprite
};

typedef struct rawpixels_s
{
    void *pixels;
    size_t width;
    size_t height;

    GLint glInternalFormat;
    GLint glFormat;
}rawpixels_t;

class GLTexture
{

public:    
    ~GLTexture();

    bool        IsLoaded() const;
    void        SetLoaded(bool val);
    std::string file_name;

    int    Width() const;
    void   SetWidth(int val);
    
    int    Height() const;
    void   SetHeight(int val);
    
    int    SpriteSheetWidth() const;
    void   SetSpriteSheetWidth(int val);
    
    int    SpriteSheetHeight() const;
    void   SetSpriteSheetHeight(int val);
    
    GLuint GLTextureNum() const;
    void   SetGLTextureNum(GLuint val);

    void UploadPixels(void * pixels, GLint internalFormat, GLenum format);


    void GenerateGLHandle();

    void Bind(size_t unit);

private:
    bool   loaded              = false;
    int    width               = 0;
    int    height              = 0;
    int    sprite_sheet_width  = 0;
    int    sprite_sheet_height = 0;
    GLuint gl_texnum           = 0;

    size_t num_references = 0;
};

GLTexture *LoadGLTexture(const char *fileName, bool force = false);
GLTexture *LoadGLTexture(FileData *pFileData, bool force = false);

void GLReloadTexture(GLTexture *);
void FreeGLTextures();
void FreeGLTexture(GLTexture *t);

// Asynchronouse loader
//
class AsynchTextureLoadResult : public ITaskStepResult
{
    void *       m_pPixels  = nullptr;
    GLTexture *m_pTexture = nullptr;

public:
    AsynchTextureLoadResult(GLTexture *texture, void *pixels);
    ~AsynchTextureLoadResult();

    void ExecuteOnCompletion() override;
    bool NeedEndCallback() override;
};

class AsynchTextureLoadTask : public ITask
{
    std::queue<GLTexture *>   m_qScheduledTextures;
    void *                    LoadTextureFileData(GLTexture *texture);

public:
    AsynchTextureLoadTask(const char *setDescription = nullptr);
    ~AsynchTextureLoadTask();

    GLTexture *    ScheduleTexture(const char *fileName);
    ITaskStepResult *ExecuteStep(LoaderThread *loaderThread) override;
    void             OnCompletion() override;
};

class TextureManager
{
public:
    TextureManager();
    ~TextureManager();

    void RegisterWADTexturePool(const char *wadFileName);
    
};

