/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

#include "common.h"
#include "file_system.h"

typedef struct gltexture_s
{	
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