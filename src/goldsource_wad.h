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

// wadlib.h

//
// wad reading
//

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
} wadinfo_t;


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



}