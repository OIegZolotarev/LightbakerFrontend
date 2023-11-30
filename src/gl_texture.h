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

    byte* data = nullptr;
    size_t dataLength = 0;

    int width = 0;
    int height = 0;

    GLint glInternalFormat = 0;
    GLint glFormat = 0;

    rawimage_s(size_t _width, size_t _height, size_t components);
    ~rawimage_s();

} rawimage_t;

class RawTexture
{
    std::list<rawimage_t *> m_lstFrames;

public:
    RawTexture();
    ~RawTexture();

    void   AddRawFrame(rawimage_t *frame);
    size_t NumFrames();

    std::list<rawimage_t *> &Items()
    {
        return m_lstFrames;
    }
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

    void UploadRawTexture(RawTexture *pTexture);
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

    size_t  m_NumFrames  = 0;
    GLuint *m_uiGLTexnum = nullptr;

    size_t m_NumReferences = 0;

    int         m_iFlags      = 0;
    std::string m_strFileName = "";
};

// Old API

GLTexture *LoadGLTexture(const char *fileName, bool force = false);
GLTexture *LoadGLTexture(FileData *pFileData, bool force = false);

void GLReloadTexture(GLTexture *);
void FreeGLTextures();
void FreeGLTexture(GLTexture *t);

// Old api

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

    void MakeFallbackTexture(GLTexture *texture, FallbackTexture fallbackTexture);

    static RawTexture *LoadRawTexture(const char *fileName, TextureSource source = TextureSource::GuessByItself);
    static RawTexture *LoadRawTexture(void *pixels, size_t length, TextureSource source = TextureSource::GuessByItself);

        
    static RawTexture *DecodeCommonImage(const void *data, size_t length);
    static RawTexture *DecodeGoldsourceMiptex(const void *data, size_t length);
    static RawTexture *DecodeGoldsourceSprite(const void *data, size_t length);

    TextureManager();

public:
    ~TextureManager();

    static TextureManager *Instance()
    {
        static TextureManager *sInstance = new TextureManager;
        return sInstance;
    }

    void RegisterWAD(const char *fileName, bool shared);
    void UnregisterWAD(const char *fileName);

    static GLTexture *LoadTextureSynch(const char *fileName, TextureSource source = TextureSource::GuessByItself);
    static GLTexture *LoadTextureSynch(FileData* fd, TextureSource source = TextureSource::GuessByItself);

    void PurgeTextures();


};
