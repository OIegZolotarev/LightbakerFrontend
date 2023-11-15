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


#include "application.h"
#include "goldsource_bsp_disk_structs.h"
#include "goldsource_bsp_mem_structs.h"
#include "goldsource_bsp_world.h"
#include "goldsource_bsp_renderer.h"

#define BACKFACE_EPSILON 0.01

using namespace GoldSource;

void BSPRenderer::RecursiveWorldNode(mnode_t *node)
{
    int i, c, side, *pindex;
    glm::vec3 acceptpt, rejectpt;
    mplane_t *plane;
    msurface_t *surf, **mark;
    mleaf_t *pleaf;
    double d, dot;

    glm::vec3 mins, maxs;

    if (!node)
        return;
    if (!node->plane)
        return;
    if (node->contents == CONTENTS_SOLID)
        return; // solid

    if (node->visframe != m_visFrameCount)
        return;

    // 	if (R_CullBox (node->minmaxs, node->minmaxs+3))
    // 		return;

    // if a leaf node, draw stuff
    if (node->contents < 0)
    {
        pleaf = (mleaf_t *)node;

        // if (Frustrum->Cull(pleaf->minmaxs, pleaf->minmaxs + 3))
        //			return;

        mark = pleaf->firstmarksurface;
        c    = pleaf->nummarksurfaces;

        if (c)
        {
            do
            {
                (*mark)->visframe = m_visFrameCount;
                mark++;
            } while (--c);
        }

        // deal with model fragments in this leaf
        // 			if (pleaf->efrags)
        // 				R_StoreEfrags (&pleaf->efrags);

        //	return;
    }

    // node is just a decision point, so go down the apropriate sides

    // find which side of the node we are on
    plane = node->plane;

    switch (plane->type)
    {
    case PLANE_X:
        dot = m_vecEyesPosition.x - plane->dist;
        break;
    case PLANE_Y:
        dot = m_vecEyesPosition.y - plane->dist;
        break;
    case PLANE_Z:
        dot = m_vecEyesPosition.z - plane->dist;
        break;
    default:
        dot = glm::dot(m_vecEyesPosition, plane->normal) - plane->dist;
        break;
    }

    if (dot >= 0)
        side = 0;
    else
        side = 1;

    // recurse down the children, front side first
    RecursiveWorldNode(node->children[side]);

    // draw stuff
    c = node->numsurfaces;

    if (c)
    {
        surf = m_pWorld->Faces(node->firstsurface);

        if (dot < -BACKFACE_EPSILON)
            side = SURF_PLANEBACK;
        else if (dot > BACKFACE_EPSILON)
            side = 0;
        {
            for (; c; c--, surf++)
            {
                // TODO: implement
                // 				if (surf->visframe != m_visFrameCount)
                // 					continue;

                if (surf->texinfo->flags & SURF_SKY)
                    continue;

                // 				if (surf->texinfo->flags2 & SURF_ALPHA_TRANSPARENT || surf->texinfo->flags2 &
                // SURF_WATER)
                // 				{
                // 					surf->texturechain = r_alpha_surfaces;
                // 					r_alpha_surfaces = surf;
                // 					continue;
                // 				}

                RenderBrushPoly(surf);
            }
        }
    }

    // recurse down the back side
    RecursiveWorldNode(node->children[!side]);
}

BSPRenderer::BSPRenderer(BSPWorld *world)
{
    m_pWorld = world;
    
    m_pMesh  = new DrawMesh;

    std::vector<msurface_t> & surfaces =  m_pWorld->GetFaces();

    m_pMesh->Begin(GL_TRIANGLES);

    for (auto & surf: surfaces)
    {
        BuildSurfaceDisplayList(&surf);
    }

    m_pMesh->End();
}

BSPRenderer::~BSPRenderer()
{
    if (m_pMesh)
        delete m_pMesh;
}

void BSPRenderer::PerformRendering(glm::vec3 cameraPosition)
{
    // Make Z-Axis look up
    m_vecEyesPosition = cameraPosition.xzy;

    glPushMatrix();
    glRotatef(-90, 1, 0, 0); // put Z going up
    glRotatef(90, 0, 0, 1);  // put Z going up

    //m_pMesh->BindAndDraw();


    m_pMesh->Bind();
    
    RecursiveWorldNode(m_pWorld->GetNodes(0));

    m_pMesh->Unbind();

    glPopMatrix();
}

void BSPRenderer::RenderBrushPoly(msurface_t *fa)
{
    mtexture_t *t;
    byte *base;
    int maps;

    int smax, tmax;

    auto p = fa->polys;

    if (1)
    {
        //glActiveTexture(GL_TEXTURE0);
        //glEnable(GL_TEXTURE_2D);

        if (fa->texinfo->texture->loadedTexture)
            glBindTexture(GL_TEXTURE_2D, fa->texinfo->texture->loadedTexture->gl_texnum);

        m_pMesh->Draw(fa->meshOffset, fa->meshLength);
        return;
    }

    glCullFace(GL_FRONT);
    glEnable(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fa->lightmaptexturenum);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);

    if (fa->texinfo->texture->loadedTexture)
        glBindTexture(GL_TEXTURE_2D, fa->texinfo->texture->loadedTexture->gl_texnum);
    // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBegin(GL_POLYGON);
    auto v = p->verts[0];
    for (int i = 0; i < p->numverts; i++, v += VERTEXSIZE)
    {
        glMultiTexCoord2f(GL_TEXTURE0, v[3], v[4]);
        glMultiTexCoord2f(GL_TEXTURE1, v[5], v[6]);

        glColor3f(1, 1, 1);
        glVertex3fv(v);
    }
    glEnd();

    glEnable(GL_TEXTURE_2D);
    glCullFace(GL_BACK);

    glActiveTexture(GL_TEXTURE1);
    glDisable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
}

void BSPRenderer::BuildSurfaceDisplayList(msurface_t *fa)
{
    int i, lindex, lnumverts;
    const medge_t *pedges, *r_pedge;
    int vertpage;
    glm::vec3 vec;
    float s = 0, t = 0;
    
    // reconstruct the polygon
    pedges = m_pWorld->GetEdges().data();
    lnumverts = fa->numedges;
    vertpage  = 0;

    auto & surfEdges = m_pWorld->GetSurfEdges();
    auto & vertices  = m_pWorld->GetVertices();
    auto lmState   = m_pWorld->GetLightmapState();

    std::vector<drawVert_t> m_verts;
    m_verts.reserve(lnumverts);

    for (i = 0; i < lnumverts; i++)
    {
        lindex = surfEdges[fa->firstedge + i];

        if (lindex > 0)
        {
            r_pedge = &pedges[lindex];
            vec     = vertices[r_pedge->v[0]];
        }
        else
        {
            r_pedge = &pedges[-lindex];
            vec     = vertices[r_pedge->v[1]];
        }

        glm::vec3 ofs = fa->texinfo->vecs[0].xyz;
        s             = glm::dot(vec, ofs) + fa->texinfo->vecs[0][3];
        s /= fa->texinfo->texture->width;

        ofs = fa->texinfo->vecs[1].xyz;
        t   = glm::dot(vec, ofs) + fa->texinfo->vecs[1][3];
        t /= fa->texinfo->texture->height;

        drawVert_t dw = drawVert_t();

        dw.uv = {s, t, 0};

        //m_pMesh->TexCoord2f(s, t);

        //
        // lightmap texture coordinates
        //

        ofs = fa->texinfo->vecs[0].xyz;
        s   = glm::dot(vec, ofs) + fa->texinfo->vecs[0][3];
        s -= fa->texturemins[0];
        s += fa->light_s * 16;
        s += 8;
        s /= lmState->BlockWidth() * 16;

        ofs = fa->texinfo->vecs[1].xyz;
        t   = glm::dot(vec, ofs) + fa->texinfo->vecs[1][3];
        t -= fa->texturemins[1];
        t += fa->light_t * 16;
        t += 8;
        t /= lmState->BlockHeight() * 16; // fa->texinfo->texture->height;


        dw.color = {s, t, 0,0};

        dw.xyz = {vec};

        //m_pMesh->Color3f(s, t, 0);
        //m_pMesh->Vertex3fv(&vec.x);

        m_verts.push_back(dw);
    }

    fa->meshOffset = m_pMesh->CurrentElement();
    

    for (size_t i = 0; i < m_verts.size() - 2; i++)
    {

        auto & v1 = m_verts[0];
        auto & v2 = m_verts[i + 1];
        auto & v3 = m_verts[i + 2];

        m_pMesh->TexCoord2f(v3.uv.x, v3.uv.y);
        m_pMesh->Color3f(v3.color.r, v3.color.g, v3.color.b);
        m_pMesh->Vertex3fv((float *)&v3.xyz);

        m_pMesh->TexCoord2f(v2.uv.x, v2.uv.y);
        m_pMesh->Color3f(v2.color.r, v2.color.g, v2.color.b);
        m_pMesh->Vertex3fv((float *)&v2.xyz);

        m_pMesh->TexCoord2f(v1.uv.x,v1.uv.y);
        m_pMesh->Color3f(v1.color.r, v1.color.g, v1.color.b);
        m_pMesh->Vertex3fv((float*)&v1.xyz);


        

        
        

//         m_Data->faces.push_back(m_verts[0]);
//         m_Data->faces.push_back(m_verts[i + 1]);
//         m_Data->faces.push_back(m_verts[i + 2]);
    }

    fa->meshLength = m_pMesh->CurrentElement() - fa->meshOffset;

}
