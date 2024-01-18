/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once

#include "common.h"
#include "brush_face.h"
#include "scene_entity.h"

class DrawMesh;
class BrushFace;

//
// Flags for CreateFromPlanes:
//
#define CREATE_BUILD_PLANE_POINTS   (1 << 0) // Whether to regenerate the 3 plane points from the generated face points.
#define CREATE_FROM_PLANES_CLIPPING (1 << 1)

class BrushObject : public SceneEntity
{
    std::list<BrushFace *> m_lstFaces;

    bool m_bValid;

    // Faces helpers
    void       RemoveFace(size_t idx);
    BrushFace *GetFace(size_t idx);

    void RemoveInvalidFaces();

    DrawMesh *m_pDrawMesh;

public:
    BrushObject(Scene * pScene );
    ~BrushObject();

    DrawMesh *GetDrawMesh();

    void AddFace(glm::vec3 pts[3]);
    bool CreateFacesFromPlanes(int flags);
};