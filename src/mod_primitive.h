/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/


#pragma once
#include "mod_manager.h"

class DrawMesh;

enum class CommonPrimitives
{
    Box,
    Sphere,
    Cone,
    Total

};

class PrimitiveModel : public IModel
{
    DrawMesh *m_pDrawMesh = nullptr;
    CommonPrimitives m_Type;

    CommonPrimitives GetPrimitiveType(const char *fileName);

public:
    PrimitiveModel(const char * fileName);
    ~PrimitiveModel();

    const std::optional<BoundingBox> GetBoundingBox() const override;
    void                             Render(SceneEntity *pEntity, SceneRenderer *sr, RenderMode mode) override;

    static IModelWeakPtr LookupByType(CommonPrimitives type);
};