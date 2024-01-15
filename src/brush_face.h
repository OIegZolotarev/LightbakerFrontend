/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once
#include "gl_texture.h"

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

#define FL_FACE_REBUILT_FROM_PLANE (1 << 0)

#define TEXTURE_AXIS_ROUND_EPSILON 0.001

typedef struct brushFaceTextureInfo_s
{
    char      textureName[32];
    uint32_t  lm_scale;
    glm::vec4 uaxis;
    glm::vec4 vaxis;
    glm::vec2 scale;
    float     rotation;
} brushFaceTextureInfo_t;

class BrushFace
{
    GLTexture *m_pTexture = nullptr;
    brushFaceTextureInfo_t m_TexInfo;

    plane_t m_Plane;

public:
    BrushFace();
};
