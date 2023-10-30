/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include <stb/stb_image.h>
#include "gl_texture.h"
#include "application.h"
#include "loader_thread.h"

void GLReloadTexture(gltexture_t* r, FileData* sourceFile);
std::vector<gltexture_t*> g_vecGLTextures;

gltexture_t* LoadGLTexture(FileData* sourceFile, bool force)
{
	if (!force && sourceFile)
	{
		for (auto& it : g_vecGLTextures)
		{
			if (it->file_name == sourceFile->Name() 
					&& it->gl_texnum != 0)
				return it;
		}
	}

	gltexture_t* r = new gltexture_t;
	memset(r, 0, sizeof(gltexture_t));

	GLReloadTexture(r, sourceFile);
	
	g_vecGLTextures.push_back(r);

	return r;
}

gltexture_t* LoadGLTexture(const char* fileName, bool force)
{
	if (!force)
	{
		for (auto& it : g_vecGLTextures)
		{
			if (!_stricmp(it->file_name.c_str(),fileName)
				&& it->gl_texnum != 0)
				return it;
		}
	}

	FileData* pData = Application::GetFileSystem()->LoadFile(fileName);
	auto r = LoadGLTexture(pData, force);

	if (pData)
		delete pData;

	return r;
}

void GLReloadTexture(gltexture_t* t)
{
	FileData* pData = Application::GetFileSystem()->LoadFile(t->file_name.c_str());
	GLReloadTexture(t, pData);
	delete pData;
}

void GLReloadTexture(gltexture_t* r,FileData * sourceFile)
{
	r->height = -1;
	r->width = -1;
	r->gl_texnum = 0;
	

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

	r->gl_texnum = image_texture;
	r->width = image_width;
	r->height = image_height;

	r->loaded;

	return;
}

void FreeGLTexture(gltexture_t*texture)
{
// Not really needed...
// 	if (texture)
// 		glDeleteTextures(1, &texture->gl_texnum);
// 	delete texture;
}

void FreeGLTextures()
{
	ClearPointersVector(g_vecGLTextures);
}


void* AsynchTextureLoadTask::LoadTextureFileData(gltexture_t* texture)
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

	texture->width = image_width;
	texture->height = image_height;

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

gltexture_t* AsynchTextureLoadTask::ScheduleTexture(const char* fileName)
{
	gltexture_t* texture = new gltexture_t;
	memset(texture, 0, sizeof(gltexture_t));

	texture->file_name = fileName;
	m_qScheduledTextures.push(texture);

	m_nTotalSteps = m_qScheduledTextures.size();

	return texture;
}

ITaskStepResult* AsynchTextureLoadTask::ExecuteStep(LoaderThread* loaderThread)
{
	if (m_qScheduledTextures.empty())
		return new ITaskStepResult(TaskStepResultType::FinishedSuccesfully);

	gltexture_t* texture = m_qScheduledTextures.front();
	m_qScheduledTextures.pop();

	m_nPerformedSteps++;

	void* pixels = LoadTextureFileData(texture);

	return new AsynchTextureLoadResult(texture, pixels);
}

void AsynchTextureLoadTask::OnCompletion()
{
	(void)0;
}

AsynchTextureLoadResult::AsynchTextureLoadResult(gltexture_t* texture, void* pixels): ITaskStepResult(TaskStepResultType::StepPerformed)
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

	glGenTextures(1, &m_pTexture->gl_texnum);
	glBindTexture(GL_TEXTURE_2D, m_pTexture->gl_texnum);

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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_pTexture->width, m_pTexture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pPixels);
}

bool AsynchTextureLoadResult::NeedEndCallback()
{
	return true;
}
