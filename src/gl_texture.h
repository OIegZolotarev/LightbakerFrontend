/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/

#pragma once

#include "common.h"
#include "file_system.h"
#include "loader_thread.h"
//#include "wad_textures.h"

namespace GoldSource
{
class WADTexturePool;
};

enum class TextureSource
{
    CommonImage,
    GoldSourceMipTexture,
    GoldSourceWadFile,
    GoldSourceSprite,
    GuessByItself,
    Unknown
};

typedef struct rawimage_s
{
    size_t frameIndex = 0;
    size_t mipLevel   = 0;

    void * pixels;
    size_t m_iWidth;
    size_t m_iHeight;

    GLint glInternalFormat;
    GLint glFormat;
} rawimage_t;

class RawTexture
{
    std::list<rawimage_t *> m_Frames;

public:
    RawTexture(size_t nFrames);
    ~RawTexture();

    void AddRawFrame(rawimage_t *frame);

    size_t      NumFrames();
    rawimage_t *RawFrame(size_t index);
};

#define FL_HAS_ALPHA_CHANNEL (1 << 0)

class GLTexture
{
    friend class TextureManager;

public:
    GLTexture(const char *fileName, TextureSource source, bool fallback, size_t nFrames = 1);
    ~GLTexture();

    bool IsLoaded() const;
    void SetLoaded(bool val);

    int  Width() const;
    void SetWidth(int val);

    int  Height() const;
    void SetHeight(int val);

    int  SpriteSheetWidth() const;
    void SetSpriteSheetWidth(int val);

    int  SpriteSheetHeight() const;
    void SetSpriteSheetHeight(int val);

    GLuint GLTextureNum(size_t frame = 0) const;
    void   SetGLTextureNum(GLuint val, size_t frame = 0);

    void UploadPixels(void *pixels, GLint internalFormat, GLenum format);

    void GenerateGLHandle();
    void Bind(size_t unit, size_t frame = 0);

    void Ref();
    void Unref();

    bool HasAlpha();

    size_t       NumRef();
    std::string &Name();

private:
    bool m_bLoaded = false;
    int  m_iWidth  = 0;
    int  m_iHeight = 0;

    int m_iSpriteSheetWidth  = 0;
    int m_iSpriteSheetHeight = 0;

    TextureSource m_Source   = TextureSource::Unknown;
    bool          m_Fallback = false;

    RawTexture *m_pRawTextureData;

    size_t m_NumFrames  = 0;
    GLuint* m_uiGLTexnum = nullptr;

    size_t m_NumReferences = 0;

    int         m_iFlags      = 0;
    std::string m_strFileName = "";
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
    void *     m_pPixels  = nullptr;
    GLTexture *m_pTexture = nullptr;

public:
    AsynchTextureLoadResult(GLTexture *texture, void *pixels);
    ~AsynchTextureLoadResult();

    void ExecuteOnCompletion() override;
    bool NeedEndCallback() override;
};

class AsynchTextureLoadTask : public ITask
{
    std::queue<GLTexture *> m_qScheduledTextures;
    void *                  LoadTextureFileData(GLTexture *texture);

public:
    AsynchTextureLoadTask(const char *setDescription = nullptr);
    ~AsynchTextureLoadTask();

    GLTexture *      ScheduleTexture(const char *fileName);
    ITaskStepResult *ExecuteStep(LoaderThread *loaderThread) override;
    void             OnCompletion() override;
};

/// Texture manager

enum class FallbackTexture
{
    EmoCheckerboard = 0,
    White,
};

class TextureManager
{
    std::list<GoldSource::WADTexturePool *> m_lstWADSPool;
    std::list<GLTexture *>                  m_lstTexturesPool;

    static TextureSource DetermineTextureSourceFromFileName(const char *fileName);
    static TextureSource DetermineTextureSourceFileFileSignature(void *pixels, size_t length);

    GLTexture *m_pEmoTexture;
    GLTexture *m_pWhiteTexture;

public:
    ~TextureManager();

    static TextureManager *Instance()
    {
        static TextureManager *sInstance = new TextureManager;
        return sInstance;
    }

    void RegisterWAD(const char *fileName, bool shared);
    void UnregisterWAD(const char *fileName);

    // Shorter forms, falls back to EMO texture
    static void LoadTexture(GLTexture *texture, TextureSource source = TextureSource::GuessByItself);
    static void LoadTexture(GLTexture *texture, void *pixels, size_t length,
                            TextureSource source = TextureSource::GuessByItself);

    static void LoadTexture(GLTexture *texture, FallbackTexture fallbackTexture,
                            TextureSource source = TextureSource::GuessByItself);
    static void LoadTexture(GLTexture *texture, void *pixels, size_t length, FallbackTexture fallbackTexture,
                            TextureSource source = TextureSource::GuessByItself);

    void PurgeTextures();

private:
    void MakeFallbackTexture(GLTexture *texture, FallbackTexture fallbackTexture);
};
