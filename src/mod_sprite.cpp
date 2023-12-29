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
     SetType(ModelType::Sprite);
     m_pTexture = TextureManager::LoadTextureAsynch(modelName);
 }

SpriteModel::~SpriteModel()
{
}

void SpriteModel::Render(SceneEntity *pEntity, SceneRenderer *sr, RenderMode mode)
{
    const BoundingBox &relativeBbox = pEntity->GetRelativeBoundingBox();

    const glm::vec3 size = relativeBbox.Size();
    sr->DrawBillboard(pEntity->GetPosition(), size.xy, m_pTexture, pEntity->GetRenderColor(),
                      pEntity->GetSerialNumber());
}

void SpriteModel::OnSceneLoaded(Scene *pScene)
{
}

const std::optional<BoundingBox> SpriteModel::GetBoundingBox() const
{
    return BoundingBox(1);
}
