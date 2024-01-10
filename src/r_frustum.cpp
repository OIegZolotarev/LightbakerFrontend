/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "common.h"
#include "r_frustum.h"
#include "r_camera.h"

void Frustum::SetPlane(int i, const glm::vec3 &vecNormal, float dist)
{
    assert(i >= 0 && i < 6);

    m_Planes[i].normal = vecNormal;
    m_Planes[i].dist   = dist;
    m_Planes[i].type   = (int)PlaneTypes::AnyZ;

    m_Planes[i].CalcSignBits();
}

void Frustum::SetPlane(int i, const glm::vec3 p1, const glm::vec3 p2, const glm::vec3 p3)
{
    assert(i >= 0 && i < 6);

    plane_t *p = &m_Planes[i];

    glm::vec3 t1;
    glm::vec3 t2;
    glm::vec3 t3;

    for (int i = 0; i < 3; i++)
    {
        t1[i] = p1[i] - p2[i];
        t2[i] = p3[i] - p2[i];
        t3[i] = p2[i];
    }

    p->normal = glm::cross(t1, t2);
    p->normal = glm::normalize(p->normal);
    p->dist   = glm::dot(t3, p->normal);
    p->CalcSignBits();
}

void Frustum::InitPerspective(Camera *pCamera)
{
    const float aspect = pCamera->AspectRatio();

    // const float zFar  = pCamera->GetZFar();
    const float zFar  = 4096;
    const float zNear = pCamera->GetZNear();
    const float fovY  = pCamera->GetFOVY(aspect);
    const float fovX  = pCamera->getFOVX();

    const float halfVSide = zFar * tanf(fovY * .5f);
    const float halfHSide = halfVSide * aspect;

    InitPerspective(pCamera->GetOrigin(), pCamera->GetForwardVector(), pCamera->GetRightVector(),
                    pCamera->GetUpVector(), zNear, zFar, fovX, fovY);
}

void Frustum::InitPerspective(const glm::vec3 &origin, const glm::vec3 &forward, const glm::vec3 &right,
                              const glm::vec3 &up, const float zNear, const float zFar, float fovX, float fovY)
{
    float flIntercept = glm::dot(origin, forward);

    // Setup the near and far planes.
    SetPlane(FrustumPlanes::FarZ, -forward, -zFar - flIntercept);
    SetPlane(FrustumPlanes::NearZ, forward, zNear + flIntercept);

    float flTanX = tan(glm::radians(fovX * 0.5f));
    float flTanY = tan(glm::radians(fovY * 0.5f));

    glm::vec3 normalPos, normalNeg;

    // Left and right

    VectorMA(right, flTanX, forward, normalPos);
    VectorMA(normalPos, -2.0f, right, normalNeg);

    VectorNormalize(normalPos);
    VectorNormalize(normalNeg);

    SetPlane(FrustumPlanes::Left, normalPos, glm::dot(normalPos, origin));
    SetPlane(FrustumPlanes::Right, normalNeg, glm::dot(normalNeg, origin));

    // Top and bottom

    VectorMA(up, flTanY, forward, normalPos);
    VectorMA(normalPos, -2.0f, up, normalNeg);

    VectorNormalize(normalPos);
    VectorNormalize(normalNeg);

    SetPlane(FrustumPlanes::Bottom, normalPos, glm::dot(normalPos, origin));
    SetPlane(FrustumPlanes::Top, normalNeg, glm::dot(normalNeg, origin));
}

bool Frustum::CullBox(const glm::vec3 &mins, const glm::vec3 &maxs)
{
    for (auto &it : FrustumPlanes::_values())
    {
        if (m_Planes[it._to_integral()].BoxOnPlaneSide(mins, maxs) == 2)
            return true;
    }

    return false;
}

bool Frustum::CullBox(const BoundingBox &bbox)
{
    BT_PROFILE("Frustum::CullBox()");

    return CullBox(bbox.Mins(), bbox.Maxs());
}

FrustumVisiblity Frustum::CullBoxEx(const BoundingBox &bbox)
{
    int r = 0;

    const glm::vec3 &mins = bbox.Mins();
    const glm::vec3 &maxs = bbox.Maxs();

    for (auto &it : FrustumPlanes::_values())
    {
        int idx  = it._to_integral();
        int flag = m_Planes[idx].BoxOnPlaneSide(mins, maxs);

        if (flag == BPS_BACK)
            return FrustumVisiblity::None;
        else if (flag == BPS_FRONT)
            r++;
    }

    if (r == 6)
        return FrustumVisiblity::Complete;
    else
        return FrustumVisiblity::Partial;
}

// Based on void CFrustum :: DrawFrustumDebug( void )
// From PrimeXT's gl_frustum.cpp https://github.com/SNMetamorph/PrimeXT/blob/master/client/render/gl_frustum.cpp
void Frustum::DrawDebug()
{
    glm::vec3 bbox[8];

    auto shader = GLBackend::Instance()->SolidColorGeometryShader();
    shader->Bind();

    for (auto &it : shader->Uniforms())
    {
        switch (it->Kind())
        {
        case UniformKind::Color:
            it->SetFloat4({1, 1, 1, 1});
            break;
        case UniformKind::TransformMatrix:
            it->SetMat4(glm::mat4x4(1.f));
            break;
        case UniformKind::ObjectSerialNumber:
            it->SetInt(0);
            break;
        default:
            GLBackend::SetUniformValue(it);
            break;
        }
    }

    ComputeFrustumCorners(bbox);

    static DrawMesh mesh(DrawMeshFlags::Dynamic);

    mesh.Begin(GL_LINES);

    for (int i = 0; i < 2; i += 1)
    {
        mesh.Vertex3fv((float *)&bbox[i + 0]);
        mesh.Vertex3fv((float *)&bbox[i + 2]);
        mesh.Vertex3fv((float *)&bbox[i + 4]);
        mesh.Vertex3fv((float *)&bbox[i + 6]);
        mesh.Vertex3fv((float *)&bbox[i + 0]);
        mesh.Vertex3fv((float *)&bbox[i + 4]);
        mesh.Vertex3fv((float *)&bbox[i + 2]);
        mesh.Vertex3fv((float *)&bbox[i + 6]);
        mesh.Vertex3fv((float *)&bbox[i * 2 + 0]);
        mesh.Vertex3fv((float *)&bbox[i * 2 + 1]);
        mesh.Vertex3fv((float *)&bbox[i * 2 + 4]);
        mesh.Vertex3fv((float *)&bbox[i * 2 + 5]);
    }

    mesh.End();
    mesh.BindAndDraw();

    shader->Unbind();
}

void Frustum::ComputeFrustumCorners(glm::vec3 *corners)
{
    memset(corners, 0, sizeof(glm::vec3) * 8);

    PlanesGetIntersectionPoint(&m_Planes[FrustumPlanes::Left], &m_Planes[FrustumPlanes::Top],
                               &m_Planes[FrustumPlanes::FarZ], corners[0]);
    PlanesGetIntersectionPoint(&m_Planes[FrustumPlanes::Right], &m_Planes[FrustumPlanes::Top],
                               &m_Planes[FrustumPlanes::FarZ], corners[1]);
    PlanesGetIntersectionPoint(&m_Planes[FrustumPlanes::Left], &m_Planes[FrustumPlanes::Bottom],
                               &m_Planes[FrustumPlanes::FarZ], corners[2]);
    PlanesGetIntersectionPoint(&m_Planes[FrustumPlanes::Right], &m_Planes[FrustumPlanes::Bottom],
                               &m_Planes[FrustumPlanes::FarZ], corners[3]);

    PlanesGetIntersectionPoint(&m_Planes[FrustumPlanes::Left], &m_Planes[FrustumPlanes::Top],
                               &m_Planes[FrustumPlanes::NearZ], corners[4]);
    PlanesGetIntersectionPoint(&m_Planes[FrustumPlanes::Right], &m_Planes[FrustumPlanes::Top],
                               &m_Planes[FrustumPlanes::NearZ], corners[5]);
    PlanesGetIntersectionPoint(&m_Planes[FrustumPlanes::Left], &m_Planes[FrustumPlanes::Bottom],
                               &m_Planes[FrustumPlanes::NearZ], corners[6]);
    PlanesGetIntersectionPoint(&m_Planes[FrustumPlanes::Right], &m_Planes[FrustumPlanes::Bottom],
                               &m_Planes[FrustumPlanes::NearZ], corners[7]);
}
