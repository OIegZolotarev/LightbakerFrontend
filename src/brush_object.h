/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once

#include "common.h"
#include "brush_face.h"
#include "gl_shader.h"
#include "scene_entity.h"
#include "hammer_map.h"

class DrawMesh;
class BrushFace;

class BrushModel : public IModel
{
    std::list<BrushFace *> m_lstFaces;

    // Faces helpers
    void       RemoveFace(size_t idx);
    BrushFace *GetFace(size_t idx);

    void RemoveInvalidFaces();

    DrawMesh *m_pDrawMesh = nullptr;
    bool      m_bValid = false;

    BoundingBox m_Bounds;

public:

    BrushModel(const char* modelName);

    const std::optional<BoundingBox> GetBoundingBox() const override;
    void Render(SceneEntity *pEntity, const SceneRenderer *sr, RenderMode mode, ShaderProgram *currentShader) override;

    DrawMesh *GetDrawMesh();

    BrushFace* AddFace(const glm::vec3 pts[3]);
    bool CreateFacesFromPlanes(int flags);

    void AddPointsToBounds(const glm::vec3 &pt);

};

//
// Flags for CreateFromPlanes:
//
#define CREATE_BUILD_PLANE_POINTS   (1 << 0) // Whether to regenerate the 3 plane points from the generated face points.
#define CREATE_FROM_PLANES_CLIPPING (1 << 1)

class BrushObject : public SceneEntity
{
    std::shared_ptr<BrushModel> m_pModel;
public:

    BrushObject(Scene *pScene);
    BrushObject(BrushObject *pOther);
    ~BrushObject();

    SceneEntity *Clone() override;
    void GenerateModel(const HammerMap *pMap, const size_t firstFace, const size_t lastFace, size_t index);

    void Render(RenderMode mode, const SceneRenderer *sr, ShaderProgram *shader) override;
};