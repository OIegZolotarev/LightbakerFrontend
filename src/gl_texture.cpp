/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "application.h"
#include <stb/stb_image.h>
#include "loader_thread.h"
#include "gl_texture.h"
#include "wad_textures.h"
#include <locale>
#include <boost/algorithm/string.hpp>


void GLReloadTexture(GLTexture* r, FileData* sourceFile);
std::vector<GLTexture*> g_vecGLTextures;

GLTexture* LoadGLTexture(FileData* sourceFile, bool force)
{
	if (!force && sourceFile)
	{
		for (auto& it : g_vecGLTextures)
		{
			if (it->Name() == sourceFile->Name() 
					&& it->GLTextureNum() != 0)
				return it;
		}
	}

	GLTexture* r = new GLTexture(sourceFile->Name().c_str(), TextureSource::CommonImage, false);
	GLReloadTexture(r, sourceFile);
	
	g_vecGLTextures.push_back(r);

	return r;
}

GLTexture* LoadGLTexture(const char* fileName, bool force)
{
	if (!force)
	{
		for (auto& it : g_vecGLTextures)
		{
			if (!_stricmp(it->Name().c_str(),fileName)
				&& it->GLTextureNum() != 0)
				return it;
		}
	}

	FileData* pData = Application::GetFileSystem()->LoadFile(fileName);
	auto r = LoadGLTexture(pData, force);

	if (pData)
		delete pData;

	return r;
}

void GLReloadTexture(GLTexture* t)
{
	FileData* pData = Application::GetFileSystem()->LoadFile(t->Name().c_str());
	GLReloadTexture(t, pData);
	delete pData;
}

void GLReloadTexture(GLTexture* r,FileData * sourceFile)
{
	r->SetHeight(-1);
	r->SetWidth(-1);
	//r->SetGLTextureNum(0);
	

	if (!sourceFile)
		return;

	//	->file_name = sourceFile->Name();

	// Load from file
	int image_width = 0;
	int image_height = 0;
	int comps = 0;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* image_data = stbi_load_from_memory(sourceFile->Data(), sourceFile->Length(), &image_width, &image_height, &comps, 4);

	if (image_data == NULL)
		return;

	r->GenerateGLHandle();
    r->Bind(0);

	// Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // This is required on WebGL for non power-of-two textures
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Same

	 // glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 

	

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

	

	stbi_image_free(image_data);
		
	r->SetWidth(image_width);
	r->SetHeight(image_height);

	r->SetLoaded(true);

	return;
}

void FreeGLTextures()
{
	ClearPointersVector(g_vecGLTextures);
}

void FreeGLTexture(GLTexture* t)
{
	if (!t)
		return;

	auto pos = std::remove_if(g_vecGLTextures.begin(), g_vecGLTextures.end(), [&](GLTexture*a) -> bool
	{
		return a->GLTextureNum() == t->GLTextureNum();
	});

	if (pos != g_vecGLTextures.end())
	{
		delete* pos;
		g_vecGLTextures.erase(pos);
	}
}


void* AsynchTextureLoadTask::LoadTextureFileData(GLTexture* texture)
{
	FileData* pData = Application::GetFileSystem()->LoadFile(texture->Name());

	// TODO: emo textures
	if (!pData)
		return nullptr;

	// Load from file
	int image_width = 0;
	int image_height = 0;
	int comps = 0;

	stbi_set_flip_vertically_on_load(true);
	void * pixels = stbi_load_from_memory(pData->Data(), pData->Length(), &image_width, &image_height, &comps, 4);

	texture->SetWidth(image_width);
	texture->SetHeight(image_height);

	pData->UnRef();

	//SDL_Delay(100);

	return pixels;
}

AsynchTextureLoadTask::AsynchTextureLoadTask(const char* setDescription)
{
	if (setDescription != nullptr)
		m_strDescription = setDescription;
	else
		m_strDescription = "Loading textures...";
}

AsynchTextureLoadTask::~AsynchTextureLoadTask()
{
	assert(m_qScheduledTextures.empty());
}

GLTexture* AsynchTextureLoadTask::ScheduleTexture(const char* fileName)
{
	GLTexture* texture = new GLTexture(fileName, TextureSource::CommonImage,0);
	//memset(texture, 0, sizeof(GLTexture));

	//texture->file_name = fileName;

	m_qScheduledTextures.push(texture);

	m_nTotalSteps = m_qScheduledTextures.size();

	return texture;
}

ITaskStepResult* AsynchTextureLoadTask::ExecuteStep(LoaderThread* loaderThread)
{
	if (m_qScheduledTextures.empty())
		return new ITaskStepResult(TaskStepResultType::FinishedSuccesfully);

	GLTexture* texture = m_qScheduledTextures.front();
	m_qScheduledTextures.pop();

	m_nPerformedSteps++;

	void* pixels = LoadTextureFileData(texture);

	return new AsynchTextureLoadResult(texture, pixels);
}

void AsynchTextureLoadTask::OnCompletion()
{
	(void)0;
}

AsynchTextureLoadResult::AsynchTextureLoadResult(GLTexture* texture, void* pixels): ITaskStepResult(TaskStepResultType::StepPerformed)
{
	m_pPixels = pixels;
	m_pTexture = texture;

	m_strElementDescription = texture->Name();
}

AsynchTextureLoadResult::~AsynchTextureLoadResult()
{
	if (m_pPixels)
		stbi_image_free(m_pPixels);
}

void AsynchTextureLoadResult::ExecuteOnCompletion()
{
	// TODO: emo textures
	if (!m_pPixels)
	{
		return;
	}

	m_pTexture->UploadPixels(m_pPixels, GL_RGBA, GL_RGBA);
}

bool AsynchTextureLoadResult::NeedEndCallback()
{
	return true;
}

bool GLTexture::IsLoaded() const
{
    return m_bLoaded;
}

void GLTexture::SetLoaded(bool val)
{
    m_bLoaded = val;
}

int GLTexture::Width() const
{
    return m_iWidth;
}

void GLTexture::SetWidth(int val)
{
    m_iWidth = val;
}

int GLTexture::Height() const
{
    return m_iHeight;
}

void GLTexture::SetHeight(int val)
{
    m_iHeight = val;
}

int GLTexture::SpriteSheetWidth() const
{
    return m_iSpriteSheetWidth;
}

void GLTexture::SetSpriteSheetWidth(int val)
{
    m_iSpriteSheetWidth = val;
}

int GLTexture::SpriteSheetHeight() const
{
    return m_iSpriteSheetHeight;
}

void GLTexture::SetSpriteSheetHeight(int val)
{
    m_iSpriteSheetHeight = val;
}

GLuint GLTexture::GLTextureNum(size_t frame) const
{
    if (m_uiGLTexnum)
		return m_uiGLTexnum[frame];
    return 0;
}

void GLTexture::SetGLTextureNum(GLuint val, size_t frame)
{
    m_uiGLTexnum[frame] = val;
}

void GLTexture::UploadPixels(void *pixels, GLint internalFormat, GLenum format)
{   
    GenerateGLHandle();
    Bind(0);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non
    // power-of-two textures glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_iWidth, m_iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    // thread-safe
    // g_vecGLTextures.push_back(m_pTexture);

    // stbi_image_free(m_pPixels);
}

 GLTexture::GLTexture(const char *fileName, TextureSource source, bool fallback, size_t nFrames /*= 1*/)
{
    m_strFileName = fileName;
    m_Source      = source;
    m_Fallback    = fallback;
    m_NumFrames   = nFrames;
}

GLTexture::~GLTexture()
{
    if (m_uiGLTexnum)
    {
        delete m_uiGLTexnum;
        glDeleteTextures(m_NumFrames, m_uiGLTexnum);
    }
}

void GLTexture::GenerateGLHandle()
{
    //assert(!m_uiGLTexnum);


	// TODO: handle texture reloading
	if (!m_uiGLTexnum)
    {
        m_uiGLTexnum = new GLuint[m_NumFrames];
        glGenTextures(m_NumFrames, m_uiGLTexnum);
    }
}

void GLTexture::Bind(size_t unit, size_t frame)
{
    GLBackend::BindTexture(unit, m_uiGLTexnum[frame]);
}

void GLTexture::Ref()
{
    m_NumReferences++;
}

void GLTexture::Unref()
{
    m_NumReferences--;
}

bool GLTexture::HasAlpha()
{
    return m_iFlags & FL_HAS_ALPHA_CHANNEL;
}

size_t GLTexture::NumRef()
{
    return m_NumReferences;
}

std::string &GLTexture::Name()
{
    return m_strFileName;
}

TextureSource TextureManager::DetermineTextureSourceFromFileName(const char *fileName)
{
    auto fs  = FileSystem::Instance();
    auto ext = fs->ExtractFileExtension(fileName);
    boost::algorithm::to_lower(ext);

	struct sLookupTable
	{
        const char *ext;
        TextureSource fmt;
	};

	static sLookupTable lookup[] = {
		{"tga", TextureSource::CommonImage},
		{"png", TextureSource::CommonImage},
		{"jpg", TextureSource::CommonImage},		
		{"spr", TextureSource::GoldSourceSprite},
    };

	for (auto it: lookup)
    {
        if (!strcmp(it.ext, ext.c_str()))
            return it.fmt;
	}
    

	// Can't determine format by extension, will try with file header
	//
	return TextureSource::GuessByItself;
}

TextureSource TextureManager::DetermineTextureSourceFileFileSignature(void *pixels, size_t length)
{
    if (length < 8)
        return TextureSource::Unknown;

	return TextureSource::Unknown;
}


TextureManager::~TextureManager()
{
}

void TextureManager::RegisterWAD(const char *fileName, bool shared)
{
    IFileHandle *fd = FileSystem::Instance()->OpenFileHandle(fileName);

    GoldSource::WADTexturePool *pool = new GoldSource::WADTexturePool(fd, shared);
    m_lstWADSPool.push_back(pool);
}

void TextureManager::UnregisterWAD(const char *fileName)
{
    m_lstWADSPool.remove_if([&](GoldSource::WADTexturePool * wt) {
		
		if (strcmp(fileName, wt->FileName()))
        {
            return true;
        }

		return false;
    });
}

void TextureManager::LoadTexture(GLTexture * texture, TextureSource source /*= TextureSource::GuessByItself*/)
{
    LoadTexture(texture, FallbackTexture::EmoCheckerboard, source);
}

void TextureManager::LoadTexture(GLTexture *texture, void *pixels, size_t length,
                                       TextureSource source /*= TextureSource::GuessByItself*/)
{
    LoadTexture(texture, pixels, length, FallbackTexture::EmoCheckerboard, source);
}

void TextureManager::LoadTexture(GLTexture *texture, FallbackTexture fallbackTexture,
                                       TextureSource source /*= TextureSource::GuessByItself*/)
{
    auto fs = FileSystem::Instance();
    auto fd = fs->LoadFile(texture->Name());

	if (!fd)
    {
        return Instance()->MakeFallbackTexture(texture, fallbackTexture);
    }

	if (source == TextureSource::GuessByItself)
        source = DetermineTextureSourceFromFileName(texture->Name().c_str());

	LoadTexture(texture, fd->Data(), fd->Length(), fallbackTexture, source);
	fd->UnRef();
}

void TextureManager::LoadTexture(GLTexture * texture, void *pixels, size_t length, FallbackTexture fallbackTexture,
                                       TextureSource source /*= TextureSource::GuessByItself*/)
{

	//rawpixels_t *raw_pixels = nullptr;

    switch (source)
    {
    case TextureSource::CommonImage:
        //pixels = DecodeCommonImage(pixels, length);
        break;
    case TextureSource::GoldSourceMipTexture:
		//pixels = Decode
        break;
    case TextureSource::GoldSourceWadFile:
        break;
    case TextureSource::GoldSourceSprite:
        break;
    case TextureSource::GuessByItself:
        break;
    case TextureSource::Unknown:
        break;
    default:
        break;
    
	}	
}

void TextureManager::PurgeTextures()
{
    size_t nPurged = 0;
    m_lstTexturesPool.remove_if([&](GLTexture *texture) 
		{ if (!texture->NumRef())
        {
            nPurged++;
            delete texture;
            return true;
        }
        return false;
		});

	Con_Printf("TextureManager::PurgeTextures(): purged %d textures\n", nPurged);
}

void TextureManager::MakeFallbackTexture(GLTexture *pResult, FallbackTexture fallbackTexture)
{

	GLTexture *pFallback = nullptr;

    switch (fallbackTexture)
    {
    case FallbackTexture::EmoCheckerboard:
        pFallback = m_pEmoTexture;
        break;
    case FallbackTexture::White:
        pFallback = m_pWhiteTexture;
        break;
    default:
        pFallback = m_pEmoTexture;
        break;    
	}

	pResult->m_uiGLTexnum = pFallback->m_uiGLTexnum;
    pResult->m_iWidth     = pFallback->m_iWidth;
    pResult->m_iHeight    = pFallback->m_iHeight;
}

