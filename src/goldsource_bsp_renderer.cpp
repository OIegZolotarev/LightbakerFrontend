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


void BSPRenderer::RecursiveWorldNode(mnode_t* node)
{
	int			i, c, side, * pindex;
	glm::vec3		acceptpt, rejectpt;
	mplane_t* plane;
	msurface_t* surf, ** mark;
	mleaf_t* pleaf;
	double		d, dot;
	glm::vec3 mins, maxs;
	
	if (!node) return;
	if (!node->plane) return;
	if (node->contents == CONTENTS_SOLID)
		return;		// solid

	if (node->visframe != m_visFrameCount)
		return;

	// 	if (R_CullBox (node->minmaxs, node->minmaxs+3))
	// 		return;

	// if a leaf node, draw stuff
	if (node->contents < 0)
	{

		pleaf = (mleaf_t*)node;
		
		//if (Frustrum->Cull(pleaf->minmaxs, pleaf->minmaxs + 3))
//			return;

		mark = pleaf->firstmarksurface;
		c = pleaf->nummarksurfaces;

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

				if (surf->texinfo->flags & SURF_SKY) continue;

// 				if (surf->texinfo->flags2 & SURF_ALPHA_TRANSPARENT || surf->texinfo->flags2 & SURF_WATER)
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

BSPRenderer::BSPRenderer(BSPWorld* world)
{
	m_pWorld = world;
}

BSPRenderer::~BSPRenderer()
{

}

void BSPRenderer::PerformRendering(glm::vec3 cameraPosition)
{
	// Make Z-Axis look up
	m_vecEyesPosition = cameraPosition.xzy;

	glPushMatrix();
	glRotatef(-90, 1, 0, 0);	    // put Z going up
	glRotatef(90, 0, 0, 1);	    // put Z going up

	RecursiveWorldNode(m_pWorld->GetNodes(0));

	glPopMatrix();
}

void BSPRenderer::RenderBrushPoly(msurface_t* fa)
{
	texture_t* t;
	byte* base;
	int			maps;
	
	int smax, tmax;

// 	glActiveTexture(GL_TEXTURE0);
// 	glEnable(GL_TEXTURE_2D);
// 	glBindTexture(GL_TEXTURE_2D, t->gl_texturenum);
// 
// 	// 
// 	glActiveTexture(GL_TEXTURE1);
// 	glEnable(GL_TEXTURE_2D);
// 	
// 	glBindTexture(GL_TEXTURE_2D, fa->lightmaptexturenum);
// 
// 	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
// 	glActiveTexture(GL_TEXTURE0);


	auto p = fa->polys;

	glCullFace(GL_FRONT);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, fa->lightmaptexturenum);

	glBegin(GL_POLYGON);
	auto v = p->verts[0];
	for (int i = 0; i < p->numverts; i++, v += VERTEXSIZE)
	{
		glTexCoord2f(v[5], v[6]);
		//glColor3fv((float*)&fa->plane->normal);
		glColor3f(1,1,1);
		glVertex3fv(v);
	}
	glEnd();

	glEnable(GL_TEXTURE_2D);
	glCullFace(GL_BACK);
}
