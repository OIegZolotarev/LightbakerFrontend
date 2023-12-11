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
void VectorNormalize(glm::vec3 & vec);

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