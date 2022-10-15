/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "..\include\ImGuiFileDialog\stb\stb_image.h"
#include "gl_texture.h"
#include "application.h"

void GLReloadTexture(gltexture_t* r, FileData* sourceFile);
std::vector<gltexture_t*> g_vecGLTextures;

gltexture_t* LoadGLTexture(FileData* sourceFile, bool force)
{
	if (!force)
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
	r->file_name = sourceFile->Name();

	if (!sourceFile)
		return;

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

	stbi_image_free(image_data);

	r->gl_texnum = image_texture;
	r->width = image_width;
	r->height = image_height;

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
