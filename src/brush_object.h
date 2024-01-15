/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once

#include "common.h"
#include "brush_face.h"

class BrushObject
{
    std::list<BrushFace *> m_lstFaces;

public:
    BrushObject();
    ~BrushObject();
};