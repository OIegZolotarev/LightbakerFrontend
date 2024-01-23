/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once

#include "common.h"
#include "mathlib.h"

class BrushFace;

#define ON_EPSILON 0.5f
#define ROUND_VERTEX_EPSILON 0.01f

#define SIDE_FRONT 0
#define SIDE_BACK  1
#define SIDE_ON    2

#define MAX_COORD_INTEGER (16384)
#define MIN_COORD_INTEGER (-MAX_COORD_INTEGER)

#define COORD_EXTENT     (2 * MAX_COORD_INTEGER)
#define MAX_TRACE_LENGTH (1.732050f * COORD_EXTENT)
#define MAX_COORD_RANGE  (MAX_COORD_INTEGER)

#define BOGUS_RANGE (MAX_COORD_INTEGER * 4)

#define MAX_POINTS_ON_WINDING 128

class Winding
{
    std::vector<glm::vec3> m_Points;

public:
    Winding(const Winding *pOther);
    Winding(const uint32_t numPoints);
    Winding(const plane_t *planeFrom);

    ~Winding();

    void Clip(plane_t &plane);
    void Clip(BrushFace *pFace);
    void RemoveDuplicatePoints(float flMinDist);

    bool IsEmpty();
    std::vector<glm::vec3> &GetPoints();
};

#define ValidateWinding(w)                                                                                             \
    {                                                                                                                  \
        if (w->IsEmpty())                                                                                              \
        {                                                                                                              \
            delete w;                                                                                                  \
            w = nullptr;                                                                                               \
        }                                                                                                              \
    }\
