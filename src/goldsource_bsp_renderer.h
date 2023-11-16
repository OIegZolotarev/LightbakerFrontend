/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

// BSP Renderer is mostly based on code by Id Software and Valve Software

/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

/***
 *
 *  Copyright (c) 1998, Valve LLC. All rights reserved.
 *
 *  This product contains software technology licensed from Id
 *  Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc.
 *  All Rights Reserved.
 *
 ****/

#pragma once

namespace GoldSource
{
class BSPRenderer
{
    glm::vec3 m_vecEyesPosition;
    size_t m_visFrameCount = 0;
    BSPWorld *m_pWorld;
    
    std::vector<msurface_t *> m_SortedFaces;

   

    typedef struct
    {
        size_t first;
        size_t count;

        GLuint diffuse;
        GLuint lm;
    } displayMesh_t;
     

    std::list<displayMesh_t> m_DisplayLists;

    void RenderBrushPoly(msurface_t *fa);
    void RecursiveWorldNode(mnode_t *node);

    void BuildSurfaceDisplayList(msurface_t *fa);

    DrawMesh *m_pMesh = nullptr;
    void BuildDisplayMesh();

  public:
    BSPRenderer(BSPWorld *world);
    ~BSPRenderer();

    void PerformRendering(glm::vec3 cameraPosition);

  private:
    glm::mat4 m_Transform;
};

} // namespace GoldSource
