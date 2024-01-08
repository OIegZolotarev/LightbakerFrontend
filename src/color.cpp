/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/


#include "application.h"
#include "common.h"
#include "color.h"



int ToColorU32(const ColorRGBA &col)
{
    color32_t t;

    t.r = col[0] * 255;
    t.g = col[1] * 255;
    t.b = col[2] * 255;
    t.a = col[3] * 255;

    return *(int *)&t;
}
