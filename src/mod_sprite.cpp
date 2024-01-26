/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "application.h"
#include "common.h"

#include "mod_manager.h"
#include "mod_sprite.h"



 SpriteModel::SpriteModel(const char *modelName) : IModel(modelName)
{
     SetTransparent(true);
     SetType(ModelType::Sprite);
     m_pTexture = TextureManager::LoadTextureAsynch(modelName);
 }

SpriteModel::~SpriteModel()
{
}

void SpriteModel::Render(SceneEntity *pEntity, const SceneRenderer * sr, RenderMode mode, ShaderProgram* currentShader)
{
    BT_PROFILE("SpriteModel::Render");

    const BoundingBox &relativeBbox = pEntity->GetRelativeBoundingBox();

    const glm::vec3 size = relativeBbox.Size();
    
    // glm::mat4 matTransform = glm::translate(glm::mat4(1), pEntity->GetPosition());

    for (auto &it : currentShader->Uniforms())
    {
        switch (it->Kind())
        {
        case UniformKind::Color:
            it->SetFloat4(pEntity->GetRenderColor());
            break;
        case UniformKind::TransformMatrix:
            it->SetMat4(pEntity->GetTransform());
            //it->SetMat4(matTransform);
            break;
        case UniformKind::Scale:
            it->SetFloat3({size.xyy});
            break;
        case UniformKind::ObjectSerialNumber:
            it->SetInt(pEntity->GetSerialNumber() + 1);
            break;
        case UniformKind::Color2:
            sr->ApplySelectedObjectColor(pEntity, it);            
            break;
        }
    }

    GLBackend::BindTexture(0, m_pTexture);    
    
    sr->DrawBillboardMesh();
}

void SpriteModel::OnSceneLoaded(Scene *pScene)
{
}

const std::optional<BoundingBox> SpriteModel::GetBoundingBox() const
{
    return BoundingBox(1);
}
