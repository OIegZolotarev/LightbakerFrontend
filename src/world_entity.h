/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "scene_entity.h"
#include "lb3k_wrapper.h"

class Scene;

class IWorldEntity: public SceneEntity
{

public:
    IWorldEntity(Scene *pScene) : SceneEntity(pScene)
    {
    }

    virtual void ReloadLightmaps() = 0;
    virtual std::string ExportForCompiling(const char *newPath, lightBakerSettings_t *lb3kOptions) = 0;
};
