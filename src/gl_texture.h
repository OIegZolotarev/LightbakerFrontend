/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

#include "common.h"
#include "file_system.h"
#include "loader_thread.h"

typedef struct gltexture_s
{	
	bool loaded = false;
	std::string file_name;

	int width;
	int height;

	int sprite_sheet_width;
	int sprite_sheet_height;

	GLuint gl_texnum;
}gltexture_t;

gltexture_t*	LoadGLTexture(const char* fileName, bool force = false);
gltexture_t*	LoadGLTexture(FileData* pFileData, bool force = false);

void			GLReloadTexture(gltexture_t*);
void			FreeGLTexture(gltexture_t*);
void			FreeGLTextures();

class AsynchTextureLoadResult: public ITaskStepResult
{
	void* m_pPixels = nullptr;
	gltexture_t* m_pTexture = nullptr;

public:	
	AsynchTextureLoadResult(gltexture_t* texture, void* pixels);
	~AsynchTextureLoadResult();

	void ExecuteOnCompletion() override;
	bool NeedEndCallback() override;
};

class AsynchTextureLoadTask: public ITask
{	
	std::queue<gltexture_t*> m_qScheduledTextures;
	
	void* LoadTextureFileData(gltexture_t* texture);

	void UploadPixels();

public:
	AsynchTextureLoadTask(const char* setDescription = nullptr);
	~AsynchTextureLoadTask();

	gltexture_t* ScheduleTexture(const char* fileName);
	
	ITaskStepResult* ExecuteStep(LoaderThread* loaderThread) override;
	void OnCompletion() override;
};

//gltexture_t* LoadGLTextureAsynch(const char* fileName);