/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "r_octree.h"

void OctreeNode::Subdivide()
{
    const int halfExtents = m_Extents / 2;

    for (auto item : OctreeChildId::_values())
    {
        int id = item._value;

        auto bbox       = CalculateBoundingBox(m_Bounds, id);
        m_pChildren[id] = new OctreeNode(this, bbox, item._value, halfExtents);
    }
}

BoundingBox OctreeNode::CalculateBoundingBox(BoundingBox parent, int id)
{
    glm::vec3 ns = parent.Mins();
    glm::vec3 xs = parent.Maxs();
    glm::vec3 c  = parent.Center();

    switch (id)
    {
    case OctreeChildId::NxNyNz:
        return BoundingBox({ns.x, ns.y, ns.z}, {c.x, c.y, c.z});
    case OctreeChildId::PxNyNz:
        return BoundingBox({c.x, ns.y, ns.z}, {xs.x, c.y, c.z});
    case OctreeChildId::PxPyNz:
        return BoundingBox({c.x, c.y, ns.z}, {xs.x, xs.y, c.z});
    case OctreeChildId::NxPyNz:
        return BoundingBox({ns.x, c.y, ns.z}, {c.x, xs.y, c.z});
    case OctreeChildId::NxPyPz:
        return BoundingBox({ns.x, c.y, c.z}, {c.x, xs.y, xs.z});
    case OctreeChildId::PxPyPz:
        return BoundingBox({c.x, c.y, c.z}, {xs.x, xs.y, xs.z});
    case OctreeChildId::PxNyPz:
        return BoundingBox({c.x, ns.y, c.z}, {xs.x, c.y, xs.z});
    case OctreeChildId::NxNyPz:
        return BoundingBox({ns.x, ns.y, c.z}, {c.x, c.y, xs.z});
        break;
    }

    assert(false && "Unknown octree id!");
    return -1; // Make compiler happy
}

OctreeNode::OctreeNode(OctreeNode *pParent, BoundingBox &bounds, int id, int extents)
{
    m_Bounds  = bounds;
    m_pParent = pParent;
    m_Id      = id;
    m_Extents = extents;

    for (int i = 0; i < 8; i++)
        m_pChildren[i] = nullptr;
}

OctreeNode::OctreeNode(int extents)
{
    m_Extents = extents;
    m_Id      = -1;
    m_pParent = 0;
    m_Bounds  = BoundingBox(extents);

    for (int i = 0; i < 8; i++)
        m_pChildren[i] = nullptr;
}

OctreeNode::~OctreeNode()
{
}

void OctreeNode::PushEntity(SceneEntityWeakPtr &entity)
{
    auto ptr = entity.lock();

    if (!ptr)
        return;

    RecursivePushEntity(entity, ptr);
}

bool OctreeNode::RecursivePushEntity(SceneEntityWeakPtr &weakRef, SceneEntityPtr &ptr)
{
    int halfExtents = m_Extents / 2;

    const BoundingBox &absBox  = ptr->GetAbsoulteBoundingBox();
    
    const glm::vec3 &oMins = m_Bounds.Mins();
    const glm::vec3 &oMaxs = m_Bounds.Maxs();
    const glm::vec3 &oCenter = m_Bounds.Center();

    const glm::vec3 &eMins = absBox.Mins();
    const glm::vec3 &eMaxs = absBox.Maxs();

    bool doesntFitIntoChildren = false;

    // if any dimensions of entity comes through center of this node - we will not subdivide nodes further
    for (int i = 0; i < 3; i++)
    {
        if (oCenter[i] >= eMins[i] && oCenter[i] <= eMaxs[i])
        {
            doesntFitIntoChildren = true;
            break;
        }
    }

    if (!doesntFitIntoChildren && halfExtents > 256)
    {
        if (!m_pChildren[0])
            Subdivide();

        for (auto &child : m_pChildren)
        {
            
            if (child->RecursivePushEntity(weakRef, ptr))
            {
                m_nTotalEntities++;
                return true;
            }
        }
    }
    else
    {
        for (int i = 0; i < 3; i++)
        {
            const float oA = oMins[i];
            const float oB = oMaxs[i];

            const float eA = eMins[i];
            const float eB = eMaxs[i];

            bool aFits = (eA >= oA && eA <= oB);
            bool bFits = (eB >= oA && eB <= oB);

            if (!(aFits && bFits))
                return false;
        }

        ptr->SetOctreeNode(this);
        m_Entities.push_back(weakRef);
        return true;
    }

    return false;
}

void OctreeNode::DrawDebug(int maxRecursion) const
{
    if (m_Entities.size() > 0)
    {
        auto sr = Application::GetMainWindow()->GetSceneRenderer();

        glm::vec3 pos       = m_Bounds.Center();
        glm::vec3 localMins = m_Bounds.Mins() - pos;
        glm::vec3 localMaxs = m_Bounds.Maxs() - pos;

        sr->RenderPointEntityDefault(pos, localMins, localMaxs, {1, 0, 0}, 0);
    }


    if (m_pChildren[0] && maxRecursion > 0 || maxRecursion == -1)
    {
        for (auto &it : m_pChildren)
            it->DrawDebug(maxRecursion - 1);
    }
}
