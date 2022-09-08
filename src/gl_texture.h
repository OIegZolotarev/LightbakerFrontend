#pragma once

#include "common.h"
#include "file_system.h"

typedef struct gltexture_s
{
	char file_name[128];

	int width;
	int height;

	int sprite_sheet_width;
	int sprite_sheet_height;

	GLuint gl_texnum;

}gltexture_t;

gltexture_t*	LoadGLTexture(FileData* pFileData, bool force = false);
void			FreeGLTexture(gltexture_t*);
void			FreeGLTextures();