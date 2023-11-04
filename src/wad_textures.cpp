/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#include "application.h"
#include "goldsource_bsp_disk_structs.h"
#include "goldsource_bsp_mem_structs.h"
#include "wad_textures.h"

using namespace GoldSource;

#define MAX_TEX_WIDTH 2048
#define MAX_TEX_HEIGHT 2048
#define MAX_TEX_SIZE (MAX_TEX_WIDTH * MAX_TEX_HEIGHT)

unsigned char rgba[MAX_TEX_SIZE * 4];


int GoldSource::LoadMiptex(GoldSource::miptex_t* pMipTex)
{
	if (!pMipTex->name[0]) return 0;

	if (pMipTex->offsets[0] == 0)
	{
		return 0;
	}

	GLuint index = 0;

	unsigned char* palette, * indices;
	int    size = pMipTex->width * pMipTex->height;
	int datasize = size + (size / 4) + (size / 16) + (size / 64);
	indices = (unsigned char*)pMipTex + pMipTex->offsets[0];
	palette = indices + datasize + 2;

	int pos = 0;

	// convert the mip palette based bitmap to RGB format...
	if (pMipTex->name[0] == '{')
	{
		unsigned char r, g, b, a;
		
		pos = 0;
		for (int i = 0; i < size; i++)
		{
			r = palette[indices[i] * 3];
			g = palette[indices[i] * 3 + 1];
			b = palette[indices[i] * 3 + 2];
			a = 255;

			if ((r == 0) && (g == 0) && (b == 255))
			{
				r = 0; b = 0; g = 0; a = 0;
			}

			rgba[pos++] = r;
			rgba[pos++] = g;
			rgba[pos++] = b;
			rgba[pos++] = a;
		}

		glBindTexture(GL_TEXTURE_2D, index);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			
		//GL_UploadTexture(VA("(internal)%s", pMipTex->name), GL_TEXTURE_2D, 0, GL_RGBA, pMipTex->width, pMipTex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
	}
	else
	{

		for (int i = 0; i < size; i++)
		{
			rgba[pos++] = palette[indices[i] * 3];
			rgba[pos++] = palette[indices[i] * 3 + 1];
			rgba[pos++] = palette[indices[i] * 3 + 2];
		}
		glBindTexture(GL_TEXTURE_2D, index);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		//GL_UploadTexture(VA("(internal)%s", pMipTex->name), GL_TEXTURE_2D, 0, GL_RGB, pMipTex->width, pMipTex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgba);
		//	glLoadEmo(index);
	}

	
}
