/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#define GLM_FORCE_SWIZZLE

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/mat4x4.hpp>
#include "helpers.h"

enum class PlaneTypes
{
    X = 0,
    Y,
    Z,
    AnyX,
    AnyY,
    AnyZ
};

void VectorMA(const glm::vec3 &start, const float scale, const glm::vec3 &direction, glm::vec3 &dest);
void VectorNormalize(glm::vec3 &vec);

typedef struct plane_s
{
    glm::vec3 normal;
    float     dist;

    byte type;
    byte signbits; // signx  + (signy<<1) + (signz<<2)

    void CalcSignBits();
    int BoxOnPlaneSide(const glm::vec3 & emins, const glm::vec3 & emaxs);

} plane_t;

bool PlanesGetIntersectionPoint(const plane_t *plane1, const plane_t *plane2, const plane_t* plane3, glm::vec3 &out);
float AngleMod(float val);

class BoundingBox
{
    glm::vec3 m_Mins = {0,0,0};
    glm::vec3 m_Maxs = {0, 0, 0};

public:

    BoundingBox()
    {
        
    }

    BoundingBox(const short *bspminsmaxs);
    BoundingBox(const glm::vec3 pos, const BoundingBox &other);
    BoundingBox(const glm::vec3 &mins, const glm::vec3 &maxs);
    BoundingBox(const short* mins, const short*maxs);
    BoundingBox(const float* mins, const float*maxs);
    BoundingBox(const glm::vec3 size);
    BoundingBox(const float scale);

    const glm::vec3 &Mins() const;
    const glm::vec3 &Maxs() const;

    const glm::vec3 Size() const
    {
        return m_Maxs - m_Mins;
    }

    void AddPoint(const glm::vec3 pt);
};

glm::mat4 R_RotateForEntity(glm::vec3 pos, glm::vec3 angles);