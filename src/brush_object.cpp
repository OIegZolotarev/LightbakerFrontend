/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "winding.h"
#include "gl_draw_mesh.h"
#include "brush_object.h"

void BrushObject::RemoveInvalidFaces()
{
    for (size_t i = m_lstFaces.size() - 1; i >= 0; i--)
    {
        BrushFace *    pFace = GetFace(i);
        const plane_t *f     = pFace->GetPlane();

        if (f->normal == glm::vec3(0, 0, 0))
        {
            RemoveFace(i);
            continue;
        }

        for (size_t j = 0; j < i; j++)
        {
            BrushFace *    pFaceCheck = GetFace(j);
            const plane_t *checkPlane = pFaceCheck->GetPlane();

            const glm::vec3 &f1 = f->normal;
            const glm::vec3 &f2 = checkPlane->normal;

            if ((glm::dot(f1, f2) > 0.999) && (fabs(f->dist - checkPlane->dist) < 0.01))
            {
                RemoveFace(j);
                break;
            }
        }
    }
}

bool BrushObject::CreateFacesFromPlanes(int flags)
{
    int i, j, k;
    m_bValid   = true;
    size_t nFaces = m_lstFaces.size();

    assert(nFaces < MAPSOLID_MAX_FACES);
    RemoveInvalidFaces();

    bool bGotFaces = false;

    if (m_pDrawMesh)
        delete m_pDrawMesh;

    m_pDrawMesh = new DrawMesh();
    m_pDrawMesh->Begin(GL_TRIANGLES);

    for (i = 0; i < nFaces; i++)
    {
        BrushFace *    pFace = GetFace(i);
        const plane_t *f     = pFace->GetPlane();

        Winding *w = new Winding(f);

        for (j = 0; j < nFaces && w; j++)
        {
            BrushFace *pFaceClip = GetFace(j);
            
            if (j != i)
            {
                w->Clip(pFaceClip);
                ValidateWinding(w);
            }
        }

        if (w != nullptr)
        {
            std::vector<glm::vec3> &points = w->GetPoints();
            const size_t            nPoints = points.size();

            for (j = 0; j < nPoints; j++)
            {
                for (k = 0; k < 3; k++)
                {
                    float v  = points[j][k];
                    float v1 = int(v);
                    if ((v != v1) && (fabs(v - v1) < ROUND_VERTEX_EPSILON))
                    {
                        points[j][k] = v1;
                    }
                }
            }

            w->RemoveDuplicatePoints(ON_EPSILON);
            
            bGotFaces = true;

            if (flags & CREATE_FROM_PLANES_CLIPPING)
            {
                pFace->CreateFaceFromWinding(w, CREATE_FACE_PRESERVE_PLANE | CREATE_FACE_CLIPPING);
            }
            else
            {
                pFace->CreateFaceFromWinding(w, CREATE_FACE_PRESERVE_PLANE);
            }

            delete w;
        }
    }

    m_pDrawMesh->End();

    return m_bValid;
}

 BrushObject::BrushObject(Scene *pScene) : SceneEntity(pScene)
{
}

BrushObject::~BrushObject()
{
     for (auto &it : m_lstFaces)
         delete it;

     m_lstFaces.clear();
 }

DrawMesh *BrushObject::GetDrawMesh()
{
    return m_pDrawMesh;
}

void BrushObject::AddFace(glm::vec3 pts[3])
{
    BrushFace *pNewFace = new BrushFace(this, pts);
    m_lstFaces.push_back(pNewFace);
}

void BrushObject::RemoveFace(size_t idx)
{
    auto it = m_lstFaces.begin();
    std::advance(it, idx);

    m_lstFaces.erase(it);
}

BrushFace *BrushObject::GetFace(size_t idx)
{
    auto it = m_lstFaces.begin();
    std::advance(it, idx);
    return *it;
}
