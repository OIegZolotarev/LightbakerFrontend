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

#include "application.h"
#include "common.h"
#include "r_bvh_boundingbox.h"



 BVHBoundingBox::BVHBoundingBox(const glm::vec3 mins, const glm::vec3 maxs) : BoundingBox(mins,maxs)
{
}

 BVHBoundingBox::BVHBoundingBox()
{
}

float BVHBoundingBox::ComputeSurfaceArea() const
{
    // Sum of "area" of all the sides.
    double sum = 0;

    // General formula for one side: hold one dimension constant
    // and multiply by all the other ones.
    for (unsigned int d1 = 0; d1 < 3; d1++)
    {
        // "Area" of current side.
        double product = 1;

        for (unsigned int d2 = 0; d2 < 3; d2++)
        {
            if (d1 == d2)
                continue;

            double dx = m_Maxs[d2] - m_Mins[d2];
            product *= dx;
        }

        // Update the sum.
        sum += product;
    }

    return 2.0 * sum;
}

float BVHBoundingBox::GetSurfaceArea() const
{
    return m_flSurfaceArea;
}

void BVHBoundingBox::Merge(const BVHBoundingBox &aabb1, const BVHBoundingBox &aabb2)
{
    for (unsigned int i = 0; i < 3; i++)
    {
        m_Mins[i] = std::min(aabb1.m_Mins[i], aabb2.m_Mins[i]);
        m_Maxs[i] = std::max(aabb1.m_Maxs[i], aabb2.m_Maxs[i]);
    }

    m_flSurfaceArea = ComputeSurfaceArea();
    m_Center         = Center();
}

bool BVHBoundingBox::Contains(const BVHBoundingBox &aabb) const
{
    for (unsigned int i = 0; i < 3; i++)
    {
        if (aabb.m_Mins[i] < m_Mins[i])
            return false;
        if (aabb.m_Maxs[i] > m_Maxs[i])
            return false;
    }

    return true;
}

bool BVHBoundingBox::Overlaps(const BVHBoundingBox &aabb, bool touchIsOverlap) const
{
    bool rv = true;

    if (touchIsOverlap)
    {
        for (unsigned int i = 0; i < 3; ++i)
        {
            if (aabb.m_Maxs[i] < m_Mins[i] || aabb.m_Mins[i] > m_Maxs[i])
            {
                rv = false;
                break;
            }
        }
    }
    else
    {
        for (unsigned int i = 0; i < 3; ++i)
        {
            if (aabb.m_Maxs[i] <= m_Mins[i] || aabb.m_Mins[i] >= m_Maxs[i])
            {
                rv = false;
                break;
            }
        }
    }

    return rv;
}

void BVHBoundingBox::Inflate(float skinThickness)
{
    glm::vec3 size = Size();

    // Fatten the new AABB.
    for (unsigned int i = 0; i < 3; i++)
    {
        m_Mins[i] -= skinThickness * size[i];
        m_Maxs[i] += skinThickness * size[i];
    }
}

void BVHBoundingBox::RecalcAreaAndCenter()
{
    m_flSurfaceArea = ComputeSurfaceArea();
    m_Center        = Center();
}
