/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "brush_face.h"
#include "winding.h"
#include "gl_draw_mesh.h"

BrushFace::BrushFace()
{
    memset(&m_TexInfo, 0, sizeof(m_TexInfo));
    memset(&m_Plane, 0, sizeof(m_Plane));
}

BrushFace::BrushFace(BrushModel *pModel, const glm::vec3 pts[3])
{
    m_pModel = pModel;
    m_Plane.SetPoints(pts[0], pts[1], pts[2]);
}

void BrushFace::SetTexture(GLTexture *pTexture)
{
    m_pTexture = pTexture;
}

const plane_t *BrushFace::GetPlane() const
{
    return &m_Plane;
}

void BrushFace::CreateFaceFromWinding(Winding *w, int flags)
{
    assert(m_pModel);

    DrawMesh *m_pBrushMesh = m_pModel->GetDrawMesh();

    ValidateTexturingInfo();

    auto &points = w->GetPoints();

    size_t startIndex = m_pBrushMesh->CurrentElement();

    for (auto &vert : points)
    {
        // m_pVerts[i] = w->p[i];

        //
        // Generate texture coordinates.
        //
        float s = glm::dot(glm::vec3(m_TexInfo.uaxis.xyz), vert) / m_TexInfo.scale[0] + m_TexInfo.uaxis[3];
        float t = glm::dot(glm::vec3(m_TexInfo.vaxis.xyz), vert) / m_TexInfo.scale[1] + m_TexInfo.vaxis[3];

        if (m_pTexture && m_pTexture->Width())
            s = s / (float)m_pTexture->Width();
        else
            s = 0;

        if (m_pTexture && m_pTexture->Height())
            t = 1 - (t / (float)m_pTexture->Height());
        else
            t = 0;

        m_pModel->AddPointsToBounds(vert);
        m_pBrushMesh->Vertex3f(vert.x,vert.y,vert.z);
    }

    if (points.size() < 3)
    {
        // FAIL?
        return;
    }

    for (size_t i = 0 ; i < points.size() - 2 ; i++)
    {
        m_pBrushMesh->Element1i(startIndex + (i + 2));
        m_pBrushMesh->Element1i(startIndex + (i + 1));
        m_pBrushMesh->Element1i(startIndex);
    }
}

void BrushFace::ValidateTexturingInfo()
{
    if (m_TexInfo.scale[0] == 0)
    {
        // TODO: default texture scale
        m_TexInfo.scale[0] = 1;
    }

    if (m_TexInfo.scale[1] == 0)
    {
        // TODO: default texture scale
        m_TexInfo.scale[1] = 1;
    }
}
