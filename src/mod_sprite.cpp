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
}

SpriteModel::~SpriteModel()
{
}

void SpriteModel::Render(SceneEntity *pEntity, RenderMode mode)
{
}

void SpriteModel::OnSceneLoaded(Scene *pScene)
{
}

const std::optional<BoundingBox> SpriteModel::GetBoundingBox() const
{
    return BoundingBox(1);
}
