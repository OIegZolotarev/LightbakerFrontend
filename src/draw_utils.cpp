/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/

#include "draw_utils.h"
#include "Icosphere.h"

DrawMesh *DrawUtils::MakeWireframeBox(glm::vec3 extents)
{
    DrawMesh *pResult = new DrawMesh(DrawMeshFlags::NoColor | DrawMeshFlags::NoUV);

    glm::vec3 max = extents * 0.5f;
    glm::vec3 min = extents * -0.5f;

    pResult->Begin(GL_LINES);

    // Bottom plane

    pResult->Vertex3f(min.x, min.y, min.z);
    pResult->Vertex3f(max.x, min.y, min.z);

    pResult->Vertex3f(max.x, min.y, min.z);
    pResult->Vertex3f(max.x, max.y, min.z);

    pResult->Vertex3f(max.x, max.y, min.z);
    pResult->Vertex3f(min.x, max.y, min.z);

    pResult->Vertex3f(min.x, max.y, min.z);
    pResult->Vertex3f(min.x, min.y, min.z);

    // Top plane

    pResult->Vertex3f(min.x, min.y, max.z);
    pResult->Vertex3f(max.x, min.y, max.z);

    pResult->Vertex3f(max.x, min.y, max.z);
    pResult->Vertex3f(max.x, max.y, max.z);

    pResult->Vertex3f(max.x, max.y, max.z);
    pResult->Vertex3f(min.x, max.y, max.z);

    pResult->Vertex3f(min.x, max.y, max.z);
    pResult->Vertex3f(min.x, min.y, max.z);

    // Sides

    pResult->Vertex3f(min.x, min.y, max.z);
    pResult->Vertex3f(min.x, min.y, min.z);

    pResult->Vertex3f(max.x, min.y, max.z);
    pResult->Vertex3f(max.x, min.y, min.z);

    pResult->Vertex3f(max.x, max.y, max.z);
    pResult->Vertex3f(max.x, max.y, min.z);

    pResult->Vertex3f(min.x, max.y, max.z);
    pResult->Vertex3f(min.x, max.y, min.z);

    pResult->End();

    return pResult;
}

DrawMesh *DrawUtils::MakeWireframeSphere(float detail, float r)
{
    DrawMesh *pResult = new DrawMesh(DrawMeshFlags::NoColor | DrawMeshFlags::NoUV);

    pResult->Begin(GL_LINE_STRIP);

    // Horizontal plane

    int step = 360 / detail;

    int i = 0;

    constexpr float pi  = glm::pi<float>();
    constexpr float pi2 = glm::pi<float>() / 2;

    float num_stacks = detail;
    float stackStep  = pi / num_stacks;

    for (int i = 0; i <= num_stacks; i++)
    {
        float stackAngle = pi2 - i * stackStep;
        float z          = r * sinf(stackAngle);
        float c          = r * cosf(stackAngle);

        for (int j = 0; j <= 360; j += step)
        {
            float a = (j / 360.f) * 6.28f;

            glm::vec3 pt;
            pt.x = cos(a) * c;
            pt.z = sin(a) * c;
            pt.y = z;

            pResult->Vertex3f(pt.x, pt.y, pt.z);
        }
    }

    pResult->End();

    return pResult;
}

DrawMesh *DrawUtils::MakeWireframeCone(float detail /*= 36*/, float r /*= 1*/, float h /*= 1*/)
{
    DrawMesh *pResult = new DrawMesh(DrawMeshFlags::NoColor | DrawMeshFlags::NoUV);

    pResult->Begin(GL_LINES);

    int step = 360 / detail;

    int i = 0;

    for (i = step; i <= 360; i += step)
    {
        float a = (i / 360.f) * 6.28f;
        float b = ((i - step) / 360.f) * 6.28f;

        glm::vec3 ptA, ptB;

        pResult->TexCoord2f(1, 1);

        ptA.x = cos(a) * r / 2;
        ptA.y = sin(a) * r / 2;
        ptA.z = -h;

        ptB.x = cos(b) * r / 2;
        ptB.y = sin(b) * r / 2;
        ptB.z = -h;

        // Расширяемый сегмент

        pResult->TexCoord2f(1, 1);
        pResult->Vertex3f(ptA.x, ptA.y, ptA.z);
        pResult->Vertex3f(ptB.x, ptB.y, ptB.z);

        // Линия 1
        pResult->TexCoord2f(1, 1);
        pResult->Vertex3f(ptA.x, ptA.y, ptA.z);

        pResult->TexCoord2f(0, 0);
        pResult->Vertex3f(0, 0, 0);

        // Линия 2
        pResult->TexCoord2f(1, 1);
        pResult->Vertex3f(ptB.x, ptB.y, ptB.z);

        pResult->TexCoord2f(0, 0);
        pResult->Vertex3f(0, 0, 0);
    }

    pResult->End();

    return pResult;
}

DrawMesh *DrawUtils::MakeIcosphere(int detailLevel)
{
    // TODO: rewrite using indices
    Icosphere s(1, detailLevel);

    DrawMesh *pResult = new DrawMesh(DrawMeshFlags::NoColor | DrawMeshFlags::NoUV);

    pResult->Begin(GL_TRIANGLES);

    typedef struct icoVert_s
    {
        float xyz[3];
        float normal[3];
        float uv[2];
    } icoVert_t;

    icoVert_t *data = (icoVert_t *)s.getInterleavedVertices();
    size_t count    = s.getInterleavedVertexCount();

    auto indices  = s.getIndices();
    auto indCount = s.getIndexCount();

    for (int i = 0; i < indCount; i++)
    {
        auto *v = &data[i];

        pResult->Normal3fv(v->normal);
        pResult->Vertex3fv(v->xyz);
    }

    pResult->End();

    return pResult;
}

DrawMesh *DrawUtils::MakeCube(float size)
{
    DrawMesh *pResult = new DrawMesh;

    float hs = size / 2;

    glm::vec3 verts[] = {{hs, hs, -hs},  {hs, -hs, -hs},  {hs, hs, hs},  {hs, -hs, hs},
                         {-hs, hs, -hs}, {-hs, -hs, -hs}, {-hs, hs, hs}, {-hs, -hs, hs}};

    int indices[] = {4, 2, 0, 2, 7, 3, 6, 5, 7, 1, 7, 5, 0, 3, 1, 4, 1, 5,
                   4, 6, 2, 2, 6, 7, 6, 4, 5, 1, 3, 7, 0, 2, 3, 4, 0, 1};

    pResult->Begin(GL_TRIANGLES);

    for (auto vert : verts)
        pResult->Vertex3fv((float *)&vert);

    pResult->Element1iv(indices, ARRAYSIZE(indices));
    pResult->End();

    return pResult;
}
