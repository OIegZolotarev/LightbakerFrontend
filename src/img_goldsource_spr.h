/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "spritegn.h"

RawTexture *DecodeGoldSourceSpite(byte *data, size_t length);

typedef struct  
{
    byte r, g, b;
}color24_t;


typedef struct
{
    byte r, g, b, a;
} color32_t;