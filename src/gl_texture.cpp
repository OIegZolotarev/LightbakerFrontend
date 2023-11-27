/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include <stb/stb_image.h>
#include "gl_texture.h"
#include "application.h"
#include "loader_thread.h"

void GLReloadTexture(GLTexture* r, FileData* sourceFile);
std::vector<GLTexture*> g_vecGLTextures;

GLTexture* LoadGLTexture(FileData* sourceFile, bool force)
{
	if (!force && sourceFile)
	{
		for (auto& it : g_vecGLTextures)
		{
			if (it->file_name == sourceFile->Name() 
					&& it->GLTextureNum() != 0)
				return it;
		}
	}

	GLTexture* r = new GLTexture;
	memset(r, 0, sizeof(GLTexture));

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
			if (!_stricmp(it->file_name.c_str(),fileName)
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
	FileData* pData = Application::GetFileSystem()->LoadFile(t->file_name.c_str());
	GLReloadTexture(t, pData);
	delete pData;
}

void GLReloadTexture(GLTexture* r,FileData * sourceFile)
{
	r->SetHeight(-1);
	r->SetWidth(-1);
	r->SetGLTextureNum(0);
	

	if (!sourceFile)
		return;

	r->file_name = sourceFile->Name();

	// Load from file
	int image_width = 0;
	int image_height = 0;
	int comps = 0;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* image_data = stbi_load_from_memory(sourceFile->Data(), sourceFile->Length(), &image_width, &image_height, &comps, 4);

	if (image_data == NULL)
		return;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Same

	

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

	stbi_image_free(image_data);

	r->SetGLTextureNum(image_texture);
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
	FileData* pData = Application::GetFileSystem()->LoadFile(texture->file_name);

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
	GLTexture* texture = new GLTexture;
	memset(texture, 0, sizeof(GLTexture));

	texture->file_name = fileName;
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

	m_strElementDescription = texture->file_name;
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

	m_pTexture->GenerateGLHandle();
    m_pTexture->Bind(0);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_pTexture->Width(), m_pTexture->Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pPixels);

	// thread-safe
	g_vecGLTextures.push_back(m_pTexture);

	stbi_image_free(m_pPixels);

}

bool AsynchTextureLoadResult::NeedEndCallback()
{
	return true;
}

bool GLTexture::IsLoaded() const
{
    return loaded;
}

void GLTexture::SetLoaded(bool val)
{
    loaded = val;
}

int GLTexture::Width() const
{
    return width;
}

void GLTexture::SetWidth(int val)
{
    width = val;
}

int GLTexture::Height() const
{
    return height;
}

void GLTexture::SetHeight(int val)
{
    height = val;
}

int GLTexture::SpriteSheetWidth() const
{
    return sprite_sheet_width;
}

void GLTexture::SetSpriteSheetWidth(int val)
{
    sprite_sheet_width = val;
}

int GLTexture::SpriteSheetHeight() const
{
    return sprite_sheet_height;
}

void GLTexture::SetSpriteSheetHeight(int val)
{
    sprite_sheet_height = val;
}

GLuint GLTexture::GLTextureNum() const
{
    return gl_texnum;
}

void GLTexture::SetGLTextureNum(GLuint val)
{
    gl_texnum = val;
}

GLTexture::~GLTexture()
{
    if (gl_texnum > 0)
        glDeleteTextures(1, &gl_texnum);
}

void GLTexture::GenerateGLHandle()
{
    glGenTextures(1, &gl_texnum);
}

void GLTexture::Bind(size_t unit)
{
    GLBackend::BindTexture(unit, this);
}
