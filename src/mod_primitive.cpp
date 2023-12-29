/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "mod_primitive.h"
#include "application.h"
#include "common.h"
#include "draw_utils.h"
#include "helpers.h"

#include "mathlib.h"

const char *g_PrimitiveFiles[] = {"primitive/box.prim", "primitive/sphere.prim", "primitive/cone.prim"};

CommonPrimitives PrimitiveModel::GetPrimitiveType(const char *fileName)
{
    for (size_t i = 0; i < (int)CommonPrimitives::Total; i++)
    {
        if (!strcasecmp(fileName, g_PrimitiveFiles[i]))
            return (CommonPrimitives)i;
    }

    return CommonPrimitives::Total;
}

PrimitiveModel::PrimitiveModel(const char *fileName) : IModel(fileName)
{
    m_Type = GetPrimitiveType(fileName);

    switch (m_Type)
    {
    case CommonPrimitives::Box:
        m_pDrawMesh = DrawUtils::MakeCube(1);
        break;
    case CommonPrimitives::Sphere:
        m_pDrawMesh = DrawUtils::MakeIcosphere(1);
        break;
    case CommonPrimitives::Cone:
        m_pDrawMesh = DrawUtils::MakeWireframeCone();
        break;
    default:
        break;
    }
}

PrimitiveModel::~PrimitiveModel()
{
    if (m_pDrawMesh)
        delete m_pDrawMesh;
}

const std::optional<BoundingBox> PrimitiveModel::GetBoundingBox() const
{
    return BoundingBox(1);
}

void PrimitiveModel::Render(SceneEntity *pEntity, SceneRenderer *sr, RenderMode mode)
{
    auto &    bbox   = pEntity->GetRelativeBoundingBox();
    glm::vec3 scale  = bbox.Size();
    auto      shader = GLBackend::Instance()->SolidColorGeometryShader();

    shader->Bind();

    for (auto &it : shader->Uniforms())
    {
        switch (it->Kind())
        {
        case UniformKind::Color:
            it->SetFloat4(pEntity->GetRenderColor());
            break;
        case UniformKind::TransformMatrix: {
            glm::vec3   offset = bbox.Mins() + scale * 0.5f;
            glm::mat4x4 mat    = glm::translate(glm::mat4x4(1.f), pEntity->GetPosition() + offset);
            it->SetMat4(mat);
        }
        break;
        case UniformKind::Scale:
            it->SetFloat3(scale.xyz);
            break;
        case UniformKind::ObjectSerialNumber:
            it->SetInt(pEntity->GetSerialNumber());
            break;
        default:
            GLBackend::SetUniformValue(it);
            break;
        }
    }

    m_pDrawMesh->BindAndDraw();

    shader->Unbind();
}

IModelWeakPtr PrimitiveModel::LookupByType(CommonPrimitives type)
{
    return ModelsManager::Instance()->LookupModel(g_PrimitiveFiles[(int)type], false);
}
