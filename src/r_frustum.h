/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "common.h"
#include <better-enums/enum.h>
#include "r_camera.h"
#include "mathlib.h"

BETTER_ENUM(FrustumPlanes, int, Right, Left, Top, Bottom, NearZ, FarZ);

class Frustum
{
    plane_t m_Planes[FrustumPlanes::_size()];
    
    void SetPlane(int i, const glm::vec3 &vecNormal, float dist);
public:
    void InitPerspective(class Camera * pCamera);
    
    void InitPerspective(const glm::vec3 & origin, 
                         const glm::vec3 & forward,
                         const glm::vec3 & right,
                         const glm::vec3 & up,
                         const float zNear,
                         const float zFar,
                         const float fovX,
                         const float fovY);

    bool CullBox(const glm::vec3 & mins, const glm::vec3 & maxs);
    

    void DrawDebug();

private:
    void ComputeFrustumCorners(glm::vec3 *bbox);
};