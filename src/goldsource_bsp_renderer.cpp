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
#include "goldsource_bsp_level.h"
#include "goldsource_bsp_renderer.h"

#define BACKFACE_EPSILON 0.01

using namespace GoldSource;

void BSPRenderer::RecursiveWorldNode(mnode_t *node)
{
    int c, side, *pindex;
//    glm::vec3 acceptpt, rejectpt;
    mplane_t *plane;
    msurface_t *surf, **mark;
    mleaf_t *pleaf;
    double dot;

  //  glm::vec3 mins, maxs;

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
        surf = m_pLevel->Faces(node->firstsurface);

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

BSPRenderer::BSPRenderer(BSPLevel *world)
{
    m_pLevel = world;

    
    auto & modelList = world->GetSubmodels();

    for (auto & it: modelList)
    {
        auto renderCookie = BuildDisplayMesh(&it);
        m_RenderInfos.push_back(renderCookie);
    }

    constexpr float ang = glm::radians(90.f);
    m_Transform = glm::rotate(glm::mat4(1), -ang, {1.f, 0.f, 0.f});
    m_Transform *= glm::rotate(glm::mat4(1), ang, {0.f, 0.f, 1.f});
}

BSPRenderer::~BSPRenderer()
{
    ClearPointersVector(m_RenderInfos);
}

void BSPRenderer::RenderWorld(glm::vec3 cameraPosition)
{
    // Make Z-Axis look up
    m_vecEyesPosition = cameraPosition.xzy;
    auto shader = GLBackend::Instance()->LightMappedSceneShader();


    shader->Bind();
    shader->SetDefaultCamera();
    shader->SetTransform(m_Transform);
    shader->SetScale(1);
    
    auto inf = m_RenderInfos[0];

    auto pMesh = inf->GetDrawMesh();
    pMesh->Bind();

    auto & lists = inf->GetDisplayList();

    //glDisable(GL_CULL_FACE);
    
    
   //for (int i = 0; i < 100; i++)
    {
        
        shader->SetTransform(m_Transform);


        for (auto &it : lists)
        {
            GLBackend::BindTexture(1, it.lm);
            GLBackend::BindTexture(0, it.diffuse);

            pMesh->Draw(it.first, it.count);
        }
    }

    //RecursiveWorldNode(m_pWorld->GetNodes(0));

    pMesh->Unbind();
    shader->Unbind();
    
}

void BSPRenderer::RenderBrushPoly(msurface_t *fa)
{
//     GLBackend::BindTexture(1, fa->lightmaptexturenum);
//     GLBackend::BindTexture(0, fa->diffuseTexture);
//                        
//     pMesh->Draw(fa->meshOffset, fa->meshLength);                  
}

void BSPRenderer::BuildSurfaceDisplayList(msurface_t *fa, DrawMesh *pMesh)
{
    int i, lindex, lnumverts;
    const medge_t *pedges, *r_pedge;
    int vertpage;
    glm::vec3 vec;
    float s = 0, t = 0;
    
    // reconstruct the polygon
    pedges = m_pLevel->GetEdges().data();
    lnumverts = fa->numedges;
    vertpage  = 0;

    auto & surfEdges = m_pLevel->GetSurfEdges();
    auto & vertices  = m_pLevel->GetVertices();
    auto lmState   = m_pLevel->GetLightmapState();

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

    fa->meshOffset = pMesh->CurrentElement();

    if (fa->texinfo->texture->loadedTexture)
        fa->diffuseTexture = fa->texinfo->texture->loadedTexture;
    

    for (size_t i = 0; i < m_verts.size() - 2; i++)
    {

        auto & v1 = m_verts[0];
        auto & v2 = m_verts[i + 1];
        auto & v3 = m_verts[i + 2];

        pMesh->TexCoord2f(v3.uv.x, v3.uv.y);
        pMesh->Color3f(v3.color.r, v3.color.g, v3.color.b);
        pMesh->Vertex3fv((float *)&v3.xyz);

        pMesh->TexCoord2f(v2.uv.x, v2.uv.y);
        pMesh->Color3f(v2.color.r, v2.color.g, v2.color.b);
        pMesh->Vertex3fv((float *)&v2.xyz);

        pMesh->TexCoord2f(v1.uv.x, v1.uv.y);
        pMesh->Color3f(v1.color.r, v1.color.g, v1.color.b);
        pMesh->Vertex3fv((float *)&v1.xyz);

    }

    fa->meshLength = pMesh->CurrentElement() - fa->meshOffset;

}

template<class T>
inline int sortCmp(T a, T b)
{
    if (a == b)
        return 0;


    if (a > b)
        return 1;
    
    return -1;
}

GoldSource::BSPModelRenderCookie * GoldSource::BSPRenderer::BuildDisplayMesh(const dmodel_t *mod)
{
    std::vector<msurface_t *> m_SortedFaces;
    std::vector<msurface_t> &surfaces = m_pLevel->GetFaces();
    
    for (int i = mod->firstface; i < mod->numfaces; i++)
    {
        auto s = &surfaces[i];

        if (s->texinfo->flags & SURF_SKY)
            continue;

        if (!s->texinfo->texture->loadedTexture)
            s->texinfo->texture->loadedTexture = TextureManager::GetFallbackTexture();
        
        m_SortedFaces.push_back(s);
    }
    
    std::qsort(m_SortedFaces.data(), m_SortedFaces.size(), sizeof(msurface_t *),
               [](const void *pa, const void *pb) -> int {
        const msurface_t *faceA = *(msurface_t **)(pa);
        const msurface_t *faceB = *(msurface_t **)(pb);

        GLuint texA = 0;
        GLuint texB = 0;

        if (faceA->texinfo->texture->loadedTexture)
            texA = faceA->texinfo->texture->loadedTexture->Index();

        if (faceB->texinfo->texture->loadedTexture)
            texB = faceB->texinfo->texture->loadedTexture->Index();

        GLuint lmA = faceA->lightmaptexturenum;
        GLuint lmB = faceB->lightmaptexturenum;

        int cmp = sortCmp(texA, texB);
        int lmCmp = sortCmp(lmA, lmB);

        if (cmp == 0)
        {
            return lmCmp;
        }

        return cmp;
    });

    displayList_t lists;
    DrawMesh *pMesh = new DrawMesh;

    pMesh->Begin(GL_TRIANGLES);

    displayMesh_t mesh = {0,0,0,0};

    GLTexture* diffuseTexture = 0;
    GLuint lmTexture = 0;

    for (auto &surf : m_SortedFaces)
    {
        m_pLevel->GL_CreateSurfaceLightmap(surf);
        BuildSurfaceDisplayList(surf, pMesh);

        if (diffuseTexture != surf->diffuseTexture || lmTexture != surf->lightmaptexturenum)
        {
            if (diffuseTexture !=0)
            {
                lists.push_back(mesh);
            }

            diffuseTexture = surf->diffuseTexture;
            lmTexture      = surf->lightmaptexturenum;

            mesh.first  = surf->meshOffset;            
            mesh.diffuse = diffuseTexture;
            mesh.lm      = surf->lightmaptexturenum;              

        }

        mesh.count = (surf->meshOffset + surf->meshLength) - mesh.first;
    }

    if (mesh.count != 0)
        lists.push_back(mesh);

    pMesh->End();

    
    auto lmState = m_pLevel->GetLightmapState();
    lmState->UploadBlock(false);

    BSPModelRenderCookie *pResult = new BSPModelRenderCookie(pMesh, lists);

    return pResult;
}

BSPModelRenderCookie::BSPModelRenderCookie(DrawMesh *pMesh, displayList_t dl)
{
    m_pMesh       = pMesh;
    m_DisplayList = std::move(dl);
}

BSPModelRenderCookie::~BSPModelRenderCookie()
{
    delete m_pMesh;
    m_DisplayList.clear();
}
