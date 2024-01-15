/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "goldsource_bsp_renderer.h"
#include "mod_manager.h"

class BSPModelAdapter : public IModel
{
    GoldSource::BSPModelRenderCookieWeakPtr m_Model;
    

    int m_ModelIndex;
public:
    BSPModelAdapter(const char *modelName);
    ~BSPModelAdapter();

    void Render(SceneEntity *pEntity, const SceneRenderer * sr, RenderMode mode, ShaderProgram* currentShader) override;
    void OnSceneLoaded(Scene *pScene) override;

    const std::optional<BoundingBox> GetBoundingBox() const override;
};
