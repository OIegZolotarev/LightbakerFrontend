/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once

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

#include "mathlib.h"
#include "r_bvh_boundingbox.h"
#include <unordered_map>
#include <vector>

#include "scene_entity.h"

struct BVHNode
{
    /// Constructor.
    BVHNode(){};

    // The fattened axis-aligned bounding box.
    BVHBoundingBox aabb;

    // Index of the parent node.
    unsigned int parent;

    // Index of the next node.
    unsigned int next;

    // Index of the left-hand child.
    unsigned int left;

    // Index of the right-hand child.
    unsigned int right;

    // Height of the node. This is 0 for a leaf and -1 for a free node.
    int height;

    SceneEntityWeakPtr entity;

    bool IsLeaf() const;
};

/*! \brief The dynamic AABB tree.

The dynamic AABB tree is a hierarchical data structure that can be used
to efficiently query overlaps between objects of arbitrary shape and
size that lie inside of a simulation box. Support is provided for
periodic and non-periodic boxes, as well as boxes with partial
periodicity, e.g. periodic along specific axes.
*/
class BVHTree
{
public:
    BVHTree(double skinThickness, const glm::vec3 &extents);

    //! Insert a particle into the tree (arbitrary shape with bounding box).
    /*! \param index
            The index of the particle.

        \param lowerBound
            The lower bound in each dimension.

        \param upperBound
            The upper bound in each dimension.
     */
    void InsertEntity(SceneEntityWeakPtr &entity, const glm::vec3 &mins, const glm::vec3 &maxs);

    /// Return the number of particles in the tree.
    unsigned int nParticles();

    //! Remove a particle from the tree.
    /*! \param particle
            The particle index (particleMap will be used to map the node).
     */
    void RemoveEntity(SceneEntityWeakPtr &entity);

    /// Remove all particles from the tree.
    void RemoveAll();

    //! Update the tree if a particle moves outside its fattened AABB.
    /*! \param particle
            The particle index (particleMap will be used to map the node).

        \param lowerBound
            The lower bound in each dimension.

        \param upperBound
            The upper bound in each dimension.

        \param alwaysReinsert
            Always reinsert the particle, even if it's within its old AABB (default: false)
     */
    bool UpdateEntity(unsigned int, const glm::vec3 &mins, const glm::vec3 &maxs, bool alwaysReinsert = false);

    //! Get the height of the tree.
    /*! \return
            The height of the binary tree.
     */
    unsigned int GetHeight() const;

    //! Get the number of nodes in the tree.
    /*! \return
            The number of nodes in the tree.
     */
    unsigned int GetNodeCount() const;

    //! Compute the maximum balance of the tree.
    /*! \return
            The maximum difference between the height of two
            children of a node.
     */
    unsigned int ComputeMaximumBalance() const;

    //! Compute the surface area ratio of the tree.
    /*! \return
            The ratio of the sum of the node surface area to the surface
            area of the root node.
     */
    double ComputeSurfaceAreaRatio() const;

    /// Validate the tree.
    void Validate() const;

    /// Rebuild an optimal tree.
    void Rebuild();

private:
    /// The index of the root node.
    unsigned int root;

    /// The dynamic tree.
    std::vector<BVHNode> nodes;

    /// The current number of nodes in the tree.
    unsigned int nodeCount;

    /// The current node capacity.
    unsigned int nodeCapacity;

    /// The position of node at the top of the free list.
    unsigned int freeList;

    /// The skin thickness of the fattened AABBs, as a fraction of the AABB base length.
    double skinThickness;

    /// Whether the system is periodic along each axis.
    glm::bvec3 periodicity;

    /// The size of the system in each dimension.
    glm::vec3 boxSize;

    /// The position of the negative minimum image.
    glm::vec3 negMinImage;

    /// The position of the positive minimum image.
    glm::vec3 posMinImage;

    /// A map between particle and node indices.
    std::unordered_map<unsigned int, unsigned int> particleMap;

    /// Does touching count as overlapping in tree queries?
    bool touchIsOverlap;

    //! Allocate a new node.
    /*! \return
            The index of the allocated node.
     */
    unsigned int AllocateNode();

    //! Free an existing node.
    /*! \param node
            The index of the node to be freed.
     */
    void FreeNode(unsigned int node);

    //! Insert a leaf into the tree.
    /*! \param leaf
            The index of the leaf node.
     */
    void InsertLeaf(unsigned int leaf);

    //! Remove a leaf from the tree.
    /*! \param leaf
            The index of the leaf node.
     */
    void RemoveLeaf(unsigned int leaf);

    //! Balance the tree.
    /*! \param node
            The index of the node.
     */
    unsigned int Balance(unsigned int node);

    //! Compute the height of the tree.
    /*! \return
            The height of the entire tree.
     */
    unsigned int ComputeHeight() const;

    //! Compute the height of a sub-tree.
    /*! \param node
            The index of the root node.

        \return
            The height of the sub-tree.
     */
    unsigned int ComputeHeight(unsigned int) const;

    //! Assert that the sub-tree has a valid structure.
    /*! \param node
            The index of the root node.
     */
    void ValidateStructure(unsigned int) const;

    //! Assert that the sub-tree has valid metrics.
    /*! \param node
            The index of the root node.
     */
    void ValidateMetrics(unsigned int) const;
};
