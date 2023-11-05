/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/


// Based on Quake 1 code

/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/


#pragma once
#include "goldsource_bsp_mem_structs.h"
#include "common.h"
#include <optional>

namespace GoldSource
{

#define MAX_LIGHTMAPS 32

#define BLOCK_WIDTH 512
#define BLOCK_HEIGHT 512

#define LIGHTMAP_BYTES 4



class LightmapAtlas
{

	int internal_format;
	int	current_lightmap_texture;

	msurface_t* lightmap_surfaces[MAX_LIGHTMAPS];

	int* allocatedMask; // [BLOCK_WIDTH]
	byte* lightmap_buffer;// [LIGHTMAP_BYTES * BLOCK_WIDTH * BLOCK_HEIGHT] ;

	GLuint m_LightmapTextures[MAX_LIGHTMAPS];
	GLuint m_CurrentLightmapTexture;

	size_t m_BlockWidth;
	size_t m_BlockHeight;

public:
	LightmapAtlas(size_t width, size_t height);	
	~LightmapAtlas();

	std::optional<glm::uvec2> AllocBlock(const unsigned int w, const unsigned int h);

	void UploadBlock(bool dynamic);
	void InitBlock();
	GLuint CurrentLightmapTexture();
	byte* LightmapBuffer();
	
	size_t BlockWidth();
	size_t BlockHeight();
	void Reset();
};


}

