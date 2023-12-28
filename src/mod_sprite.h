/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "mod_manager.h"

class GLTexture;

class SpriteModel : public IModel
{
    GLTexture *m_pTexture;

public:
    SpriteModel(const char *modelName);
    ~SpriteModel();

    void Render(SceneEntity *pEntity, RenderMode mode) override;
    void OnSceneLoaded(Scene *pScene) override;

    const std::optional<BoundingBox> GetBoundingBox() const override;
};
