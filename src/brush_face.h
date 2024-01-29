/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once
#include "brush_object.h"
#include "gl_texture.h"
#include "material_assets.h"

enum class FaceOrientation
{
    Floor = 0,
    Ceiling,
    North,
    South,
    East,
    West,
    Invalid
};

enum class TextureAlignment
{
    None  = 0x0000,
    World = 0x0001,
    Face  = 0x0002,
    Quake = 0x0004
};

enum class TextureJustification
{
    None = 0,
    Top,
    Bottom,
    Left,
    Center,
    Right,
    Fit
};

// Stuff taken from Instant Lammer Editor, deal with it B-)
//
// Flags for ILE_MapFace::CopyFrom.
//
#define COPY_FACE_PLANE  (1 << 0) // Copies only the face's plane. Used for carving.
#define COPY_FACE_POINTS (1 << 1) // Copies the face's points and plane.

#define INIT_TEXTURE_FORCE    (1 << 0)
#define INIT_TEXTURE_AXES     (1 << 1)
#define INIT_TEXTURE_ROTATION (1 << 2)
#define INIT_TEXTURE_SHIFT    (1 << 3)
#define INIT_TEXTURE_SCALE    (1 << 4)

#define INIT_TEXTURE_ALL (INIT_TEXTURE_AXES | INIT_TEXTURE_ROTATION | INIT_TEXTURE_SHIFT | INIT_TEXTURE_SCALE)

//
// Flags for CreateFaceFromWinding.
//
#define CREATE_FACE_PRESERVE_PLANE                                                                                     \
    (1 << 0) // Hack to prevent plane from being recalculated while building a solid from its planes.
#define CREATE_FACE_CLIPPING (1 << 1)

#define TEXTURE_AXIS_ROUND_EPSILON 0.001

// TODO: review
#define MAPSOLID_MAX_FACES 512

typedef struct brushFaceTextureInfo_s

{
    char      textureName[32];
    uint32_t  lm_scale;
    glm::vec4 uaxis;
    glm::vec4 vaxis;
    glm::vec2 scale;
    float     rotation;
} brushFaceTextureInfo_t;

class BrushObject;
class BrushModel;
class Winding;

class BrushFace
{
    MaterialAssetPtr m_pMaterialAsset;

    brushFaceTextureInfo_t m_TexInfo;

    plane_t m_Plane;

    BrushModel *m_pModel;

    void ValidateTexturingInfo();

    friend class BrushModel;

    size_t m_FaceId = 0;

public:
    BrushFace();
    BrushFace(BrushModel *pBrush, const glm::vec3 pts[3]);

    void SetTexture(GLTexture *pTexture);

    const plane_t *GetPlane() const;
    void           CreateFaceFromWinding(Winding *w, int flags);
    void           SetUAxis(glm::vec4 uAxis);
    void           SetVAxis(glm::vec4 vAxis);
};
