/*
    Edited for LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

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

/*
============================================================================

                                        BYTE ORDER FUNCTIONS

============================================================================
*/

#ifdef _SGI_SOURCE
#define __BIG_ENDIAN__
#endif

short   BigShort(short l);
short   LittleShort(short l);
int    BigLong(int l);
int    LittleLong(int l);
float   BigFloat(float l);
float   LittleFloat(float l);

