/*
    LightBaker3000 Frontend project,
    (c) 2024 CrazyRussian
*/

// BVH Tree algorithm based on code from https://github.com/lohedges/aabbcc

/*
  Copyright (c) 2009 Erin Catto http://www.box2d.org
  Copyright (c) 2016-2018 Lester Hedges <lester.hedges+aabbcc@gmail.com>

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.

  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.

  3. This notice may not be removed or altered from any source distribution.

  This code was adapted from parts of the Box2D Physics Engine,
  http://www.box2d.org
*/

#pragma once

#include "mathlib.h"

class BVHBoundingBox : public BoundingBox
{
    float m_flSurfaceArea;
    glm::vec3 m_Center;

public:
    BVHBoundingBox(const glm::vec3 mins, const glm::vec3 maxs);
    BVHBoundingBox();

    float ComputeSurfaceArea() const;
    float GetSurfaceArea() const;

    void Merge(const BVHBoundingBox &aabb1, const BVHBoundingBox &aabb2);

    bool Contains(const BVHBoundingBox &aabb) const;

    bool Overlaps(const BVHBoundingBox &aabb, bool touchIsOverlap) const;
    void Inflate(float skinThickness);
    void RecalcAreaAndCenter();
};
