/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "mathlib.h"
#include <stdint.h>

namespace GoldSource
{

#define MAXSTUDIOBONES 128

typedef struct dstudiolump_s
{
    int32_t num_items;
    int32_t index;
} dstudiolump_t;

typedef struct dstudiohdrv10_s
{
    int32_t magic;
    int32_t version;

    char    name[64];
    int32_t length;

    glm::vec3 eyepos;
    glm::vec3 min;
    glm::vec3 max;

    glm::vec3 bbmin;
    glm::vec3 bbmax;

    int32_t flags;

    dstudiolump_s bones;
    dstudiolump_s bonecontrollers;
    dstudiolump_t hitboxes;
    dstudiolump_t sequences;
    dstudiolump_t seqgroups;
    dstudiolump_t textures;

    int32_t texturedata_index;
    int32_t num_skinref;
    int32_t num_skins;
    int32_t skins_index;

    

    dstudiolump_t bodyparts;
    dstudiolump_t attachments;

    dstudiolump_t unused1;
    dstudiolump_t unused2;

    dstudiolump_t transitions;

} dstudiohdrv10_t;

typedef struct dstudioseqhdr10_s
{
    int32_t id;
    int32_t version;

    char    name[64];
    int32_t length;
} dstudioseqhdr10_t;

// bones
typedef struct dstudiobone10_s
{
    char    name[32];          // bone name for symbolic links
    int32_t parent;            // parent bone
    int32_t flags;             // ??
    int32_t bonecontroller[6]; // bone controller index, -1 == none
    float   value[6];          // default DoF values
    float   scale[6];          // scale for delta DoF values
} dstudiobone10_t;

// bone controllers
typedef struct dstudiobonecontroller10_s
{
    int32_t bone; // -1 == 0
    int32_t type; // X, Y, Z, XR, YR, ZR, M
    float   start;
    float   end;
    int32_t rest;  // byte index value at rest
    int32_t index; // 0-3 user set controller, 4 mouth
} dstudiobonecontroller10_t;

// int32_tersection boxes
typedef struct dstudiobbox10_t
{
    int32_t   bone;
    int32_t   group; // int32_tersection group
    glm::vec3 bbmin; // bounding box
    glm::vec3 bbmax;
} dstudiobbox10_t;

// demand loaded sequence groups
typedef struct dstudioseqgroup10_s
{
    char    label[32]; // textual name
    char    name[64];  // file name
    int32_t cache;     // cache index point32_ter
    int32_t data;      // hack for group 0
} dstudioseqgroup10_t;

// sequence descriptions
typedef struct dstudioseqdesc10_s
{
    char label[32]; // sequence label

    float   fps;   // frames per second
    int32_t flags; // looping/non-looping flags

    int32_t activity;
    int32_t actweight;

    int32_t numevents;
    int32_t eventindex;

    int32_t numframes; // number of frames per sequence

    int32_t numpivots; // number of foot pivots
    int32_t pivotindex;

    int32_t   motiontype;
    int32_t   motionbone;
    glm::vec3 linearmovement;
    int32_t   automoveposindex;
    int32_t   automoveangleindex;

    glm::vec3 bbmin; // per sequence bounding box
    glm::vec3 bbmax;

    int32_t numblends;
    int32_t animindex; // mstudioanim_t point32_ter relative to start of sequence group data
                       // [blend][bone][X, Y, Z, XR, YR, ZR]

    int32_t blendtype[2];  // X, Y, Z, XR, YR, ZR
    float   blendstart[2]; // starting value
    float   blendend[2];   // ending value
    int32_t blendparent;

    int32_t seqgroup; // sequence group for demand loading

    int32_t entrynode; // transition node at entry
    int32_t exitnode;  // transition node at exit
    int32_t nodeflags; // transition rules

    int32_t nextseq; // auto advancing sequences
} dstudioseqdesc10_t;

// events
typedef struct dstudioevent10_s
{
    int32_t frame;
    int32_t event;
    int32_t type;
    char    options[64];
} dstudioevent10_t;

// pivots
typedef struct dstudiopivot10_s
{
    glm::vec3 org; // pivot point32_t
    int32_t   start;
    int32_t   end;
} dstudiopivot10_t;

// attachment
typedef struct dstudioattachment10_s
{
    char      name[32];
    int32_t   type;
    int32_t   bone;
    glm::vec3 org; // attachment point32_t
    glm::vec3 vectors[3];
} dstudioattachment10_t;

typedef struct dstudioanim10_s
{
    uint16_t offset[6];
} dstudioanim10_t;

// animation frames
typedef union dstudioanimvalue10_s {
    struct
    {
        byte valid;
        byte total;
    } num;
    int16_t value;
} dstudioanimvalue10_t;

// body part index
typedef struct dstudiobodyparts10_s
{
    char    name[64];
    int32_t nummodels;
    int32_t base;
    int32_t modelindex; // index int32_to models array
} dstudiobodypart10_t;

// skin info
typedef struct dstudiotexture10_s
{
    char    name[64];
    int32_t flags;
    int32_t width;
    int32_t height;
    int32_t index;
} dstudiotexture10_t;

// studio models
typedef struct dstudiomodel10_s
{
    char name[64];

    int32_t unused1;
    float   unused2;

    int32_t nummesh;
    int32_t meshindex;

    int32_t numverts;      // number of unique vertices
    int32_t vertinfoindex; // vertex bone info
    int32_t vertindex;     // vertex vec3_t

    int32_t numnorms;      // number of unique surface normals
    int32_t norminfoindex; // normal bone info
    int32_t normindex;     // normal vec3_t

    int32_t unused3; // deformation groups
    int32_t unused4;
} dstudiomodel10_t;

// meshes
typedef struct dstudiomesh10_s
{
    int32_t numtris;
    int32_t triindex;
    int32_t skinref;
    int32_t numnorms;  // per mesh normals
    int32_t normindex; // normal vec3_t
} dstudiomesh10_t;

// triangles

typedef struct dstudiotricmd10_s
{
    int16_t vertindex; // index int32_to vertex array
    int16_t normindex; // index int32_to normal array
    int16_t s, t;      // s,t position on skin
} dstudiotricmd10_t;

// motion flags
#define STUDIO_X     0x0001
#define STUDIO_Y     0x0002
#define STUDIO_Z     0x0004
#define STUDIO_XR    0x0008
#define STUDIO_YR    0x0010
#define STUDIO_ZR    0x0020
#define STUDIO_LX    0x0040
#define STUDIO_LY    0x0080
#define STUDIO_LZ    0x0100
#define STUDIO_AX    0x0200
#define STUDIO_AY    0x0400
#define STUDIO_AZ    0x0800
#define STUDIO_AXR   0x1000
#define STUDIO_AYR   0x2000
#define STUDIO_AZR   0x4000
#define STUDIO_TYPES 0x7FFF
#define STUDIO_RLOOP 0x8000 // controller that wraps shortest distance


} // namespace GoldSource