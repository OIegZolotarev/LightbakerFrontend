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
#include <stdexcept>

const unsigned int NULL_NODE = 0xffffffff;

 BVHTree::BVHTree(double skinThickness, const glm::vec3 &extents)
{
    // Initialise the tree.
    root         = NULL_NODE;
    nodeCount    = 0;    
    nodes.resize(nodeCapacity);

    // Build a linked list for the list of free nodes.
    for (unsigned int i = 0; i < nodeCapacity - 1; i++)
    {
        nodes[i].next   = i + 1;
        nodes[i].height = -1;
    }

    nodes[nodeCapacity - 1].next   = NULL_NODE;
    nodes[nodeCapacity - 1].height = -1;

    // Assign the index of the first free node.
    freeList = 0;
}

void BVHTree::insertParticle(SceneEntityWeakPtr &entity, const glm::vec3 &mins, const glm::vec3 &maxs)
{
//     Make sure the particle doesn't already exist.
//         if (particleMap.count(particle) != 0)
//         {
//             throw std::invalid_argument("[ERROR]: Particle already exists in tree!");
//         }

    // Allocate a new node for the particle.
    unsigned int node = allocateNode();

    nodes[node].aabb = BVHBoundingBox(mins, maxs);
    nodes[node].aabb.Inflate(skinThickness);
    nodes[node].aabb.RecalcAreaAndCenter();

    // Zero the height.
    nodes[node].height = 0;

    // Insert a new leaf into the tree.
    insertLeaf(node);

    // Add the new particle to the map.
    // particleMap.insert(std::unordered_map<unsigned int, unsigned int>::value_type(particle, node));

    // Store the particle index.
    nodes[node].entity = particle;
}

unsigned int BVHTree::nParticles()
{
    return particleMap.size();
}

void BVHTree::removeParticle(SceneEntityWeakPtr &entity)
{
    // Map iterator.
    std::unordered_map<unsigned int, unsigned int>::iterator it;

    // Find the particle.
    it = particleMap.find(particle);

    // The particle doesn't exist.
    if (it == particleMap.end())
    {
        throw std::invalid_argument("[ERROR]: Invalid particle index!");
    }

    // Extract the node index.
    unsigned int node = it->second;

    // Erase the particle from the map.
    particleMap.erase(it);

    assert(node < nodeCapacity);
    assert(nodes[node].isLeaf());

    removeLeaf(node);
    freeNode(node);
}

void BVHTree::removeAll()
{
    // Iterator pointing to the start of the particle map.
    std::unordered_map<unsigned int, unsigned int>::iterator it = particleMap.begin();

    // Iterate over the map.
    while (it != particleMap.end())
    {
        // Extract the node index.
        unsigned int node = it->second;

        assert(node < nodeCapacity);
        assert(nodes[node].isLeaf());

        removeLeaf(node);
        freeNode(node);

        it++;
    }

    // Clear the particle map.
    particleMap.clear();
}

bool BVHTree::updateParticle(unsigned int particle, const glm::vec3 &mins, const glm::vec3 &maxs, bool alwaysReinsert)
{
    // Map iterator.
    std::unordered_map<unsigned int, unsigned int>::iterator it;

    // Find the particle.
    it = particleMap.find(particle);

    // The particle doesn't exist.
    if (it == particleMap.end())
    {
        // FIXME
        // throw std::invalid_argument("[ERROR]: Invalid particle index!");
    }

    // Extract the node index.
    unsigned int node = it->second;

    assert(node < nodeCapacity);
    assert(nodes[node].isLeaf());



    // Create the new AABB.
    BVHBoundingBox aabb(mins, maxs);

    // No need to update if the particle is still within its fattened AABB.
    if (!alwaysReinsert && nodes[node].aabb.Contains(aabb))
        return false;

    // Remove the current leaf.
    removeLeaf(node);

    aabb.Inflate(skinThickness);
    aabb.RecalcAreaAndCenter();

    // Assign the new AABB.
    nodes[node].aabb = aabb;

    // Insert a new leaf node.
    insertLeaf(node);

    return true;
}

unsigned int BVHTree::getHeight() const
{
    if (root == NULL_NODE)
        return 0;
    return nodes[root].height;
}

unsigned int BVHTree::getNodeCount() const
{
    return nodeCount;
}

unsigned int BVHTree::computeMaximumBalance() const
{
    unsigned int maxBalance = 0;
    for (unsigned int i = 0; i < nodeCapacity; i++)
    {
        if (nodes[i].height <= 1)
            continue;

        assert(nodes[i].isLeaf() == false);

        unsigned int balance = std::abs(nodes[nodes[i].left].height - nodes[nodes[i].right].height);
        maxBalance           = std::max(maxBalance, balance);
    }

    return maxBalance;
}

double BVHTree::computeSurfaceAreaRatio() const
{
    if (root == NULL_NODE)
        return 0.0;

    double rootArea  = nodes[root].aabb.ComputeSurfaceArea();
    double totalArea = 0.0;

    for (unsigned int i = 0; i < nodeCapacity; i++)
    {
        if (nodes[i].height < 0)
            continue;

        totalArea += nodes[i].aabb.ComputeSurfaceArea();
    }

    return totalArea / rootArea;
}

void BVHTree::validate() const
{
#ifndef NDEBUG
    validateStructure(root);
    validateMetrics(root);

    unsigned int freeCount = 0;
    unsigned int freeIndex = freeList;

    while (freeIndex != NULL_NODE)
    {
        assert(freeIndex < nodeCapacity);
        freeIndex = nodes[freeIndex].next;
        freeCount++;
    }

    assert(getHeight() == computeHeight());
    assert((nodeCount + freeCount) == nodeCapacity);
#endif
}

void BVHTree::rebuild()
{
    std::vector<unsigned int> nodeIndices(nodeCount);
    unsigned int              count = 0;

    for (unsigned int i = 0; i < nodeCapacity; i++)
    {
        // Free node.
        if (nodes[i].height < 0)
            continue;

        if (nodes[i].isLeaf())
        {
            nodes[i].parent    = NULL_NODE;
            nodeIndices[count] = i;
            count++;
        }
        else
            freeNode(i);
    }

    while (count > 1)
    {
        double minCost = std::numeric_limits<double>::max();
        int    iMin = -1, jMin = -1;

        for (unsigned int i = 0; i < count; i++)
        {
            BVHBoundingBox aabbi = nodes[nodeIndices[i]].aabb;

            for (unsigned int j = i + 1; j < count; j++)
            {
                BVHBoundingBox aabbj = nodes[nodeIndices[j]].aabb;
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

        unsigned int parent  = allocateNode();
        nodes[parent].left   = index1;
        nodes[parent].right  = index2;
        nodes[parent].height = 1 + std::max(nodes[index1].height, nodes[index2].height);
        nodes[parent].aabb.Merge(nodes[index1].aabb, nodes[index2].aabb);
        nodes[parent].parent = NULL_NODE;

        nodes[index1].parent = parent;
        nodes[index2].parent = parent;

        nodeIndices[jMin] = nodeIndices[count - 1];
        nodeIndices[iMin] = parent;
        count--;
    }

    root = nodeIndices[0];

    validate();
}

unsigned int BVHTree::allocateNode()
{
    // Exand the node pool as needed.
    if (freeList == NULL_NODE)
    {
        assert(nodeCount == nodeCapacity);

        // The free list is empty. Rebuild a bigger pool.
        nodeCapacity *= 2;
        nodes.resize(nodeCapacity);

        // Build a linked list for the list of free nodes.
        for (unsigned int i = nodeCount; i < nodeCapacity - 1; i++)
        {
            nodes[i].next   = i + 1;
            nodes[i].height = -1;
        }
        nodes[nodeCapacity - 1].next   = NULL_NODE;
        nodes[nodeCapacity - 1].height = -1;

        // Assign the index of the first free node.
        freeList = nodeCount;
    }

    // Peel a node off the free list.
    unsigned int node  = freeList;
    freeList           = nodes[node].next;
    nodes[node].parent = NULL_NODE;
    nodes[node].left   = NULL_NODE;
    nodes[node].right  = NULL_NODE;
    nodes[node].height = 0;    
    nodeCount++;

    return node;
}

void BVHTree::freeNode(unsigned int node)
{
    assert(node < nodeCapacity);
    assert(0 < nodeCount);

    nodes[node].next   = freeList;
    nodes[node].height = -1;
    freeList           = node;
    nodeCount--;
}

void BVHTree::insertLeaf(unsigned int leaf)
{
    if (root == NULL_NODE)
    {
        root               = leaf;
        nodes[root].parent = NULL_NODE;
        return;
    }

    // Find the best sibling for the node.

    BVHBoundingBox         leafAABB = nodes[leaf].aabb;
    unsigned int index    = root;

    while (!nodes[index].isLeaf())
    {
        // Extract the children of the node.
        unsigned int left  = nodes[index].left;
        unsigned int right = nodes[index].right;

        double surfaceArea = nodes[index].aabb.GetSurfaceArea();

        BVHBoundingBox combinedAABB;
        combinedAABB.Merge(nodes[index].aabb, leafAABB);
        double combinedSurfaceArea = combinedAABB.GetSurfaceArea();

        // Cost of creating a new parent for this node and the new leaf.
        double cost = 2.0 * combinedSurfaceArea;

        // Minimum cost of pushing the leaf further down the tree.
        double inheritanceCost = 2.0 * (combinedSurfaceArea - surfaceArea);

        // Cost of descending to the left.
        double costLeft;
        if (nodes[left].isLeaf())
        {
            BVHBoundingBox aabb;
            aabb.Merge(leafAABB, nodes[left].aabb);
            costLeft = aabb.GetSurfaceArea() + inheritanceCost;
        }
        else
        {
            BVHBoundingBox aabb;
            aabb.Merge(leafAABB, nodes[left].aabb);
            double oldArea = nodes[left].aabb.GetSurfaceArea();
            double newArea = aabb.GetSurfaceArea();
            costLeft       = (newArea - oldArea) + inheritanceCost;
        }

        // Cost of descending to the right.
        double costRight;
        if (nodes[right].isLeaf())
        {
            BVHBoundingBox aabb;
            aabb.Merge(leafAABB, nodes[right].aabb);
            costRight = aabb.GetSurfaceArea() + inheritanceCost;
        }
        else
        {
            BVHBoundingBox aabb;
            aabb.Merge(leafAABB, nodes[right].aabb);
            double oldArea = nodes[right].aabb.GetSurfaceArea();
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
    unsigned int oldParent  = nodes[sibling].parent;
    unsigned int newParent  = allocateNode();
    nodes[newParent].parent = oldParent;
    nodes[newParent].aabb.Merge(leafAABB, nodes[sibling].aabb);
    nodes[newParent].height = nodes[sibling].height + 1;

    // The sibling was not the root.
    if (oldParent != NULL_NODE)
    {
        if (nodes[oldParent].left == sibling)
            nodes[oldParent].left = newParent;
        else
            nodes[oldParent].right = newParent;

        nodes[newParent].left  = sibling;
        nodes[newParent].right = leaf;
        nodes[sibling].parent  = newParent;
        nodes[leaf].parent     = newParent;
    }
    // The sibling was the root.
    else
    {
        nodes[newParent].left  = sibling;
        nodes[newParent].right = leaf;
        nodes[sibling].parent  = newParent;
        nodes[leaf].parent     = newParent;
        root                   = newParent;
    }

    // Walk back up the tree fixing heights and AABBs.
    index = nodes[leaf].parent;
    while (index != NULL_NODE)
    {
        index = balance(index);

        unsigned int left  = nodes[index].left;
        unsigned int right = nodes[index].right;

        assert(left != NULL_NODE);
        assert(right != NULL_NODE);

        nodes[index].height = 1 + std::max(nodes[left].height, nodes[right].height);
        nodes[index].aabb.Merge(nodes[left].aabb, nodes[right].aabb);

        index = nodes[index].parent;
    }
}

void BVHTree::removeLeaf(unsigned int leaf)
{
    if (leaf == root)
    {
        root = NULL_NODE;
        return;
    }

    unsigned int parent      = nodes[leaf].parent;
    unsigned int grandParent = nodes[parent].parent;
    unsigned int sibling;

    if (nodes[parent].left == leaf)
        sibling = nodes[parent].right;
    else
        sibling = nodes[parent].left;

    // Destroy the parent and connect the sibling to the grandparent.
    if (grandParent != NULL_NODE)
    {
        if (nodes[grandParent].left == parent)
            nodes[grandParent].left = sibling;
        else
            nodes[grandParent].right = sibling;

        nodes[sibling].parent = grandParent;
        freeNode(parent);

        // Adjust ancestor bounds.
        unsigned int index = grandParent;
        while (index != NULL_NODE)
        {
            index = balance(index);

            unsigned int left  = nodes[index].left;
            unsigned int right = nodes[index].right;

            nodes[index].aabb.Merge(nodes[left].aabb, nodes[right].aabb);
            nodes[index].height = 1 + std::max(nodes[left].height, nodes[right].height);

            index = nodes[index].parent;
        }
    }
    else
    {
        root                  = sibling;
        nodes[sibling].parent = NULL_NODE;
        freeNode(parent);
    }
}

unsigned int BVHTree::balance(unsigned int node)
{
    assert(node != NULL_NODE);

    if (nodes[node].isLeaf() || (nodes[node].height < 2))
        return node;

    unsigned int left  = nodes[node].left;
    unsigned int right = nodes[node].right;

    assert(left < nodeCapacity);
    assert(right < nodeCapacity);

    int currentBalance = nodes[right].height - nodes[left].height;

    // Rotate right branch up.
    if (currentBalance > 1)
    {
        unsigned int rightLeft  = nodes[right].left;
        unsigned int rightRight = nodes[right].right;

        assert(rightLeft < nodeCapacity);
        assert(rightRight < nodeCapacity);

        // Swap node and its right-hand child.
        nodes[right].left   = node;
        nodes[right].parent = nodes[node].parent;
        nodes[node].parent  = right;

        // The node's old parent should now point to its right-hand child.
        if (nodes[right].parent != NULL_NODE)
        {
            if (nodes[nodes[right].parent].left == node)
                nodes[nodes[right].parent].left = right;
            else
            {
                assert(nodes[nodes[right].parent].right == node);
                nodes[nodes[right].parent].right = right;
            }
        }
        else
            root = right;

        // Rotate.
        if (nodes[rightLeft].height > nodes[rightRight].height)
        {
            nodes[right].right       = rightLeft;
            nodes[node].right        = rightRight;
            nodes[rightRight].parent = node;
            nodes[node].aabb.Merge(nodes[left].aabb, nodes[rightRight].aabb);
            nodes[right].aabb.Merge(nodes[node].aabb, nodes[rightLeft].aabb);

            nodes[node].height  = 1 + std::max(nodes[left].height, nodes[rightRight].height);
            nodes[right].height = 1 + std::max(nodes[node].height, nodes[rightLeft].height);
        }
        else
        {
            nodes[right].right      = rightRight;
            nodes[node].right       = rightLeft;
            nodes[rightLeft].parent = node;
            nodes[node].aabb.Merge(nodes[left].aabb, nodes[rightLeft].aabb);
            nodes[right].aabb.Merge(nodes[node].aabb, nodes[rightRight].aabb);

            nodes[node].height  = 1 + std::max(nodes[left].height, nodes[rightLeft].height);
            nodes[right].height = 1 + std::max(nodes[node].height, nodes[rightRight].height);
        }

        return right;
    }

    // Rotate left branch up.
    if (currentBalance < -1)
    {
        unsigned int leftLeft  = nodes[left].left;
        unsigned int leftRight = nodes[left].right;

        assert(leftLeft < nodeCapacity);
        assert(leftRight < nodeCapacity);

        // Swap node and its left-hand child.
        nodes[left].left   = node;
        nodes[left].parent = nodes[node].parent;
        nodes[node].parent = left;

        // The node's old parent should now point to its left-hand child.
        if (nodes[left].parent != NULL_NODE)
        {
            if (nodes[nodes[left].parent].left == node)
                nodes[nodes[left].parent].left = left;
            else
            {
                assert(nodes[nodes[left].parent].right == node);
                nodes[nodes[left].parent].right = left;
            }
        }
        else
            root = left;

        // Rotate.
        if (nodes[leftLeft].height > nodes[leftRight].height)
        {
            nodes[left].right       = leftLeft;
            nodes[node].left        = leftRight;
            nodes[leftRight].parent = node;
            nodes[node].aabb.Merge(nodes[right].aabb, nodes[leftRight].aabb);
            nodes[left].aabb.Merge(nodes[node].aabb, nodes[leftLeft].aabb);

            nodes[node].height = 1 + std::max(nodes[right].height, nodes[leftRight].height);
            nodes[left].height = 1 + std::max(nodes[node].height, nodes[leftLeft].height);
        }
        else
        {
            nodes[left].right      = leftRight;
            nodes[node].left       = leftLeft;
            nodes[leftLeft].parent = node;
            nodes[node].aabb.Merge(nodes[right].aabb, nodes[leftLeft].aabb);
            nodes[left].aabb.Merge(nodes[node].aabb, nodes[leftRight].aabb);

            nodes[node].height = 1 + std::max(nodes[right].height, nodes[leftLeft].height);
            nodes[left].height = 1 + std::max(nodes[node].height, nodes[leftRight].height);
        }

        return left;
    }

    return node;
}

unsigned int BVHTree::computeHeight() const
{
    return computeHeight(root);
}

unsigned int BVHTree::computeHeight(unsigned int node) const
{
    assert(node < nodeCapacity);

    if (nodes[node].isLeaf())
        return 0;

    unsigned int height1 = computeHeight(nodes[node].left);
    unsigned int height2 = computeHeight(nodes[node].right);

    return 1 + std::max(height1, height2);
}

void BVHTree::validateStructure(unsigned int node) const
{
    if (node == NULL_NODE)
        return;

    if (node == root)
        assert(nodes[node].parent == NULL_NODE);

    unsigned int left  = nodes[node].left;
    unsigned int right = nodes[node].right;

    if (nodes[node].isLeaf())
    {
        assert(left == NULL_NODE);
        assert(right == NULL_NODE);
        assert(nodes[node].height == 0);
        return;
    }

    assert(left < nodeCapacity);
    assert(right < nodeCapacity);

    assert(nodes[left].parent == node);
    assert(nodes[right].parent == node);

    validateStructure(left);
    validateStructure(right);
}

void BVHTree::validateMetrics(unsigned int node) const
{
    if (node == NULL_NODE)
        return;

    unsigned int left  = nodes[node].left;
    unsigned int right = nodes[node].right;

    if (nodes[node].isLeaf())
    {
        assert(left == NULL_NODE);
        assert(right == NULL_NODE);
        assert(nodes[node].height == 0);
        return;
    }

    assert(left < nodeCapacity);
    assert(right < nodeCapacity);

    int height1 = nodes[left].height;
    int height2 = nodes[right].height;
    int height  = 1 + std::max(height1, height2);
    (void)height; // Unused variable in Release build
    assert(nodes[node].height == height);

    BVHBoundingBox aabb;
    aabb.Merge(nodes[left].aabb, nodes[right].aabb);

    for (unsigned int i = 0; i < 3; i++)
    {
        assert(aabb.Mins()[i] == nodes[node].aabb.Mins()[i]);
        assert(aabb.Maxs()[i] == nodes[node].aabb.Maxs()[i]);
    }

    validateMetrics(left);
    validateMetrics(right);
}

bool BVHNode::isLeaf() const
{
    return (left == NULL_NODE);
}
