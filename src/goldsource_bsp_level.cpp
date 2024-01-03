/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#define _CRT_SECURE_NO_WARNINGS

#include "application.h"
#include "common.h"


#include "byteorder.h"
#include "wad_textures.h"
#include "goldsource_lightmap_atlas.h"
#include "bsp_entity.h"

#include "goldsource_bsp_level.h"

using namespace GoldSource;


void BSPLevel::Mod_LoadVertexes(lump_t* l)
{
	dvertex_t* in;
	mvertex_t* out;
	int			i, count;

	in = (dvertex_t*)(m_pFileData->Data() + l->fileofs);
	
	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadVertexes", this);

	count = l->filelen / sizeof(*in);

	m_vVertices.resize(count);
	out = (mvertex_t*)m_vVertices.data();

	for (i = 0; i < count; i++, in++, out++)
	{
		out->x = LittleFloat(in->point[0]);
		out->y = LittleFloat(in->point[1]);
		out->z = LittleFloat(in->point[2]);
	}
}

void BSPLevel::Mod_LoadEdges(lump_t* l)
{
	dedge_t* in;
	medge_t* out;
	int 	i, count;

	in = (dedge_t*)(m_pFileData->Data() + l->fileofs);
	
	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadEdges", this); 

	count = l->filelen / sizeof(*in);
	m_vEdges.resize(count);

	out = m_vEdges.data();

	for (i = 0; i < count; i++, in++, out++)
	{
		out->v[0] = (unsigned short)LittleShort(in->v[0]);
		out->v[1] = (unsigned short)LittleShort(in->v[1]);
	}
}

void BSPLevel::Mod_LoadSurfedges(lump_t* l)
{
	int		i, count;
	int* in, *out;

	in = (int*)(m_pFileData->Data() + l->fileofs);
	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadSurfEdges",this);
		
	
	count = l->filelen / sizeof(*in);		
	
	m_vSurfedges.resize(count);
	out = m_vSurfedges.data();

	for (i = 0; i < count; i++)
		out[i] = LittleLong(in[i]);
}

void BSPLevel::Mod_LoadTextures(lump_t* l)
{	
// 	miptex_t* mt;
 	mtexture_t* tx, * tx2; 
 	mtexture_t* anims[10];
 	mtexture_t* altanims[10];
// 	dmiptexlump_t* m;
 	int max = 0;
	int j;
	int num;
	int altmax;

	if (!l->filelen)
	{		
		return;
	}
	
	dmiptexlump_t* m = (dmiptexlump_t*)(m_pFileData->Data() + l->fileofs);
	m->nummiptex = LittleLong(m->nummiptex);

	m_vTextures.resize(m->nummiptex);

	for (int i = 0; i < m->nummiptex; i++)
	{
		m->dataofs[i] = LittleLong(m->dataofs[i]);


		miptex_t* mt = (miptex_t*)((byte*)m + m->dataofs[i]);
		mt->width = LittleLong(mt->width);
		mt->height = LittleLong(mt->height);

		mtexture_t * tx = new mtexture_t;
		memset(tx, 0, sizeof(tx));
		
		m_vTextures[i] = tx;
		memcpy(tx->name, mt->name, sizeof(tx->name));

		// TODO: make asynch

		
		if (mt->offsets[0])
            tx->loadedTexture = TextureManager::LoadTextureAsynch(mt, 0, mt->name, TextureSource::GoldSourceMipTexture);
		else
            tx->loadedTexture = TextureManager::LoadWADTextureAsynch(tx->name);
			


		tx->height = mt->height;
		tx->width = mt->width;

	}

	SequenceTextureAnims(m, tx, anims, altanims, max, altmax, j, tx2, num);

}

void BSPLevel::Mod_LoadLighting(lump_t* l)
{
	if (!l->filelen)
	{	
		return;
	}

	if (m_Version == HL_BSPVERSION)
	{		
		m_pLightData = new byte[l->filelen];
		memcpy(m_pLightData, m_pFileData->Data() + l->fileofs, l->filelen);
	}
	else if (m_Version == Q1_BSPVERSION)
	{		
		// Конвертируем чб-лайтмапу первокваки в цветную 
		//
		m_pLightData = new byte[l->filelen * 3];
		color24_t* pixels = (color24_t*)m_pLightData;
		byte* bwPixels = m_pFileData->Data() + l->fileofs;

		for (int i = 0; i <= l->filelen; i++)
		{
			pixels[i].r = bwPixels[i];
			pixels[i].g = bwPixels[i];
			pixels[i].b = bwPixels[i];
		}
	}
}

void BSPLevel::Mod_LoadPlanes(lump_t* l)
{
	int			i, j;
	mplane_t* out;
	dplane_t* in;
	int			count;
	int			bits;

	in = (dplane_t*)(m_pFileData->Data() + l->fileofs);
	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadPlanes", this);

	count = l->filelen / sizeof(*in);

	m_vPlanes.resize(count);

	out = m_vPlanes.data();

	for (i = 0; i < count; i++, in++, out++)
	{
		bits = 0;
		for (j = 0; j < 3; j++)
		{
			out->normal[j] = LittleFloat(in->normal[j]);
			if (out->normal[j] < 0)
				bits |= 1 << j;
		}

		out->dist = LittleFloat(in->dist);
		out->type = LittleLong(in->type);
		out->signbits = bits;
	}
}

void BSPLevel::Mod_LoadTexinfo(lump_t* l)
{
	texinfo_t* in;
	mtexinfo_t* out;
	int 	i, j, count;
	int		miptex;
	float	len1, len2;

	in = (texinfo_t*)(m_pFileData->Data() + l->fileofs);
	
	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadTexinfo", this);
	
	count = l->filelen / sizeof(*in);
	m_vTexInfo.resize(count);

	out = m_vTexInfo.data();

	for (i = 0; i < count; i++, in++, out++)
	{
		for (j = 0; j < 2; j++)
		{
			for(int k = 0 ; k  < 4; k++)
				out->vecs[j][k] = LittleFloat(in->vecs[j][k]);			
		}
		
		len1 = glm::length(out->vecs[0]);
		len2 = glm::length(out->vecs[1]);

		len1 = (len1 + len2) / 2;
		if (len1 < 0.32)
			out->mipadjust = 4;
		else if (len1 < 0.49)
			out->mipadjust = 3;
		else if (len1 < 0.99)
			out->mipadjust = 2;
		else
			out->mipadjust = 1;

		miptex = LittleLong(in->miptex);
		out->flags = LittleLong(in->flags);
		out->local_texture_idx = miptex;
		out->texture = m_vTextures[miptex];

		if (!_stricmp(out->texture->name, "sky"))
		{
			out->flags |= SURF_SKY;
		}

		if (!_stricmp(out->texture->name, "aaatrigger"))
		{
			out->flags |= SURF_SKY;
		}

		if (out->texture->name[0] == '{')
		{
			out->flags2 |= SURF_ALPHA_TRANSPARENT;
		}

		if (out->texture->name[0] == '!')
		{
			out->flags2 |= SURF_WATER;
		}
	}
}

void BSPLevel::Mod_LoadFaces(lump_t* l)
{
	dface_t* in;
	msurface_t* out;
	int			i, count, surfnum;
	int			planenum, side;

	in = (dface_t*)(m_pFileData->Data() + l->fileofs);
	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadFaces", this);

	count = l->filelen / sizeof(*in);

	m_vFaces.resize(count);

	out = m_vFaces.data();
	memset(out, 0, count * sizeof(*out));
		
	for (surfnum = 0; surfnum < count; surfnum++, in++, out++)
	{
		out->firstedge = LittleLong(in->firstedge);
		out->numedges = LittleShort(in->numedges);
		out->flags = 0;

		planenum = (unsigned short)LittleShort(in->planenum);
		side = LittleShort(in->side);
		if (side)
			out->flags |= SURF_PLANEBACK;

		out->plane = &m_vPlanes[planenum];

		out->texinfo = &m_vTexInfo[LittleShort(in->texinfo)];
				
		CalcSurfaceExtents(out);

		// lighting info

		for (i = 0; i < MAXLIGHTMAPS; i++)
			out->styles[i] = in->styles[i];

		i = LittleLong(in->lightofs);

		if (i == -1)
			out->samples = NULL;
		else
		{

			if (m_Version == HL_BSPVERSION)
				out->samples = m_pLightData + i;
			else if (m_Version == Q1_BSPVERSION)
				out->samples = m_pLightData + i * 3;
		}
					
// 		Handled by BSP renderer
// 		if (out->samples)	
// 			GL_CreateSurfaceLightmap(out);
// 
// 		BuildSurfaceDisplayList(out);
	}
}

void BSPLevel::Mod_LoadMarksurfaces(lump_t* l)
{
	size_t i, j, count;
	short* in;
	msurface_t** out;

	in = (short*)(m_pFileData->Data() + l->fileofs);

	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadMarksurfaces",this);


	count = l->filelen / sizeof(*in);
	m_vMarkSurfaces.resize(count);
	out = m_vMarkSurfaces.data();

	for (i = 0; i < count; i++)
	{
		j = LittleShort(in[i]);
		if (j >= m_vFaces.size())
			throw new std::exception("Mod_ParseMarksurfaces: bad surface number");
		
		out[i] = &m_vFaces[j];
	}
}

void BSPLevel::Mod_LoadVisibility(lump_t* l)
{
	if (!l->filelen)
	{
		m_pVisdata = nullptr;
		return;
	}
	
	m_pVisdata = new byte[l->filelen];
	memcpy(m_pVisdata, m_pFileData->Data() + l->fileofs, l->filelen);
}

void BSPLevel::Mod_LoadLeafs(lump_t* l)
{
	dleaf_t* in;
	mleaf_t* out;
	int			i, j, count, p;

	in = (dleaf_t*)(m_pFileData->Data() + l->fileofs);
	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadLeafs",this);
	
	count = l->filelen / sizeof(*in);

	m_vLeafs.resize(count);

	out = m_vLeafs.data();

	for (i = 0; i < count; i++, in++, out++)
	{
		for (j = 0; j < 3; j++)
		{
			out->minmaxs[j] = LittleShort(in->mins[j]);
			out->minmaxs[3 + j] = LittleShort(in->maxs[j]);
		}

		p = LittleLong(in->contents);
		out->contents = p;

		if (in->firstmarksurface < m_vMarkSurfaces.size())
		{
			out->firstmarksurface = &m_vMarkSurfaces[LittleShort(in->firstmarksurface)];
			out->nummarksurfaces = LittleShort(in->nummarksurfaces);
		}

		p = LittleLong(in->visofs);
		if (p == -1)
			out->compressed_vis = NULL;
		else
			out->compressed_vis = m_pVisdata + p;
		out->efrags = NULL;

		for (j = 0; j < 4; j++)
			out->ambient_sound_level[j] = in->ambient_level[j];

		// gl underwater warp
		if (out->contents != CONTENTS_EMPTY)
		{
			for (j = 0; j < out->nummarksurfaces; j++)
				out->firstmarksurface[j]->flags |= SURF_UNDERWATER;
		}
	}
}

void BSPLevel::Mod_LoadNodes(lump_t* l)
{
	int			i, j, count, p;
	dnode_t* in;
	mnode_t* out;

	in = (dnode_t*)(m_pFileData->Data() + l->fileofs);
	
	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadNodes",this);
	count = l->filelen / sizeof(*in);

	m_vNodes.resize(count);
	out = m_vNodes.data();

	for (i = 0; i < count; i++, in++)
	{


		out[i].minmaxs[0] = LittleShort(in->mins[0]);
		out[i].minmaxs[1] = LittleShort(in->mins[1]);
		out[i].minmaxs[2] = LittleShort(in->mins[2]);
		out[i].minmaxs[3] = LittleShort(in->maxs[0]);
		out[i].minmaxs[4] = LittleShort(in->maxs[1]);
		out[i].minmaxs[5] = LittleShort(in->maxs[2]);




		p = LittleLong(in->planenum);
		out[i].plane = &m_vPlanes[p];

		if (LittleShort(in->firstface) < 0)
			__debugbreak();


		out[i].firstsurface = LittleShort(in->firstface);
		out[i].numsurfaces = LittleShort(in->numfaces);

		//out[i].contents = 1;	// differentiate from leafs

		for (j = 0; j < 2; j++)
		{
			p = LittleShort(in->children[j]);
			if (p >= 0)
				out[i].children[j] = &m_vNodes[p];
			else
				out[i].children[j] = (mnode_t*)(&m_vLeafs[(-1 - p)]);
		}
	}

	Mod_SetParent(m_vNodes.data(), NULL);	// sets nodes and leafs
}

void BSPLevel::Mod_LoadClipnodes(lump_t* l)
{
	dclipnode_t* in, * out;
	int			i, count;
	//hull_t* hull;

	in = (dclipnode_t*)(m_pFileData->Data() + l->fileofs);
	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadClipnodes", this);
	
	count = l->filelen / sizeof(*in);
	
	m_vClipNodes.resize(count);
	out = m_vClipNodes.data();

// 	hull = &loadmodel->hulls[1];
// 	hull->clipnodes = out;
// 	hull->firstclipnode = 0;
// 	hull->lastclipnode = count - 1;
// 	hull->planes = loadmodel->planes;
// 	hull->clip_mins[0] = -16;
// 	hull->clip_mins[1] = -16;
// 	hull->clip_mins[2] = -36;
// 	hull->clip_maxs[0] = 16;
// 	hull->clip_maxs[1] = 16;
// 	hull->clip_maxs[2] = 36;
// 	//	hull->available = true;
// 
// 	hull = &loadmodel->hulls[2];
// 	hull->clipnodes = out;
// 	hull->firstclipnode = 0;
// 	hull->lastclipnode = count - 1;
// 	hull->planes = loadmodel->planes;
// 	hull->clip_mins[0] = -32;
// 	hull->clip_mins[1] = -32;
// 	hull->clip_mins[2] = -32;
// 	hull->clip_maxs[0] = 32;
// 	hull->clip_maxs[1] = 32;
// 	hull->clip_maxs[2] = 32;
// 	//	hull->available = true;
// 
// 	hull = &loadmodel->hulls[3];
// 	hull->clipnodes = out;
// 	hull->firstclipnode = 0;
// 	hull->lastclipnode = count - 1;
// 	hull->planes = loadmodel->planes;
// 	hull->clip_mins[0] = -16;
// 	hull->clip_mins[1] = -16;
// 	hull->clip_mins[2] = -18;
// 	hull->clip_maxs[0] = 16;
// 	hull->clip_maxs[1] = 16;
// 	hull->clip_maxs[2] = 18;
// 	//	hull->available = true;

	for (i = 0; i < count; i++, out++, in++)
	{
		out->planenum = LittleLong(in->planenum);
		out->children[0] = LittleShort(in->children[0]);
		out->children[1] = LittleShort(in->children[1]);
	}
}


void BSPLevel::Mod_LoadSubmodels(lump_t* l)
{
	dmodel_t* in;
	dmodel_t* out;
	int			i, j, count;

	in = (dmodel_t*)(m_pFileData->Data() + l->fileofs);
	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadSubmodels", this);

	count = l->filelen / sizeof(*in);
	
	m_vSubmodels.resize(count);
	out = m_vSubmodels.data();
		

	for (i = 0; i < count; i++, in++, out++)
	{
		for (j = 0; j < 3; j++)
		{	// spread the mins / maxs by a pixel
			out->mins[j] = LittleFloat(in->mins[j]) - 1;
			out->maxs[j] = LittleFloat(in->maxs[j]) + 1;
			out->origin[j] = LittleFloat(in->origin[j]);
		}
		for (j = 0; j < MAX_MAP_HULLS; j++)
			out->headnode[j] = LittleLong(in->headnode[j]);
		out->visleafs = LittleLong(in->visleafs);
		out->firstface = LittleLong(in->firstface);
		out->numfaces = LittleLong(in->numfaces);
	}
}

BSPLevel::BSPLevel(FileData * fd, Scene * pScene)
{
    m_pScene = pScene;

	m_pFileData = fd;
	m_Header = (dheader_t*)fd->Data();

	m_Version = m_Header->version;

	m_pLightmapState = new LightmapAtlas(512, 512);

	// Handled by loader task
// 	Mod_LoadEntities(&m_Header->lumps[LUMP_ENTITIES]);
// 
// 	Mod_LoadVertexes(&m_Header->lumps[LUMP_VERTEXES]);
// 	Mod_LoadEdges(&m_Header->lumps[LUMP_EDGES]);
// 	Mod_LoadSurfedges(&m_Header->lumps[LUMP_SURFEDGES]);
// 
// 	Mod_LoadTextures(&m_Header->lumps[LUMP_TEXTURES]);
// 	Mod_LoadLighting(&m_Header->lumps[LUMP_LIGHTING]);
// 	Mod_LoadPlanes(&m_Header->lumps[LUMP_PLANES]);
// 	Mod_LoadTexinfo(&m_Header->lumps[LUMP_TEXINFO]);
// 	Mod_LoadFaces(&m_Header->lumps[LUMP_FACES]);
// 	Mod_LoadMarksurfaces(&m_Header->lumps[LUMP_MARKSURFACES]);
// 	Mod_LoadVisibility(&m_Header->lumps[LUMP_VISIBILITY]);
// 	Mod_LoadLeafs(&m_Header->lumps[LUMP_LEAFS]);
// 	Mod_LoadNodes(&m_Header->lumps[LUMP_NODES]);
// 	Mod_LoadClipnodes(&m_Header->lumps[LUMP_CLIPNODES]);
// 	
// 	Mod_LoadSubmodels(&m_Header->lumps[LUMP_MODELS]);
	

	m_pFileData->Ref();

	
}

BSPLevel::~BSPLevel()
{
	if (m_pVisdata) delete m_pVisdata;
	if (m_pLightData) delete m_pLightData;
	if (m_pLightmapState) delete m_pLightmapState;
	if (m_pEntdata) delete m_pEntdata;

	FreeVector(m_vVertices);
	FreeVector(m_vEdges);
	FreeVector(m_vTextures);
	FreeVector(m_vPlanes);
	FreeVector(m_vTexInfo);
	FreeVector(m_vSurfedges);
	FreeVector(m_vFaces);
	FreeVector(m_vMarkSurfaces);
	FreeVector(m_vLeafs);
	FreeVector(m_vNodes);
	FreeVector(m_vClipNodes);
	FreeVector(m_vSubmodels);

	ClearPointersVector(m_vEntities);

	m_pFileData->UnRef();
}

std::string BSPLevel::GetBaseName()
{
	return m_pFileData->BaseName();	
}

void BSPLevel::SequenceTextureAnims(dmiptexlump_t* m, mtexture_t*& tx, mtexture_t** anims, mtexture_t** altanims, int& max, int& altmax, int& j, mtexture_t*& tx2, int& num)
{
	//
	// sequence the animations
	//
	for (int i = 0; i < m->nummiptex; i++)
	{

		tx = m_vTextures[i];
		if (!tx || tx->name[0] != '+')
			continue;
		if (tx->anim_next)
			continue;	// allready sequenced

	// find the number of frames in the animation
		memset(anims, 0, sizeof(anims));
		memset(altanims, 0, sizeof(altanims));

		max = tx->name[1];
		altmax = 0;
		if (max >= 'a' && max <= 'z')
			max -= 'a' - 'A';
		if (max >= '0' && max <= '9')
		{
			max -= '0';
			altmax = 0;
			anims[max] = tx;
			max++;
		}
		else if (max >= 'A' && max <= 'J')
		{
			altmax = max - 'A';
			max = 0;
			altanims[altmax] = tx;
			altmax++;
		}
		else
			throw new std::exception("Bad animating texture %s"); // TODO: fixme

		for (j = i + 1; j < m->nummiptex; j++)
		{
			tx2 = m_vTextures[j];
			if (!tx2 || tx2->name[0] != '+')
				continue;
			if (strcmp(tx2->name + 2, tx->name + 2))
				continue;

			num = tx2->name[1];
			if (num >= 'a' && num <= 'z')
				num -= 'a' - 'A';
			if (num >= '0' && num <= '9')
			{
				num -= '0';
				anims[num] = tx2;
				if (num + 1 > max)
					max = num + 1;
			}
			else if (num >= 'A' && num <= 'J')
			{
				num = num - 'A';
				altanims[num] = tx2;
				if (num + 1 > altmax)
					altmax = num + 1;
			}
			else
				throw new std::exception("Bad animating texture %s"); // TODO: fixme
		}

#define	ANIM_CYCLE	2
		// 	// link them all together
		for (j = 0; j < max; j++)
		{
			tx2 = anims[j];
			if (!tx2)
				//throw new std::exception("Missing frame %i of %s", j, tx->name);
				throw new std::exception("Missing frame %i of %s"); // TODO: fixme
			tx2->anim_total = max * ANIM_CYCLE;
			tx2->anim_min = j * ANIM_CYCLE;
			tx2->anim_max = (j + 1) * ANIM_CYCLE;
			tx2->anim_next = anims[(j + 1) % max];
			if (altmax)
				tx2->alternate_anims = altanims[0];
		}

		for (j = 0; j < altmax; j++)
		{
			tx2 = altanims[j];
			if (!tx2)
				//throw new std::exception("Missing frame %i of %s", j, tx->name);
				throw new std::exception("Missing frame %i of %s"); // TODO: fixme

			tx2->anim_total = altmax * ANIM_CYCLE;
			tx2->anim_min = j * ANIM_CYCLE;
			tx2->anim_max = (j + 1) * ANIM_CYCLE;
			tx2->anim_next = altanims[(j + 1) % altmax];
			if (max)
				tx2->alternate_anims = anims[0];
		}
	}



}

void BSPLevel::CalcSurfaceExtents(msurface_t* s)
{
	float	mins[2], maxs[2], val;
	int		i, j, e;
	mvertex_t* v;
	mtexinfo_t* tex;
	int		bmins[2], bmaxs[2];

	mins[0] = mins[1] = 999999;
	maxs[0] = maxs[1] = -99999;

	tex = s->texinfo;

	for (i = 0; i < s->numedges; i++)
	{
		e = m_vSurfedges[s->firstedge + i];
		if (e >= 0)
			v = &m_vVertices[m_vEdges[e].v[0]];
		else
			v = &m_vVertices[m_vEdges[-e].v[1]];

		for (j = 0; j < 2; j++)
		{
			val = v->x * tex->vecs[j][0] +
				v->y * tex->vecs[j][1] +
				v->z * tex->vecs[j][2] +
				tex->vecs[j][3];
			if (val < mins[j])
				mins[j] = val;
			if (val > maxs[j])
				maxs[j] = val;
		}
	}

	for (i = 0; i < 2; i++)
	{
		bmins[i] = floor(mins[i] / 16);
		bmaxs[i] = ceil(maxs[i] / 16);

		s->texturemins[i] = bmins[i] * 16;
		s->extents[i] = (bmaxs[i] - bmins[i]) * 16;
		
		if (!(tex->flags & TEX_SPECIAL) && s->extents[i] > 512 /* 256 */)
			throw new std::exception("Bad surface extents");
	}
}

void BSPLevel::Mod_SetParent(mnode_t* node, mnode_t* parent)
{
	node->parent = parent;
	if (node->contents < 0)
	{
		// 		node->children[0]=0;
		// 		node->children[1]=0;
		return;
	}
	Mod_SetParent(node->children[0], node);
	Mod_SetParent(node->children[1], node);
}


void BSPLevel::GL_CreateSurfaceLightmap(msurface_t* surf)
{
	int		smax, tmax;
	
	if (surf->flags & (SURF_DRAWSKY | SURF_DRAWTURB))
		return;

	smax = (surf->extents[0] >> 4) + 1;
	tmax = (surf->extents[1] >> 4) + 1;

	auto pos = m_pLightmapState->AllocBlock(smax, tmax);

	if (!pos)
	{
		
		m_pLightmapState->UploadBlock(false);
		m_pLightmapState->InitBlock();

		pos = m_pLightmapState->AllocBlock(smax, tmax);

		if (!pos)
		{
			// TODO: fixme
			throw new std::exception("Consecutive calls to LM_AllocBlock(%d,%d) failed\n");
		}
	}
	surf->light_s = pos->x;
	surf->light_t = pos->y;

	surf->lightmaptexturenum = m_pLightmapState->CurrentLightmapTexture();

	auto base = m_pLightmapState->LightmapBuffer();
	base += (surf->light_t * m_pLightmapState->BlockWidth() + surf->light_s) * LIGHTMAP_BYTES;
		
	size_t stride = m_pLightmapState->BlockWidth() * LIGHTMAP_BYTES;

	BuildLightMap(surf, base, stride);
}

void BSPLevel::Mod_LoadEntities(lump_t* l)
{
	if (!l->filelen)
	{
		m_pEntdata = nullptr;
		return;
	}

	m_pEntdata = new char[l->filelen];
	memcpy(m_pEntdata, m_pFileData->Data() + l->fileofs, l->filelen);


	size_t offset = 0;
	

	std::string_view token;

	auto grabToken = [&]()
	{
		// Skip spaces
		while (m_pEntdata[offset] <= ' ')
			offset++;

		size_t start = offset;

		bool quote = false;

		while (offset < (size_t)l->filelen)
		{
			if (m_pEntdata[offset] == '"')
			{
				quote = !quote;				
			}

			if (m_pEntdata[offset] <= ' ' && !quote)
				break;

			offset++;
		}

		token = std::string_view(&m_pEntdata[start], offset - start);
	};
	
	while (offset < (size_t)l->filelen)
	{
		grabToken();

		if (token.size() == 0)
			continue;

		if (token[0] == '{')
		{


			BSPEntity* pEntity = new BSPEntity(m_pScene);

			while (offset < (size_t)l->filelen)
			{
				grabToken();

				if (token[0] == '}')
					break;

				std::string key = std::string(token);
				grabToken();
				
				std::string value = std::string(token);

				// Strip quotes

				if (key.size() > 0 && key[0] == '"')
					key = key.substr(1, key.size() - 2);

				if (value.size() > 0 && value[0] == '"')
					value = value.substr(1, value.size() - 2);

				pEntity->SetKeyValue(key, value);

			}

			m_vEntities.push_back(pEntity);			
		}



	}
}

// TODO: delme
void BSPLevel::BuildSurfaceDisplayList(msurface_t *fa)
{
    int i, lindex, lnumverts;
    medge_t *pedges, *r_pedge;
    int vertpage;
    glm::vec3 vec;
    float s = 0, t = 0;
    glpoly_t *poly;

    // reconstruct the polygon
    pedges    = m_vEdges.data();
    lnumverts = fa->numedges;
    vertpage  = 0;

    //
    // draw texture
    //
    // poly = (glpoly_t*)Mem_Alloc(loadmodel->mem_pool, sizeof(glpoly_t) + (lnumverts - 4) * VERTEXSIZE *
    // sizeof(float));

    // TODO: это неправильно, надо разобраться в ядреном матане сверху, сейчас отжирает больше нужного
    poly = (glpoly_t *)new glpoly_t[lnumverts];

    poly->next     = fa->polys;
    poly->flags    = fa->flags;
    fa->polys      = poly;
    poly->numverts = lnumverts;

    for (i = 0; i < lnumverts; i++)
    {
        lindex = m_vSurfedges[fa->firstedge + i];

        if (lindex > 0)
        {
            r_pedge = &pedges[lindex];
            vec     = m_vVertices[r_pedge->v[0]];
        }
        else
        {
            r_pedge = &pedges[-lindex];
            vec     = m_vVertices[r_pedge->v[1]];
        }
        glm::vec3 ofs = fa->texinfo->vecs[0].xyz;
        s             = glm::dot(vec, ofs) + fa->texinfo->vecs[0][3];
        s /= fa->texinfo->texture->width;

        ofs = fa->texinfo->vecs[1].xyz;
        t   = glm::dot(vec, ofs) + fa->texinfo->vecs[1][3];
        t /= fa->texinfo->texture->height;

        *(glm::vec3 *)&poly->verts[i] = vec;
        poly->verts[i][3]             = s;
        poly->verts[i][4]             = t;

        //
        // lightmap texture coordinates
        //

        ofs = fa->texinfo->vecs[0].xyz;
        s   = glm::dot(vec, ofs) + fa->texinfo->vecs[0][3];
        s -= fa->texturemins[0];
        s += fa->light_s * 16;
        s += 8;
        s /= m_pLightmapState->BlockWidth() * 16;

        ofs = fa->texinfo->vecs[1].xyz;
        t   = glm::dot(vec, ofs) + fa->texinfo->vecs[1][3];
        t -= fa->texturemins[1];
        t += fa->light_t * 16;
        t += 8;
        t /= m_pLightmapState->BlockHeight() * 16; // fa->texinfo->texture->height;

        poly->verts[i][5] = s;
        poly->verts[i][6] = t;
    }

    //
    // remove co-linear points - Ed
    //
    // 	if (!(fa->flags & SURF_UNDERWATER))
    // 	{
    // 		for (i = 0; i < lnumverts; ++i)
    // 		{
    // 			vec3_t v1, v2;
    // 			float* prev, * me, * next;
    // 			float f;
    //
    // 			prev = poly->verts[(i + lnumverts - 1) % lnumverts];
    // 			me = poly->verts[i];
    // 			next = poly->verts[(i + 1) % lnumverts];
    //
    // 			VectorSubtract(me, prev, v1);
    // 			VectorNormalize(v1);
    // 			VectorSubtract(next, prev, v2);
    // 			VectorNormalize(v2);
    //
    // 			// skip co-linear points
    // #define COLINEAR_EPSILON 0.001
    // 			if ((fabs(v1[0] - v2[0]) <= COLINEAR_EPSILON) &&
    // 				(fabs(v1[1] - v2[1]) <= COLINEAR_EPSILON) &&
    // 				(fabs(v1[2] - v2[2]) <= COLINEAR_EPSILON))
    // 			{
    // 				int j;
    // 				for (j = i + 1; j < lnumverts; ++j)
    // 				{
    // 					int k;
    // 					for (k = 0; k < VERTEXSIZE; ++k)
    // 						poly->verts[j - 1][k] = poly->verts[j][k];
    // 				}
    // 				--lnumverts;
    // 				++nColinElim;
    // 				// retry next vertex next time, which is now current vertex
    // 				--i;
    // 			}
    // 		}
    // 	}

    poly->numverts = lnumverts;
}

msurface_t* GoldSource::BSPLevel::Faces(size_t firstSurface)
{
	return &m_vFaces[firstSurface];
}

const std::vector<GoldSource::dmodel_t> &BSPLevel::GetSubmodels() const
{
    return m_vSubmodels;
}

const std::vector<GoldSource::medge_t> &BSPLevel::GetEdges() const
{
    return m_vEdges;
}

const std::vector<GoldSource::mvertex_t> &BSPLevel::GetVertices() const
{
    return m_vVertices;
}

const std::vector<int> &BSPLevel::GetSurfEdges() const
{
    return m_vSurfedges;
}

GoldSource::LightmapAtlas *BSPLevel::GetLightmapState() const
{
    return m_pLightmapState;
}

std::vector<GoldSource::msurface_t> & BSPLevel::GetFaces()
{
    return m_vFaces;
}

void BSPLevel::PopulateScene(Scene * pScene)
{
	// TODO: design a proper way to link worldspawn and BSPWorld 
	//

    for (auto it : m_vEntities)
    {
        auto mod = it->GetModel();

		auto ptr = mod.lock();

		if (!ptr)
			continue;

		if (ptr->GetType() != ModelType::StaticLightmapped)
			continue;

        it->SetSerialNumber(pScene->AllocSerialNumber());
        it->PopulateScene();

		assert(it->GetOctreeNode());
    }

	
	// Entities now owned by scene
	m_vEntities.clear();
}

void BSPLevel::Mod_ReloadFacesLighting(lump_t* l)
{
	dface_t* in;
	msurface_t* out;
	int			i, count, surfnum;
	

	in = (dface_t*)(m_pFileData->Data() + l->fileofs);
	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadFaces", this);

	count = l->filelen / sizeof(*in);

	if (count != m_vFaces.size())
		throw new std::exception("Mod_ReloadFacesLighting: count != m_vFaces.size()");

	out = m_vFaces.data();
	//memset(out, 0, count * sizeof(*out));

	for (surfnum = 0; surfnum < count; surfnum++, in++, out++)
	{
		for (i = 0; i < MAXLIGHTMAPS; i++)
			out->styles[i] = in->styles[i];

		i = LittleLong(in->lightofs);

		if (i == -1)
			out->samples = NULL;
		else
		{

			if (m_Version == HL_BSPVERSION)
				out->samples = m_pLightData + i;
			else if (m_Version == Q1_BSPVERSION)
				out->samples = m_pLightData + i * 3;
		}

		if (out->samples)
			GL_CreateSurfaceLightmap(out);
	}
}


void BSPLevel::ReloadLightmaps()
{
	auto fs = Application::GetFileSystem();
	std::string name = m_pFileData->Name();

	m_pFileData->UnRef();

	m_pFileData = fs->LoadFile(name);

	m_Header = (dheader_t*)m_pFileData->Data();

	if (m_pLightData)
		delete m_pLightData;

	m_pLightmapState->Reset();

	Mod_LoadLighting(&m_Header->lumps[LUMP_LIGHTING]);
	Mod_ReloadFacesLighting(&m_Header->lumps[LUMP_FACES]);
}

std::string BSPLevel::Export(const char *newPath, lightBakerSettings_t *lb3kOptions)
{
	
	FILE *fpOut = fopen(m_pFileData->Name().c_str(), "wb");
	
	//fwrite(m_Header, sizeof(dheader_t), 1, fpOut);
	fwrite(m_pFileData->Data(), m_pFileData->Length(), 1, fpOut);

	size_t pos = ftell(fpOut);

	m_Header->lumps[LUMP_ENTITIES].fileofs = (uint32_t)pos;

	auto scene = Application::Instance()->GetMainWindow()->GetSceneRenderer()->GetScene();

	for (auto & ent : scene->GetSceneObjects())
	{
        auto rawPtr = ent.get();

		BSPEntity *pBSPEntity = dynamic_cast<BSPEntity *>(rawPtr);
		
		if (pBSPEntity)
		{
            //pBSPEntity->UpdateProperties();
            pBSPEntity->Export(fpOut);
		}
	}

	m_Header->lumps[LUMP_ENTITIES].filelen = ftell(fpOut) - (uint32_t)pos;

	fseek(fpOut, 0, SEEK_SET);
	fwrite(m_Header, sizeof(dheader_t), 1, fpOut);

	fclose(fpOut);

	return m_pFileData->Name();
}

void BSPLevel::BuildLightMap(msurface_t* surf, byte* dest, size_t stride)
{
	int			smax, tmax;
	int			r, g, b, a, max;
	int			i, j, size;
	byte* lightmap;
	float		scale[4];
	int			nummaps;
	float* bl;

	static float s_blocklights[34 * 16 * 34 * 3];

	smax = (surf->extents[0] >> 4) + 1;
	tmax = (surf->extents[1] >> 4) + 1;
	size = smax * tmax;

	if (!surf->samples) return;

	if (surf->texinfo->flags & (SURF_SKY | SURF_TRANS33 | SURF_TRANS66 | SURF_WARP))
		return;

	if (size > (sizeof(s_blocklights) >> 4))
		//throw new std::exception("Bad s_blocklights size %d (must be less than %d)", size, (sizeof(s_blocklights) >> 4));
		throw new std::exception("Bad s_blocklights size %d (must be less than %d)"); // TODO: FIXME

	// set to full bright if no light data
	if (!surf->samples)
		return;
	
	// count the # of maps
	for (nummaps = 0; nummaps < MAXLIGHTMAPS && surf->styles[nummaps] != 255; nummaps++);

	lightmap = surf->samples;

	// add all the lightmaps
	if (nummaps == 1)
	{
		int maps;

		for (maps = 0; maps < MAXLIGHTMAPS && surf->styles[maps] != 255;
			maps++)
		{
			bl = s_blocklights;

			for (i = 0; i < 3; i++)
				scale[i] = 1;

			if (scale[0] == 1.0F &&
				scale[1] == 1.0F &&
				scale[2] == 1.0F)
			{
				for (i = 0; i < size; i++, bl += 3)
				{
					bl[0] = lightmap[i * 3 + 0];
					bl[1] = lightmap[i * 3 + 1];
					bl[2] = lightmap[i * 3 + 2];
				}
			}
			else
			{
				for (i = 0; i < size; i++, bl += 3)
				{
					bl[0] = lightmap[i * 3 + 0] * scale[0];
					bl[1] = lightmap[i * 3 + 1] * scale[1];
					bl[2] = lightmap[i * 3 + 2] * scale[2];
				}
			}
			lightmap += size * 3;		// skip to next lightmap
		}
	}
	else
	{
		int maps;

		memset(s_blocklights, 0, sizeof(s_blocklights[0]) * size * 3);

		for (maps = 0; maps < MAXLIGHTMAPS && surf->styles[maps] != 255;
			maps++)
		{
			bl = s_blocklights;
			// 
			for (i = 0; i < 3; i++)
				scale[i] = 1.0f;

			if (scale[0] == 1.0F &&
				scale[1] == 1.0F &&
				scale[2] == 1.0F)
			{
				for (i = 0; i < size; i++, bl += 3)
				{
					bl[0] += lightmap[i * 3 + 0];
					bl[1] += lightmap[i * 3 + 1];
					bl[2] += lightmap[i * 3 + 2];
				}
			}
			else
			{
				for (i = 0; i < size; i++, bl += 3)
				{
					bl[0] += lightmap[i * 3 + 0] * scale[0];
					bl[1] += lightmap[i * 3 + 1] * scale[1];
					bl[2] += lightmap[i * 3 + 2] * scale[2];
				}
			}
			lightmap += size * 3;		// skip to next lightmap
		}
	}

	// add all the dynamic lights
	// 	if (surf->dlightframe == r_framecount)
	// 		R_AddDynamicLights (surf);

	// put into texture format

	stride -= (smax << 2);
	bl = s_blocklights;

	

	for (i = 0; i < tmax; i++, dest += stride)
	{
		for (j = 0; j < smax; j++)
		{

			r = int(bl[0]);
			g = int(bl[1]);
			b = int(bl[2]);

			// catch negative lights
			if (r < 0)
				r = 0;
			if (g < 0)
				g = 0;
			if (b < 0)
				b = 0;

			/*
			** determine the brightest of the three color components
			*/
			if (r > g)
				max = r;
			else
				max = g;
			if (b > max)
				max = b;

			/*
			** alpha is ONLY used for the mono lightmap case.  For this reason
			** we set it to the brightest of the color components so that
			** things don't get too dim.
			*/
			a = max;

			/*
			** rescale all the color components if the intensity of the greatest
			** channel exceeds 1.0
			*/
			if (max > 255)
			{
				float t = 255.0F / max;

				r = r * t;
				g = g * t;
				b = b * t;
				a = a * t;
			}

			dest[0] = r;
			dest[1] = g;
			dest[2] = b;
			dest[3] = a;

			bl += 3;
			dest += 4;
		}
	}
	
	
}

mnode_t* BSPLevel::GetNodes(size_t index)
{
	return &m_vNodes[index];
}

GoldSource::FunnyLumpSizeException::FunnyLumpSizeException(const char* function, BSPLevel* level)
{
	message = std::format("{0}: funny lump size in {1}.bsp", message, level->GetBaseName());
	m_pLevel = level;
}


GoldSource::FunnyLumpSizeException::~FunnyLumpSizeException()
{
	
}
