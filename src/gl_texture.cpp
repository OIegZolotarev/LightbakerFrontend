#include "..\include\ImGuiFileDialog\stb\stb_image.h"
#include "gl_texture.h"


std::vector<gltexture_t*> g_vecGLTextures;

gltexture_t* LoadGLTexture(FileData* sourceFile, bool force)
{
	if (!force)
	{
		for (auto& it : g_vecGLTextures)
		{
			if (!strcmp(it->file_name, sourceFile->Name().c_str()))
				return it;
		}
	}

	gltexture_t* r = new gltexture_t;
	memset(r, 0, sizeof(gltexture_t));

	r->height = -1;
	r->width = -1;
	r->gl_texnum = 0;

	if (!sourceFile)
		return r;

	// Load from file
	int image_width = 0;
	int image_height = 0;
	int comps = 0;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* image_data = stbi_load_from_memory(sourceFile->Data(), sourceFile->Length(), &image_width, &image_height, &comps, 4);

	if (image_data == NULL)
		return r;

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

	strcpy_s(r->file_name, sourceFile->Name().c_str());

	g_vecGLTextures.push_back(r);

	return r;
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
