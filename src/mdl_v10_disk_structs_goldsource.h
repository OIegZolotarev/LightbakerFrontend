/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "mathlib.h"

namespace GoldSource
{
typedef struct dstudiolump_s
{
    int num_items;
    int index;
} dstudiolump_t;

typedef struct dstudiohdrv10_s
{
    int magic;
    int version;

    char name[64];
    int  length;

    glm::vec3 eyepos;
    glm::vec3 min;
    glm::vec3 max;

    int flags;

    dstudiolump_s bones;
    dstudiolump_t hitboxes;
    dstudiolump_t sequences;
    dstudiolump_t seqgroups;
    dstudiolump_t textures;

    int texturedata_index;
    int num_skinref;

    dstudiolump_t skins;

    dstudiolump_t bodyparts;
    dstudiolump_t attachments;

    dstudiolump_t unused1;
    dstudiolump_t unused2;

    dstudiolump_t transitions;

} dstudiohdrv10_t;

typedef struct dstudioseqhdr10_s
{
    int id;
    int version;

    char name[64];
    int  length;
} dstudioseqhdr10_t;

// bones
typedef struct dstudiobone10_s
{
    char  name[32];          // bone name for symbolic links
    int   parent;            // parent bone
    int   flags;             // ??
    int   bonecontroller[6]; // bone controller index, -1 == none
    float value[6];          // default DoF values
    float scale[6];          // scale for delta DoF values
} dstudiobone10_t;

// bone controllers
typedef struct dstudiobonecontroller10_s
{
    int   bone; // -1 == 0
    int   type; // X, Y, Z, XR, YR, ZR, M
    float start;
    float end;
    int   rest;  // byte index value at rest
    int   index; // 0-3 user set controller, 4 mouth
} dstudiobonecontroller10_t;

// intersection boxes
typedef struct dstudiobbox10_t
{
    int       bone;
    int       group; // intersection group
    glm::vec3 bbmin; // bounding box
    glm::vec3 bbmax;
} dstudiobbox10_t;

// demand loaded sequence groups
typedef struct dstudioseqgroup10_s
{
    char label[32]; // textual name
    char name[64];  // file name
    int  cache;     // cache index pointer
    int  data;      // hack for group 0
} dstudioseqgroup10_t;

// sequence descriptions
typedef struct dstudioseqdesc10_s
{
    char label[32]; // sequence label

    float fps;   // frames per second
    int   flags; // looping/non-looping flags

    int activity;
    int actweight;

    int numevents;
    int eventindex;

    int numframes; // number of frames per sequence

    int numpivots; // number of foot pivots
    int pivotindex;

    int       motiontype;
    int       motionbone;
    glm::vec3 linearmovement;
    int       automoveposindex;
    int       automoveangleindex;

    glm::vec3 bbmin; // per sequence bounding box
    glm::vec3 bbmax;

    int numblends;
    int animindex; // mstudioanim_t pointer relative to start of sequence group data
                   // [blend][bone][X, Y, Z, XR, YR, ZR]

    int   blendtype[2];  // X, Y, Z, XR, YR, ZR
    float blendstart[2]; // starting value
    float blendend[2];   // ending value
    int   blendparent;

    int seqgroup; // sequence group for demand loading

    int entrynode; // transition node at entry
    int exitnode;  // transition node at exit
    int nodeflags; // transition rules

    int nextseq; // auto advancing sequences
} dstudioseqdesc10_t;

// events
typedef struct dstudioevent10_s
{
    int  frame;
    int  event;
    int  type;
    char options[64];
} dstudioevent10_t;

// pivots
typedef struct dstudiopivot10_s
{
    glm::vec3 org; // pivot point
    int       start;
    int       end;
} dstudiopivot10_t;

// attachment
typedef struct dstudioattachment10_s
{
    char      name[32];
    int       type;
    int       bone;
    glm::vec3 org; // attachment point
    glm::vec3 vectors[3];
} dstudioattachment10_t;

typedef struct dstudioanim10_s
{
    unsigned short offset[6];
} dstudioanim10_t;

// animation frames
typedef union dstudioanimvalue10_s {
    struct
    {
        byte valid;
        byte total;
    } num;
    short value;
} dstudioanimvalue10_t;

// body part index
typedef struct dstudiobodyparts10_s
{
    char name[64];
    int  nummodels;
    int  base;
    int  modelindex; // index into models array
} dstudiobodyparts10_t;

// skin info
typedef struct dstudiotexture10_s
{
    char name[64];
    int  flags;
    int  width;
    int  height;
    int  index;
} dstudiotexture10_t;

// studio models
typedef struct dstudiomodel10_s
{
    char name[64];

    int type;

    float boundingradius;

    int nummesh;
    int meshindex;

    int numverts;      // number of unique vertices
    int vertinfoindex; // vertex bone info
    int vertindex;     // vertex vec3_t

    int numnorms;      // number of unique surface normals
    int norminfoindex; // normal bone info
    int normindex;     // normal vec3_t

    int numgroups; // deformation groups
    int groupindex;
} dstudiomodel10_t;

// meshes
typedef struct dstudiomesh10_s
{
    int numtris;
    int triindex;
    int skinref;
    int numnorms;  // per mesh normals
    int normindex; // normal vec3_t
} dstudiomesh10_t;

// triangles

typedef struct dstudiotricmd10_s
{
    short vertindex; // index into vertex array
    short normindex; // index into normal array
    short s, t;      // s,t position on skin
} dstudiotricmd10_t;

} // namespace GoldSource