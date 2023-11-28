/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

// WAD loader based on Half-Life SDK code

/***
*
*  Copyright (c) 1998, Valve LLC. All rights reserved.
*
*  This product contains software technology licensed from Id
*  Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
*  All Rights Reserved.
*
****/

#pragma once


namespace GoldSource
{

#define	CMP_NONE		0
#define	CMP_LZSS		1

#define	TYP_NONE		0
#define	TYP_LABEL		1
#define	TYP_LUMPY		64				// 64 + grab command number

typedef struct
{
	char		identification[4];		// should be WAD2 or 2DAW
	int			numlumps;
	int			infotableofs;
} wadheader_t;


typedef struct
{
	int			filepos;
	int			disksize;
	int			size;					// uncompressed
	char		type;
	char		compression;
	char		pad1, pad2;
	char		name[16];				// must be null terminated
} lumpinfo_t;

GLTexture* LoadMiptex(struct miptex_s* pMipTex);

class WADTexturePool
{
    IFileHandle *m_pFileHandle = nullptr;

	size_t m_NumEntries = 0;
	lumpinfo_t* m_pLumpInfo = nullptr;

	wadheader_t* m_pHeader = nullptr;

	lumpinfo_t* FindLumpInfo(const char* name);

	bool m_bShared = false;

public:
	WADTexturePool(IFileHandle* fd, bool shared = true);
	~WADTexturePool();

	GLTexture* LoadTexture(const char* name);
    const char* FileName();
};


class WADPool
{
	WADPool();

	std::vector<WADTexturePool*> m_vecWadFiles;
public:
	WADTexturePool* LoadWad(const char* fileName);
	~WADPool();

	static WADPool* Instance();
	
	GLTexture* LoadTexture(char* name);
};

}

