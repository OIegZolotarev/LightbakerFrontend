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

const GoldSource::lumpinfo_t *WADTexturePool::Items() const
{
    return m_pLumpInfo;
}

const size_t WADTexturePool::NumItems() const
{
    return m_NumEntries;
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
	return nullptr;
}
