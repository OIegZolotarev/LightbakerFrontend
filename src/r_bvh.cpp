/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
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

#include "r_bvh.h"
#include "application.h"
#include <stdexcept>

const unsigned int NULL_NODE = 0xffffffff;

BVHTree::BVHTree(double m_flSkinThickness, const glm::vec3 &extents)
{
    m_uiNodeCapacity = 8192;
    m_uiRootNode     = NULL_NODE;
    m_uiNodeCount    = 0;
    m_vecNodes.resize(m_uiNodeCapacity);

    // Build a linked list for the list of free nodes.
    for (unsigned int i = 0; i < m_uiNodeCapacity - 1; i++)
    {
        m_vecNodes[i].next   = i + 1;
        m_vecNodes[i].height = -1;
    }

    m_vecNodes[m_uiNodeCapacity - 1].next   = NULL_NODE;
    m_vecNodes[m_uiNodeCapacity - 1].height = -1;

    // Assign the index of the first free node.
    m_uiFreeList = 0;
}

void BVHTree::InsertEntity(SceneEntityPtr &entity)
{
    //     Make sure the particle doesn't already exist.
    //         if (particleMap.count(particle) != 0)
    //         {
    //             throw std::invalid_argument("[ERROR]: Particle already exists in tree!");
    //         }

    // Allocate a new node for the particle.
    unsigned int node = AllocateNode();

    m_vecNodes[node].aabb = entity->GetAbsoulteBoundingBox();
    m_vecNodes[node].aabb.Inflate(m_flSkinThickness);
    m_vecNodes[node].aabb.RecalcAreaAndCenter();

    // Zero the height.
    m_vecNodes[node].height = 0;

    // Insert a new leaf into the tree.
    InsertLeaf(node);

    // Add the new particle to the map.
    m_EntitiesMap.insert(std::pair<uint32_t, uint32_t>(entity->GetSerialNumber(), node));

    // Store the particle index.
    m_vecNodes[node].entity = entity;
}

unsigned int BVHTree::nParticles()
{
    return m_EntitiesMap.size();
}

void BVHTree::RemoveEntity(SceneEntityPtr &entity)
{
    // Map iterator.
    std::unordered_map<unsigned int, unsigned int>::iterator it;

    // Find the particle.
    it = m_EntitiesMap.find(entity->GetSerialNumber());

    // The particle doesn't exist.
    if (it == m_EntitiesMap.end())
    {
        throw std::invalid_argument("[ERROR]: Invalid particle index!");
    }

    // Extract the node index.
    unsigned int node = it->second;

    // Erase the particle from the map.
    m_EntitiesMap.erase(it);

    assert(node < m_uiNodeCapacity);
    assert(m_vecNodes[node].IsLeaf());

    RemoveLeaf(node);
    FreeNode(node);
}

void BVHTree::RemoveAll()
{
    // Iterator pointing to the start of the particle map.
    std::unordered_map<unsigned int, unsigned int>::iterator it = m_EntitiesMap.begin();

    // Iterate over the map.
    while (it != m_EntitiesMap.end())
    {
        // Extract the node index.
        unsigned int node = it->second;

        assert(node < m_uiNodeCapacity);
        assert(m_vecNodes[node].IsLeaf());

        RemoveLeaf(node);
        FreeNode(node);

        it++;
    }

    // Clear the particle map.
    m_EntitiesMap.clear();
}

bool BVHTree::UpdateEntity(const uint32_t serialNumber, const BVHBoundingBox &newABSBox,
                           const bool alwaysReinsert)
{
    BT_PROFILE("BVHTree::UpdateEntity");

    // Map iterator.
    std::unordered_map<unsigned int, unsigned int>::iterator it;

    // Find the particle.
    it = m_EntitiesMap.find(serialNumber);

    // The particle doesn't exist.
    if (it == m_EntitiesMap.end())
    {
        return false;
        // FIXME
        // throw std::invalid_argument("[ERROR]:* Invalid particle index!");
    }

    // Extract the node index.
    unsigned int node = it->second;

    assert(node < m_uiNodeCapacity);
    assert(m_vecNodes[node].IsLeaf());

    // Create the new AABB.
    BVHBoundingBox aabb(newABSBox);

    // No need to update if the particle is still within its fattened AABB.
    if (!alwaysReinsert && m_vecNodes[node].aabb.Contains(aabb))
        return false;

    // Remove the current leaf.
    RemoveLeaf(node);

    aabb.Inflate(m_flSkinThickness);
    aabb.RecalcAreaAndCenter();

    // Assign the new AABB.
    m_vecNodes[node].aabb = aabb;

    // Insert a new leaf node.
    InsertLeaf(node);

    return true;
}

unsigned int BVHTree::GetHeight() const
{
    if (m_uiRootNode == NULL_NODE)
        return 0;
    return m_vecNodes[m_uiRootNode].height;
}

unsigned int BVHTree::GetNodeCount() const
{
    return m_uiNodeCount;
}

unsigned int BVHTree::ComputeMaximumBalance() const
{
    unsigned int maxBalance = 0;
    for (unsigned int i = 0; i < m_uiNodeCapacity; i++)
    {
        if (m_vecNodes[i].height <= 1)
            continue;

        assert(m_vecNodes[i].IsLeaf() == false);

        unsigned int Balance = std::abs(m_vecNodes[m_vecNodes[i].left].height - m_vecNodes[m_vecNodes[i].right].height);
        maxBalance           = std::max(maxBalance, Balance);
    }

    return maxBalance;
}

double BVHTree::ComputeSurfaceAreaRatio() const
{
    if (m_uiRootNode == NULL_NODE)
        return 0.0;

    double rootArea  = m_vecNodes[m_uiRootNode].aabb.ComputeSurfaceArea();
    double totalArea = 0.0;

    for (unsigned int i = 0; i < m_uiNodeCapacity; i++)
    {
        if (m_vecNodes[i].height < 0)
            continue;

        totalArea += m_vecNodes[i].aabb.ComputeSurfaceArea();
    }

    return totalArea / rootArea;
}

void BVHTree::Validate() const
{
#ifndef NDEBUG
    ValidateStructure(m_uiRootNode);
    ValidateMetrics(m_uiRootNode);

    unsigned int freeCount = 0;
    unsigned int freeIndex = m_uiFreeList;

    while (freeIndex != NULL_NODE)
    {
        assert(freeIndex < m_uiNodeCapacity);
        freeIndex = m_vecNodes[freeIndex].next;
        freeCount++;
    }

    assert(GetHeight() == ComputeHeight());
    assert((m_uiNodeCount + freeCount) == m_uiNodeCapacity);
#endif
}

void BVHTree::Rebuild()
{
    std::vector<unsigned int> nodeIndices(m_uiNodeCount);
    unsigned int              count = 0;

    for (unsigned int i = 0; i < m_uiNodeCapacity; i++)
    {
        // Free node.
        if (m_vecNodes[i].height < 0)
            continue;

        if (m_vecNodes[i].IsLeaf())
        {
            m_vecNodes[i].parent = NULL_NODE;
            nodeIndices[count]   = i;
            count++;
        }
        else
            FreeNode(i);
    }

    while (count > 1)
    {
        double minCost = std::numeric_limits<double>::max();
        int    iMin = -1, jMin = -1;

        for (unsigned int i = 0; i < count; i++)
        {
            BVHBoundingBox aabbi = m_vecNodes[nodeIndices[i]].aabb;

            for (unsigned int j = i + 1; j < count; j++)
            {
                BVHBoundingBox aabbj = m_vecNodes[nodeIndices[j]].aabb;
                BVHBoundingBox aabb;
                aabb.Merge(aabbi, aabbj);
                double cost = aabb.GetSurfaceArea();

                if (cost < minCost)
                {
                    iMin    = i;
                    jMin    = j;
                    minCost = cost;
                }
            }
        }

        unsigned int index1 = nodeIndices[iMin];
        unsigned int index2 = nodeIndices[jMin];

        unsigned int parent       = AllocateNode();
        m_vecNodes[parent].left   = index1;
        m_vecNodes[parent].right  = index2;
        m_vecNodes[parent].height = 1 + std::max(m_vecNodes[index1].height, m_vecNodes[index2].height);
        m_vecNodes[parent].aabb.Merge(m_vecNodes[index1].aabb, m_vecNodes[index2].aabb);
        m_vecNodes[parent].parent = NULL_NODE;

        m_vecNodes[index1].parent = parent;
        m_vecNodes[index2].parent = parent;

        nodeIndices[jMin] = nodeIndices[count - 1];
        nodeIndices[iMin] = parent;
        count--;
    }

    m_uiRootNode = nodeIndices[0];

    Validate();
}

int BVHTree::RootNode()
{
    return m_uiRootNode;
}

void BVHTree::DebugRender(int node)
{
    if (node == NULL_NODE)
        return;

    BVHNode *nodePtr = &m_vecNodes[node];
    BVHNode *sibling = nodePtr;

    auto sr = Application::GetMainWindow()->GetSceneRenderer();

    glm::vec3   pos = sibling->aabb.Center();
    BoundingBox rel = sibling->aabb.ConvertToRelative();

    sr->RenderPointEntityDefault(pos, rel.Mins(), rel.Maxs(), {1, 0, 0}, 0);
}

int nodeToRender = NULL_NODE;

void BVHTree::DebugRenderTreeUINode(int node)
{
    BVHNode *ptr = &m_vecNodes[node];

    // bool opened = TreeNode(id, "##nolabel);

    if (ptr->IsLeaf())
    {
        if (ImGui::TreeNodeEx(ptr, ImGuiTreeNodeFlags_Leaf, "%s", ptr->entity->GetClassName().c_str()))
        {
            ImGui::TreePop();
        }
    }
    else
    {
        if (ImGui::TreeNode(ptr, "Node %d", node))
        {
            if (ImGui::IsItemActive())
                nodeToRender = node;

            if (ptr->left != NULL_NODE)
                DebugRenderTreeUINode(ptr->left);

            if (ptr->right != NULL_NODE)
                DebugRenderTreeUINode(ptr->right);

            ImGui::TreePop();
        }
    }
}

void BVHTree::DebugRenderTreeUI()
{
    ImGui::Begin("Scene BVH");

    if (m_uiRootNode != NULL_NODE)
        DebugRenderTreeUINode(m_uiRootNode);

    ImGui::End();
}

unsigned int BVHTree::AllocateNode()
{
    // Expand the node pool as needed.
    if (m_uiFreeList == NULL_NODE)
    {
        assert(m_uiNodeCount == m_uiNodeCapacity);

        // The free list is empty. Rebuild a bigger pool.
        m_uiNodeCapacity *= 2;
        m_vecNodes.resize(m_uiNodeCapacity);

        // Build a linked list for the list of free nodes.
        for (unsigned int i = m_uiNodeCount; i < m_uiNodeCapacity - 1; i++)
        {
            m_vecNodes[i].next   = i + 1;
            m_vecNodes[i].height = -1;
        }
        m_vecNodes[m_uiNodeCapacity - 1].next   = NULL_NODE;
        m_vecNodes[m_uiNodeCapacity - 1].height = -1;

        // Assign the index of the first free node.
        m_uiFreeList = m_uiNodeCount;
    }

    // Peel a node off the free list.
    unsigned int node       = m_uiFreeList;
    m_uiFreeList            = m_vecNodes[node].next;
    m_vecNodes[node].parent = NULL_NODE;
    m_vecNodes[node].left   = NULL_NODE;
    m_vecNodes[node].right  = NULL_NODE;
    m_vecNodes[node].height = 0;
    m_uiNodeCount++;

    return node;
}

void BVHTree::FreeNode(unsigned int node)
{
    assert(node < m_uiNodeCapacity);
    assert(0 < m_uiNodeCount);

    m_vecNodes[node].next   = m_uiFreeList;
    m_vecNodes[node].height = -1;
    m_uiFreeList            = node;
    m_uiNodeCount--;
}

void BVHTree::InsertLeaf(unsigned int leaf)
{
    if (m_uiRootNode == NULL_NODE)
    {
        m_uiRootNode                    = leaf;
        m_vecNodes[m_uiRootNode].parent = NULL_NODE;
        return;
    }

    // Find the best sibling for the node.

    BVHBoundingBox leafAABB = m_vecNodes[leaf].aabb;
    unsigned int   index    = m_uiRootNode;

    while (!m_vecNodes[index].IsLeaf())
    {
        // Extract the children of the node.
        unsigned int left  = m_vecNodes[index].left;
        unsigned int right = m_vecNodes[index].right;

        double surfaceArea = m_vecNodes[index].aabb.GetSurfaceArea();

        BVHBoundingBox combinedAABB;
        combinedAABB.Merge(m_vecNodes[index].aabb, leafAABB);
        double combinedSurfaceArea = combinedAABB.GetSurfaceArea();

        // Cost of creating a new parent for this node and the new leaf.
        double cost = 2.0 * combinedSurfaceArea;

        // Minimum cost of pushing the leaf further down the tree.
        double inheritanceCost = 2.0 * (combinedSurfaceArea - surfaceArea);

        // Cost of descending to the left.
        double costLeft;
        if (m_vecNodes[left].IsLeaf())
        {
            BVHBoundingBox aabb;
            aabb.Merge(leafAABB, m_vecNodes[left].aabb);
            costLeft = aabb.GetSurfaceArea() + inheritanceCost;
        }
        else
        {
            BVHBoundingBox aabb;
            aabb.Merge(leafAABB, m_vecNodes[left].aabb);
            double oldArea = m_vecNodes[left].aabb.GetSurfaceArea();
            double newArea = aabb.GetSurfaceArea();
            costLeft       = (newArea - oldArea) + inheritanceCost;
        }

        // Cost of descending to the right.
        double costRight;
        if (m_vecNodes[right].IsLeaf())
        {
            BVHBoundingBox aabb;
            aabb.Merge(leafAABB, m_vecNodes[right].aabb);
            costRight = aabb.GetSurfaceArea() + inheritanceCost;
        }
        else
        {
            BVHBoundingBox aabb;
            aabb.Merge(leafAABB, m_vecNodes[right].aabb);
            double oldArea = m_vecNodes[right].aabb.GetSurfaceArea();
            double newArea = aabb.GetSurfaceArea();
            costRight      = (newArea - oldArea) + inheritanceCost;
        }

        // Descend according to the minimum cost.
        if ((cost < costLeft) && (cost < costRight))
            break;

        // Descend.
        if (costLeft < costRight)
            index = left;
        else
            index = right;
    }

    unsigned int sibling = index;

    // Create a new parent.
    unsigned int oldParent       = m_vecNodes[sibling].parent;
    unsigned int newParent       = AllocateNode();
    m_vecNodes[newParent].parent = oldParent;
    m_vecNodes[newParent].aabb.Merge(leafAABB, m_vecNodes[sibling].aabb);
    m_vecNodes[newParent].height = m_vecNodes[sibling].height + 1;

    // The sibling was not the root.
    if (oldParent != NULL_NODE)
    {
        if (m_vecNodes[oldParent].left == sibling)
            m_vecNodes[oldParent].left = newParent;
        else
            m_vecNodes[oldParent].right = newParent;

        m_vecNodes[newParent].left  = sibling;
        m_vecNodes[newParent].right = leaf;
        m_vecNodes[sibling].parent  = newParent;
        m_vecNodes[leaf].parent     = newParent;
    }
    // The sibling was the root.
    else
    {
        m_vecNodes[newParent].left  = sibling;
        m_vecNodes[newParent].right = leaf;
        m_vecNodes[sibling].parent  = newParent;
        m_vecNodes[leaf].parent     = newParent;
        m_uiRootNode                = newParent;
    }

    // Walk back up the tree fixing heights and AABBs.
    index = m_vecNodes[leaf].parent;
    while (index != NULL_NODE)
    {
        index = Balance(index);

        unsigned int left  = m_vecNodes[index].left;
        unsigned int right = m_vecNodes[index].right;

        assert(left != NULL_NODE);
        assert(right != NULL_NODE);

        m_vecNodes[index].height = 1 + std::max(m_vecNodes[left].height, m_vecNodes[right].height);
        m_vecNodes[index].aabb.Merge(m_vecNodes[left].aabb, m_vecNodes[right].aabb);

        index = m_vecNodes[index].parent;
    }
}

void BVHTree::RemoveLeaf(unsigned int leaf)
{
    if (leaf == m_uiRootNode)
    {
        m_uiRootNode = NULL_NODE;
        return;
    }

    unsigned int parent      = m_vecNodes[leaf].parent;
    unsigned int grandParent = m_vecNodes[parent].parent;
    unsigned int sibling;

    if (m_vecNodes[parent].left == leaf)
        sibling = m_vecNodes[parent].right;
    else
        sibling = m_vecNodes[parent].left;

    // Destroy the parent and connect the sibling to the grandparent.
    if (grandParent != NULL_NODE)
    {
        if (m_vecNodes[grandParent].left == parent)
            m_vecNodes[grandParent].left = sibling;
        else
            m_vecNodes[grandParent].right = sibling;

        m_vecNodes[sibling].parent = grandParent;
        FreeNode(parent);

        // Adjust ancestor bounds.
        unsigned int index = grandParent;
        while (index != NULL_NODE)
        {
            index = Balance(index);

            unsigned int left  = m_vecNodes[index].left;
            unsigned int right = m_vecNodes[index].right;

            m_vecNodes[index].aabb.Merge(m_vecNodes[left].aabb, m_vecNodes[right].aabb);
            m_vecNodes[index].height = 1 + std::max(m_vecNodes[left].height, m_vecNodes[right].height);

            index = m_vecNodes[index].parent;
        }
    }
    else
    {
        m_uiRootNode               = sibling;
        m_vecNodes[sibling].parent = NULL_NODE;
        FreeNode(parent);
    }
}

unsigned int BVHTree::Balance(unsigned int node)
{
    assert(node != NULL_NODE);

    if (m_vecNodes[node].IsLeaf() || (m_vecNodes[node].height < 2))
        return node;

    unsigned int left  = m_vecNodes[node].left;
    unsigned int right = m_vecNodes[node].right;

    assert(left < m_uiNodeCapacity);
    assert(right < m_uiNodeCapacity);

    int currentBalance = m_vecNodes[right].height - m_vecNodes[left].height;

    // Rotate right branch up.
    if (currentBalance > 1)
    {
        unsigned int rightLeft  = m_vecNodes[right].left;
        unsigned int rightRight = m_vecNodes[right].right;

        assert(rightLeft < m_uiNodeCapacity);
        assert(rightRight < m_uiNodeCapacity);

        // Swap node and its right-hand child.
        m_vecNodes[right].left   = node;
        m_vecNodes[right].parent = m_vecNodes[node].parent;
        m_vecNodes[node].parent  = right;

        // The node's old parent should now point to its right-hand child.
        if (m_vecNodes[right].parent != NULL_NODE)
        {
            if (m_vecNodes[m_vecNodes[right].parent].left == node)
                m_vecNodes[m_vecNodes[right].parent].left = right;
            else
            {
                assert(m_vecNodes[m_vecNodes[right].parent].right == node);
                m_vecNodes[m_vecNodes[right].parent].right = right;
            }
        }
        else
            m_uiRootNode = right;

        // Rotate.
        if (m_vecNodes[rightLeft].height > m_vecNodes[rightRight].height)
        {
            m_vecNodes[right].right       = rightLeft;
            m_vecNodes[node].right        = rightRight;
            m_vecNodes[rightRight].parent = node;
            m_vecNodes[node].aabb.Merge(m_vecNodes[left].aabb, m_vecNodes[rightRight].aabb);
            m_vecNodes[right].aabb.Merge(m_vecNodes[node].aabb, m_vecNodes[rightLeft].aabb);

            m_vecNodes[node].height  = 1 + std::max(m_vecNodes[left].height, m_vecNodes[rightRight].height);
            m_vecNodes[right].height = 1 + std::max(m_vecNodes[node].height, m_vecNodes[rightLeft].height);
        }
        else
        {
            m_vecNodes[right].right      = rightRight;
            m_vecNodes[node].right       = rightLeft;
            m_vecNodes[rightLeft].parent = node;
            m_vecNodes[node].aabb.Merge(m_vecNodes[left].aabb, m_vecNodes[rightLeft].aabb);
            m_vecNodes[right].aabb.Merge(m_vecNodes[node].aabb, m_vecNodes[rightRight].aabb);

            m_vecNodes[node].height  = 1 + std::max(m_vecNodes[left].height, m_vecNodes[rightLeft].height);
            m_vecNodes[right].height = 1 + std::max(m_vecNodes[node].height, m_vecNodes[rightRight].height);
        }

        return right;
    }

    // Rotate left branch up.
    if (currentBalance < -1)
    {
        unsigned int leftLeft  = m_vecNodes[left].left;
        unsigned int leftRight = m_vecNodes[left].right;

        assert(leftLeft < m_uiNodeCapacity);
        assert(leftRight < m_uiNodeCapacity);

        // Swap node and its left-hand child.
        m_vecNodes[left].left   = node;
        m_vecNodes[left].parent = m_vecNodes[node].parent;
        m_vecNodes[node].parent = left;

        // The node's old parent should now point to its left-hand child.
        if (m_vecNodes[left].parent != NULL_NODE)
        {
            if (m_vecNodes[m_vecNodes[left].parent].left == node)
                m_vecNodes[m_vecNodes[left].parent].left = left;
            else
            {
                assert(m_vecNodes[m_vecNodes[left].parent].right == node);
                m_vecNodes[m_vecNodes[left].parent].right = left;
            }
        }
        else
            m_uiRootNode = left;

        // Rotate.
        if (m_vecNodes[leftLeft].height > m_vecNodes[leftRight].height)
        {
            m_vecNodes[left].right       = leftLeft;
            m_vecNodes[node].left        = leftRight;
            m_vecNodes[leftRight].parent = node;
            m_vecNodes[node].aabb.Merge(m_vecNodes[right].aabb, m_vecNodes[leftRight].aabb);
            m_vecNodes[left].aabb.Merge(m_vecNodes[node].aabb, m_vecNodes[leftLeft].aabb);

            m_vecNodes[node].height = 1 + std::max(m_vecNodes[right].height, m_vecNodes[leftRight].height);
            m_vecNodes[left].height = 1 + std::max(m_vecNodes[node].height, m_vecNodes[leftLeft].height);
        }
        else
        {
            m_vecNodes[left].right      = leftRight;
            m_vecNodes[node].left       = leftLeft;
            m_vecNodes[leftLeft].parent = node;
            m_vecNodes[node].aabb.Merge(m_vecNodes[right].aabb, m_vecNodes[leftLeft].aabb);
            m_vecNodes[left].aabb.Merge(m_vecNodes[node].aabb, m_vecNodes[leftRight].aabb);

            m_vecNodes[node].height = 1 + std::max(m_vecNodes[right].height, m_vecNodes[leftLeft].height);
            m_vecNodes[left].height = 1 + std::max(m_vecNodes[node].height, m_vecNodes[leftRight].height);
        }

        return left;
    }

    return node;
}

unsigned int BVHTree::ComputeHeight() const
{
    return ComputeHeight(m_uiRootNode);
}

unsigned int BVHTree::ComputeHeight(unsigned int node) const
{
    assert(node < m_uiNodeCapacity);

    if (m_vecNodes[node].IsLeaf())
        return 0;

    unsigned int height1 = ComputeHeight(m_vecNodes[node].left);
    unsigned int height2 = ComputeHeight(m_vecNodes[node].right);

    return 1 + std::max(height1, height2);
}

void BVHTree::ValidateStructure(unsigned int node) const
{
    if (node == NULL_NODE)
        return;

    if (node == m_uiRootNode)
        assert(m_vecNodes[node].parent == NULL_NODE);

    unsigned int left  = m_vecNodes[node].left;
    unsigned int right = m_vecNodes[node].right;

    if (m_vecNodes[node].IsLeaf())
    {
        assert(left == NULL_NODE);
        assert(right == NULL_NODE);
        assert(m_vecNodes[node].height == 0);
        return;
    }

    assert(left < m_uiNodeCapacity);
    assert(right < m_uiNodeCapacity);

    assert(m_vecNodes[left].parent == node);
    assert(m_vecNodes[right].parent == node);

    ValidateStructure(left);
    ValidateStructure(right);
}

void BVHTree::ValidateMetrics(unsigned int node) const
{
    if (node == NULL_NODE)
        return;

    unsigned int left  = m_vecNodes[node].left;
    unsigned int right = m_vecNodes[node].right;

    if (m_vecNodes[node].IsLeaf())
    {
        assert(left == NULL_NODE);
        assert(right == NULL_NODE);
        assert(m_vecNodes[node].height == 0);
        return;
    }

    assert(left < m_uiNodeCapacity);
    assert(right < m_uiNodeCapacity);

    int height1 = m_vecNodes[left].height;
    int height2 = m_vecNodes[right].height;
    int height  = 1 + std::max(height1, height2);
    (void)height; // Unused variable in Release build
    assert(m_vecNodes[node].height == height);

    BVHBoundingBox aabb;
    aabb.Merge(m_vecNodes[left].aabb, m_vecNodes[right].aabb);

    for (unsigned int i = 0; i < 3; i++)
    {
        assert(aabb.Mins()[i] == m_vecNodes[node].aabb.Mins()[i]);
        assert(aabb.Maxs()[i] == m_vecNodes[node].aabb.Maxs()[i]);
    }

    ValidateMetrics(left);
    ValidateMetrics(right);
}

BVHNode *BVHTree::GetRootNode()
{
    if (m_uiRootNode == NULL_NODE)
        return nullptr;

    return &m_vecNodes[m_uiRootNode];
}

BVHNode *BVHTree::GetNodePtr(unsigned int idx)
{
    if (idx == NULL_NODE)
        return nullptr;

    return &m_vecNodes[idx];
}

BVHNode::BVHNode()
{
}

bool BVHNode::IsLeaf() const
{
    return (left == NULL_NODE);
}
