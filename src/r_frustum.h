/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "common.h"
#include <better-enums/enum.h>
#include "r_camera_controller.h"
#include "mathlib.h"

BETTER_ENUM(FrustumPlanes, int, Right, Left, Top, Bottom, NearZ, FarZ);

enum class FrustumVisiblity
{
    None = 0,
    Partial,
    Complete
};

class Frustum
{
    plane_t m_Planes[FrustumPlanes::_size()];
    
    void ComputeFrustumCorners(glm::vec3 *bbox);
    

public:
    void SetPlane(int i, const glm::vec3 &vecNormal, float dist);
    void SetPlane(int i, const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3);
    
    void LimitFarZDist(float dist);

    void InitPerspective(class CameraController * pCamera);    
    void InitPerspective(const glm::vec3 & origin, 
                         const glm::vec3 & forward,
                         const glm::vec3 & right,
                         const glm::vec3 & up,
                         const float zNear,
                         const float zFar,
                         const float fovX,
                         const float fovY);

    bool CullBox(const glm::vec3 & mins, const glm::vec3 & maxs);
    bool CullBox(const BoundingBox & bbox);
    
    FrustumVisiblity CullBoxEx(const BoundingBox &bbox);
    FrustumVisiblity CullBoxEx2(const BoundingBox &bbox);

    void DrawDebug();
    const plane_t *GetPlane(int idx) const;

};