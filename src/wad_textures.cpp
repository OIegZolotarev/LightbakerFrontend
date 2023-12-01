/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#include "application.h"
#include "goldsource_bsp_disk_structs.h"
#include "goldsource_bsp_mem_structs.h"
#include "wad_textures.h"
#include "byteorder.h"

using namespace GoldSource;

#define MAX_TEX_WIDTH 2048
#define MAX_TEX_HEIGHT 2048
#define MAX_TEX_SIZE (MAX_TEX_WIDTH * MAX_TEX_HEIGHT)

unsigned char rgba[MAX_TEX_SIZE * 4];



WADTexturePool::WADTexturePool(IFileHandle *fd, bool shared)
{
    m_pFileHandle = fd;
	
	m_pHeader               = new wadheader_t; 
	
	m_pFileHandle->Read(sizeof(wadheader_t), 1, m_pHeader);
	m_pHeader->numlumps = LittleLong(m_pHeader->numlumps);
	m_pHeader->infotableofs = LittleLong(m_pHeader->infotableofs);

	m_NumEntries = m_pHeader->numlumps;	
    m_pLumpInfo = new lumpinfo_t[m_NumEntries];
	
	m_pFileHandle->Seek(m_pHeader->infotableofs, SeekOrigin::Start);
	m_pFileHandle->Read(sizeof(lumpinfo_t), m_NumEntries, m_pLumpInfo);

	lumpinfo_t* ptr = m_pLumpInfo;


	for (int i = 0; i < m_pHeader->numlumps; i++, ptr++)
	{
		ptr->filepos = LittleLong(ptr->filepos);
		ptr->size = LittleLong(ptr->size);
	}
	
	m_bShared = shared;
}

WADTexturePool::~WADTexturePool()
{
    delete m_pFileHandle;
	delete m_pLumpInfo;
    delete m_pHeader;
}


miptex_t *WADTexturePool::LoadMipTex(const char *name)
{
	lumpinfo_t* info = FindLumpInfo(name);

	if (!info)
		return nullptr;
	
	miptex_t *texture = (miptex_t *)malloc(info->disksize);
    
	m_pFileHandle->Seek(info->filepos, SeekOrigin::Start);
	m_pFileHandle->Read(info->disksize, 1, texture);
	
	return texture;
}

lumpinfo_t* WADTexturePool::FindLumpInfo(const char* name)
{
	for (size_t i = 0; i < m_NumEntries; i++)
	{
		if (!_stricmp(m_pLumpInfo[i].name, name))
			return &m_pLumpInfo[i];
	}

	return nullptr;
}

const char* GoldSource::WADTexturePool::FileName()
{
    return m_pFileHandle->FileName();
}

WADPool* WADPool::Instance()
{
	static WADPool* sInstance = new WADPool;
	return sInstance;
}

WADPool::WADPool()
{

}

WADTexturePool* WADPool::LoadWad(const char* fileName)
{
	auto fs = FileSystem::Instance();
	auto fd = fs->LoadFile(fileName);

	if (!fd)
		return nullptr;

	// FIXME
	//WADTexturePool* result = new WADTexturePool(fd);
	//m_vecWadFiles.push_back(result);

	return nullptr;
}

WADPool::~WADPool()
{
	ClearPointersVector(m_vecWadFiles);
}

GLTexture* WADPool::LoadRawTexture(char* name)
{
    return nullptr;
}

GLTexture* GoldSource::LoadMiptex(struct miptex_s* pMipTex)
{
	if (!pMipTex->name[0]) return 0;

	if (pMipTex->offsets[0] == 0)
	{
		return 0;
	}

	GLuint index = 0;

	unsigned char* palette, * indices;
    int            size     = pMipTex->width * pMipTex->height;
	int datasize = size + (size / 4) + (size / 16) + (size / 64);
	indices = (unsigned char*)pMipTex + pMipTex->offsets[0];
	palette = indices + datasize + 2;

	int pos = 0;

	// FIXME
    GLTexture *pResult = new GLTexture(pMipTex->name, TextureSource::GoldSourceMipTexture, false);
	
	//pResult->file_name = pMipTex->name;
	pResult->SetWidth(pMipTex->width);
	pResult->SetHeight(pMipTex->height);

	pResult->GenerateGLHandle();
    pResult->Bind(0);
	

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


		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // This is required on WebGL for non power-of-two textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Same

		//GL_UploadTexture(VA("(internal)%s", pMipTex->name), GL_TEXTURE_2D, 0, GL_RGBA, pMipTex->width, pMipTex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pResult->Width(), pResult->Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
	}
	else
	{

		for (int i = 0; i < size; i++)
		{
			rgba[pos++] = palette[indices[i] * 3];
			rgba[pos++] = palette[indices[i] * 3 + 1];
			rgba[pos++] = palette[indices[i] * 3 + 2];

		}
		

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glHint(GL_GENERATE_MIPMAP_HINT, GL_NICEST);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);


		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // This is required on WebGL for non power-of-two textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Same

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pResult->Width(), pResult->Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, rgba);
	}
	return pResult;
	
}
