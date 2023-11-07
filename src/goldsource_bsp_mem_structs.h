/*
    Edited for LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

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

#pragma once

namespace GoldSource
{

#define SURF_PLANEBACK		2
#define SURF_DRAWSKY		4 
#define SURF_DRAWSPRITE 	8
#define SURF_DRAWTURB		0x10
#define SURF_DRAWTILED		0x20
#define SURF_DRAWBACKGROUND 0x40
#define SURF_UNDERWATER 	0x80
#define SURF_DONTWARP		0x100

#define	SURF_SKY		0x4		// don't draw, but add to skybox
#define	SURF_WARP		0x8		// turbulent water warp
#define	SURF_TRANS33	0x10
#define	SURF_TRANS66	0x20
#define	SURF_FLOWING	0x40	// scroll towards angle
#define	SURF_NODRAW		0x80	// don't bother referencing the texture

#define	SURF_ALPHA_TRANSPARENT		(1<<0)
#define	SURF_WATER		(1<<1)
#define SURF_REFLECTIVE_WATER (1<<2)	

#define MAXLIGHTMAPS 4
#define	VERTEXSIZE	7
#define MIPLEVELS 4
#define NUM_AMBIENTS 4


typedef glm::vec3 mvertex_t;



typedef struct mtexture_s
{
    char		name[16];
    unsigned	width, height;
    
    gltexture_t* loadedTexture;


    struct msurface_s* texturechain;	// for gl_texsort drawing
    int			anim_total;				// total tenths in sequence ( 0 = no)
    int			anim_min, anim_max;		// time for this frame min <=time< max
    struct mtexture_s* anim_next;		// in the animation sequence
    struct mtexture_s* alternate_anims;	// bmodels in frmae 1 use these
    unsigned	offsets[MIPLEVELS];		// four mip maps stored
} mtexture_t;

typedef struct mplane_s
{
    glm::vec3	normal;
    float		dist;
    byte		type;			// for texture axis selection and fast side tests
    byte		signbits;		// signx + signy<<1 + signz<<1
    byte		pad[2];
} mplane_t;

typedef struct
{
    unsigned short	v[2];
    unsigned int	cachededgeoffset;
} medge_t;

typedef struct glpoly_s
{
    struct	glpoly_s* next;
    struct	glpoly_s* chain;
    int		numverts;
    int		flags;			// for SURF_UNDERWATER
    float	verts[4][VERTEXSIZE];	// variable sized (x y z s1 t1 s2 t2)
} glpoly_t;



typedef struct
{
    glm::vec4		vecs[2];
    float		mipadjust;
    mtexture_t* texture;
    int local_texture_idx;
    int			flags;
    int flags2;
} mtexinfo_t;

typedef struct msurface_s
{
    int         visframe;       // should be drawn when node is crossed
    mplane_t* plane;
    int         flags;

    int         firstedge;  // look up in model->surfedges[], negative numbers
    int         numedges;   // are backwards edges

    short       texturemins[2];
    short       extents[2];

    int         light_s, light_t;           // gl lightmap coordinates

    glpoly_t* polys;                     // multiple if warped
    struct     msurface_s* texturechain;
    mtexinfo_t* texinfo;

	// lighting info
	int         dlightframe;
	long         dlightbits;

	int         lightmaptexturenum;
	byte        styles[MAXLIGHTMAPS];
	int         cached_light[MAXLIGHTMAPS]; // values currently used in lightmap
	bool        cached_dlight;              // true if dynamic light in cache

    byte* samples;                   // note: this is the actual lightmap data for this surface

    float min[2], max[2];
    
    
} msurface_t;

typedef struct 
{
    byte r, g, b;
}color24_t;

typedef struct mleaf_s
{
	// common with node
	int			contents;		// wil be a negative contents number
	int			visframe;		// node needs to be traversed if current

	short		minmaxs[6];		// for bounding box culling

	struct mnode_s* parent;

	// leaf specific
	byte* compressed_vis;
	struct efrag_s* efrags;

	msurface_t** firstmarksurface;
	int			nummarksurfaces;
	int			key;			// BSP sequence number for leaf's contents
	byte		ambient_sound_level[NUM_AMBIENTS];
} mleaf_t;

typedef struct mnode_s
{
	// common with leaf
	int			contents;		// 0, to differentiate from leafs
	//int			visframe;		// node needs to be traversed if current
	size_t          visframe;		// node needs to be traversed if current

	short		minmaxs[6];		// for bounding box culling

	struct mnode_s* parent;

	// node specific
	mplane_t* plane;
	struct mnode_s* children[2];

	unsigned short		firstsurface;
	unsigned short		numsurfaces;
} mnode_t;

typedef struct
{
	dclipnode_t*    clipnodes;
	mplane_t*       planes;
	int          	firstclipnode;
	int          	lastclipnode;
	glm::vec3    	clip_mins;
    glm::vec3       clip_maxs;
} hull_t;


}