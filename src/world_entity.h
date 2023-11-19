/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "scene_entity.h"

class IWorldEntity: public SceneEntity
{

public:
    virtual void ReloadLightmaps() = 0;
};
