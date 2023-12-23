/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#define _CRT_SECURE_NO_WARNINGS

#include "application.h"
#include "goldsource_bsp_disk_structs.h"
#include "goldsource_bsp_mem_structs.h"
#include "goldsource_lightmap_atlas.h"

#define GL_LIGHTMAP_FORMAT GL_RGBA

using namespace GoldSource;

LightmapAtlas::LightmapAtlas(size_t width, size_t height)
{
	m_BlockWidth = width;
	m_BlockHeight = height;

	lightmap_buffer = new byte[LIGHTMAP_BYTES * width * height];
	allocatedMask = new int[width];

	InitBlock();

	glGenTextures(MAX_LIGHTMAPS, m_LightmapTextures);


}

LightmapAtlas::~LightmapAtlas()
{
	delete lightmap_buffer;
	delete allocatedMask;

	glDeleteTextures(4, m_LightmapTextures);
}

std::optional<glm::uvec2> LightmapAtlas::AllocBlock(const unsigned int w, const unsigned int h)
{
	int		i, j;
	int		best, best2;

	glm::uvec2 result;

	best = m_BlockWidth;

	for (i = 0; i < m_BlockWidth - w; i++)
	{
		best2 = 0;

		for (j = 0; j < w; j++)
		{
			if (allocatedMask[i + j] >= best)
				break;
			if (allocatedMask[i + j] > best2)
				best2 = allocatedMask[i + j];
		}
		if (j == w)
		{	// this is a valid spot
			result.x = i;
			result.y = best = best2;
		}
	}

	if (best + h > m_BlockHeight)
		return std::nullopt;

	for (i = 0; i < w; i++)
		allocatedMask[result.x + i] = best + h;

	return result;
}

void LightmapAtlas::UploadBlock(bool dynamic)
{
	if (m_CurrentLightmapTexture == MAX_LIGHTMAPS)
		return;
	
	int texture;
	int height = 0;

	if (dynamic)
	{
		texture = 0;
	}
	else
	{
		// TODO: review this
		texture = m_LightmapTextures[m_CurrentLightmapTexture++];
	}

	
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_LIGHTMAP_FORMAT, m_BlockWidth, m_BlockHeight, 0, GL_LIGHTMAP_FORMAT, GL_UNSIGNED_BYTE, lightmap_buffer);

	if (m_CurrentLightmapTexture == MAX_LIGHTMAPS)
		throw new std::exception("LM_UploadBlock() - MAX_LIGHTMAPS exceeded\n");

	m_CurrentLightmapTexture++;
	
// 	FILE *fp = fopen("test.raw", "wb");
//     fwrite(lightmap_buffer, m_BlockHeight * m_BlockWidth, 4, fp);
//     fclose(fp);

}

void LightmapAtlas::InitBlock()
{
	memset(allocatedMask, 0, sizeof(int) * m_BlockWidth);
}

GLuint GoldSource::LightmapAtlas::CurrentLightmapTexture()
{
	return m_LightmapTextures[m_CurrentLightmapTexture];
}

const size_t GoldSource::LightmapAtlas::BlockWidth() const
{
	return m_BlockWidth;
}

void LightmapAtlas::Reset()
{
	m_CurrentLightmapTexture = 0;
	InitBlock();
}

const size_t LightmapAtlas::BlockHeight() const
{
	return m_BlockHeight;
}

byte* LightmapAtlas::LightmapBuffer()
{
	return lightmap_buffer;
}
