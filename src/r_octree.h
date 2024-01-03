/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once
#include "mathlib.h"
#include "scene_entity.h"

class Scene;

BETTER_ENUM(OctreeChildId, int, NxNyNz, PxNyNz, PxPyNz, NxPyNz, NxPyPz, PxPyPz, PxNyPz, NxNyPz);

class OctreeNode
{
    BoundingBox m_Bounds;
    int         m_Extents;

    // Hierarchy
    OctreeNode *m_pChildren[8];
    OctreeNode *m_pParent = nullptr;

    size_t                        m_nTotalEntities;
    std::list<SceneEntityWeakPtr> m_Entities;
    int                           m_Id;

    void Subdivide();

    BoundingBox CalculateBoundingBox(BoundingBox parent, int id);

public:
    OctreeNode(int extents);
    OctreeNode(OctreeNode *pParent, BoundingBox &bounds, int id, int extents);
    ~OctreeNode();

    void PushEntity(SceneEntityWeakPtr &entity);
    bool RecursivePushEntity(SceneEntityWeakPtr & weakRef, SceneEntityPtr &ptr);

    void DrawDebug(int maxRecursion = -1) const;
};