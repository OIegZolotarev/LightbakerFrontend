/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "goldsource_bsp_world.h"
#include "byteorder.h"
#include "wad_textures.h"

using namespace GoldSource;


void BSPWorld::Mod_LoadVertexes(lump_t* l)
{
	dvertex_t* in;
	mvertex_t* out;
	int			i, count;

	in = (dvertex_t*)(m_pFileData->Data() + l->fileofs);
	
	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadVertexes", this);

	count = l->filelen / sizeof(*in);

	m_vVertices.reserve(count);
	out = (mvertex_t*)m_vVertices.data();

	for (i = 0; i < count; i++, in++, out++)
	{
		out->x = LittleFloat(in->point[0]);
		out->y = LittleFloat(in->point[1]);
		out->z = LittleFloat(in->point[2]);
	}
}

void BSPWorld::Mod_LoadEdges(lump_t* l)
{
	dedge_t* in;
	medge_t* out;
	int 	i, count;

	in = (dedge_t*)(m_pFileData->Data() + l->fileofs);
	
	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadEdges", this); 

	count = l->filelen / sizeof(*in);
	m_vEdges.reserve(count);

	out = m_vEdges.data();

	for (i = 0; i < count; i++, in++, out++)
	{
		out->v[0] = (unsigned short)LittleShort(in->v[0]);
		out->v[1] = (unsigned short)LittleShort(in->v[1]);
	}
}

void BSPWorld::Mod_LoadSurfedges(lump_t* l)
{
	int		i, count;
	int* in, *out;

	in = (int*)(m_pFileData->Data() + l->fileofs);
	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadSurfEdges",this);
		
	
	count = l->filelen / sizeof(*in);		
	
	m_vSurfedges.reserve(count);
	out = m_vSurfedges.data();

	for (i = 0; i < count; i++)
		out[i] = LittleLong(in[i]);
}

void BSPWorld::Mod_LoadTextures(lump_t* l)
{	
// 	miptex_t* mt;
 	texture_t* tx, * tx2;
 	texture_t* anims[10];
 	texture_t* altanims[10];
// 	dmiptexlump_t* m;
 	int max = 0;
	int j;
	int num;
	int altmax;

	if (!l->filelen)
	{		
		return;
	}
	
	dmiptexlump_t* m = (dmiptexlump_t*)(m_pFileData->Length() + l->fileofs);
	m->nummiptex = LittleLong(m->nummiptex);

	m_vTextures.reserve(m->nummiptex);

	for (int i = 0; i < m->nummiptex; i++)
	{
		m->dataofs[i] = LittleLong(m->dataofs[i]);
		// 		if (m->dataofs[i] == -1)
		// 			continue;

		miptex_t* mt = (miptex_t*)((byte*)m + m->dataofs[i]);
		mt->width = LittleLong(mt->width);
		mt->height = LittleLong(mt->height);

		texture_t * tx = new texture_t;
		memset(tx, 0, sizeof(tx));
		
		m_vTextures[i] = tx;
		memcpy(tx->name, mt->name, sizeof(tx->name));

		if (mt->offsets[0])
			tx->gl_texturenum = LoadMiptex(mt);


// 		if (1)
// 		{
// 
// 			glGenTextures(1, (GLuint*)&tx->gl_normal_texture_num);
// 			GL_PrepareNormalGenerator(&tx->gl_normal_texture_num, tx->name);
// 
// 			if (loadmodel->version == 29)
// 			{
// 				tx->gl_texturenum = glLoadJpg(VA("textures/world/%s.jpg", mt->name));
// 			}
// 			else
// 			{
// 				if (mt->offsets[0])
// 					tx->gl_texturenum = LoadMiptex(mt);
// 				else if (!LoadWadTex(tx->name, &tx->gl_texturenum))
// 					tx->gl_texturenum = glLoadJpg(VA("textures/world/%s.jpg", mt->name));
// 			}
// 
// 			ML_LoadTexture(mt->name, tx);
// 
// 		}
// 		LoadAviForTexture(tx);
// 
// 		//	tx->gl_texturenum=glUniload("gg@!hb_display");
// 
// 		tx->height = mt->height;
// 		tx->width = mt->width;
// 
// 		if (!tx->gl_texturenum || tx->height == 0xFFFFFFFF || tx->width == 0xFFFFFFFF || !tx->width || !tx->height)
// 		{
// 			tx->gl_texturenum = R_CachedPic("EMO_TEXTURE");
// 			tx->height = 64;
// 			tx->width = 64;
// 		}
// 
// 
// 
// 		if (!stricmp(tx->name, "sky")) R_LoadSky();
	}

	SequenceTextureAnims(m, tx, anims, altanims, max, altmax, j, tx2, num);

}

void BSPWorld::Mod_LoadLighting(lump_t* l)
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

void BSPWorld::Mod_LoadPlanes(lump_t* l)
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

	m_vPlanes.reserve(count);

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

void BSPWorld::Mod_LoadTexinfo(lump_t* l)
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
	m_vTexInfo.reserve(count);

	out = m_vTexInfo.data();

	for (i = 0; i < count; i++, in++, out++)
	{
		for (j = 0; j < 8; j++)
			out->vecs[0][j] = LittleFloat(in->vecs[0][j]);
		
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

void BSPWorld::Mod_LoadFaces(lump_t* l)
{
	dface_t* in;
	msurface_t* out;
	int			i, count, surfnum;
	int			planenum, side;

	in = (dface_t*)(m_pFileData->Data() + l->fileofs);
	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadFaces", this);

	count = l->filelen / sizeof(*in);

	m_vFaces.reserve(count);

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
					
		
		// TODO: implement
		//if (out->samples)	
			//GL_CreateSurfaceLightmap(out);
	}
}

void BSPWorld::Mod_LoadMarksurfaces(lump_t* l)
{
	size_t i, j, count;
	short* in;
	msurface_t** out;

	in = (short*)(m_pFileData->Data() + l->fileofs);

	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadMarksurfaces",this);


	count = l->filelen / sizeof(*in);
	m_vMarkSurfaces.reserve(count);
	out = m_vMarkSurfaces.data();

	for (i = 0; i < count; i++)
	{
		j = LittleShort(in[i]);
		if (j >= m_vFaces.size())
			throw new std::exception("Mod_ParseMarksurfaces: bad surface number");
		
		out[i] = &m_vFaces[j];
	}
}

void BSPWorld::Mod_LoadVisibility(lump_t* l)
{
	if (!l->filelen)
	{
		m_pVisdata = nullptr;
		return;
	}
	
	m_pVisdata = new byte[l->filelen];
	memcpy(m_pVisdata, m_pFileData->Data() + l->fileofs, l->filelen);
}

void BSPWorld::Mod_LoadLeafs(lump_t* l)
{
	dleaf_t* in;
	mleaf_t* out;
	int			i, j, count, p;

	in = (dleaf_t*)(m_pFileData->Data() + l->fileofs);
	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadLeafs",this);
	
	count = l->filelen / sizeof(*in);
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

		out->firstmarksurface = &m_vMarkSurfaces[LittleShort(in->firstmarksurface)];
		out->nummarksurfaces = LittleShort(in->nummarksurfaces);

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

void BSPWorld::Mod_LoadNodes(lump_t* l)
{
	int			i, j, count, p;
	dnode_t* in;
	mnode_t* out;

	in = (dnode_t*)(m_pFileData->Data() + l->fileofs);
	
	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadNodes",this);
	count = l->filelen / sizeof(*in);

	m_vNodes.reserve(count);
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

void BSPWorld::Mod_LoadClipnodes(lump_t* l)
{
	dclipnode_t* in, * out;
	int			i, count;
	//hull_t* hull;

	in = (dclipnode_t*)(m_pFileData->Data() + l->fileofs);
	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadClipnodes", this);
	
	count = l->filelen / sizeof(*in);
	
	m_vClipNodes.reserve(count);
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

void BSPWorld::Mod_LoadEntities(lump_t* l)
{

	// TODO: implement app-specific parser

// 	if (!l->filelen)
// 	{
// 		loadmodel->entities = NULL;
// 		return;
// 	}
// 	loadmodel->entities = (char*)Mem_Alloc(loadmodel->mem_pool, l->filelen);
// 	memcpy(loadmodel->entities, mod_base + l->fileofs, l->filelen);
}

void BSPWorld::Mod_LoadSubmodels(lump_t* l)
{
	dmodel_t* in;
	dmodel_t* out;
	int			i, j, count;

	in = (dmodel_t*)(m_pFileData->Data() + l->fileofs);
	if (l->filelen % sizeof(*in))
		throw new FunnyLumpSizeException("Mod_LoadSubmodels", this);

	count = l->filelen / sizeof(*in);
	
	m_vSubmodels.reserve(count);
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

BSPWorld::BSPWorld(FileData * fd)
{
	m_pFileData = fd;
	m_Header = (dheader_t*)fd->Data();


	Mod_LoadVertexes(&m_Header->lumps[LUMP_VERTEXES]);
	Mod_LoadEdges(&m_Header->lumps[LUMP_EDGES]);
	Mod_LoadSurfedges(&m_Header->lumps[LUMP_SURFEDGES]);

	Mod_LoadTextures(&m_Header->lumps[LUMP_TEXTURES]);
	Mod_LoadLighting(&m_Header->lumps[LUMP_LIGHTING]);
	Mod_LoadPlanes(&m_Header->lumps[LUMP_PLANES]);
	Mod_LoadTexinfo(&m_Header->lumps[LUMP_TEXINFO]);
	Mod_LoadFaces(&m_Header->lumps[LUMP_FACES]);
	Mod_LoadMarksurfaces(&m_Header->lumps[LUMP_MARKSURFACES]);
	Mod_LoadVisibility(&m_Header->lumps[LUMP_VISIBILITY]);
	Mod_LoadLeafs(&m_Header->lumps[LUMP_LEAFS]);
	Mod_LoadNodes(&m_Header->lumps[LUMP_NODES]);
	Mod_LoadClipnodes(&m_Header->lumps[LUMP_CLIPNODES]);
	Mod_LoadEntities(&m_Header->lumps[LUMP_ENTITIES]);

	Mod_LoadSubmodels(&m_Header->lumps[LUMP_MODELS]);
	
}

BSPWorld::~BSPWorld()
{
	if (m_pVisdata) delete m_pVisdata;
	if (m_pLightData) delete m_pLightData;

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
}

std::string BSPWorld::GetBaseName()
{
	return m_pFileData->BaseName();	
}

void BSPWorld::SequenceTextureAnims(dmiptexlump_t* m, texture_t*& tx, texture_t** anims, texture_t** altanims, int& max, int& altmax, int& j, texture_t*& tx2, int& num)
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

void BSPWorld::CalcSurfaceExtents(msurface_t* s)
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

void BSPWorld::Mod_SetParent(mnode_t* node, mnode_t* parent)
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

GoldSource::FunnyLumpSizeException::FunnyLumpSizeException(const char* function, BSPWorld* level)
{
	message = std::format("{0}: funny lump size in {1}.bsp", message, level->GetBaseName());
	m_pLevel = level;
}


GoldSource::FunnyLumpSizeException::~FunnyLumpSizeException()
{
	
}
