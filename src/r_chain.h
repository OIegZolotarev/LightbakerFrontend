/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "scene_entity.h"

class RenderChain
{
    SceneEntity* m_pChainStart;
    SceneEntity *m_pChainEnd;

public:
    RenderChain();
    ~RenderChain();

    void Reset(glm::vec3 eyePos);

    void AddDistanceSorted(SceneEntity *pEntity);

};


