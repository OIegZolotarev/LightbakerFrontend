/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "scene_entity.h"

class RenderChain
{
    SceneEntityWeakPtr m_ChainStart;
    SceneEntityWeakPtr m_ChainEnd;

    float m_flClosestEntity  = 0;
    float m_flFarthestEntity = 0;

    glm::vec3 m_EyePos;

public:
    RenderChain();
    ~RenderChain();

    void Reset(glm::vec3 eyePos);

    void AddDistanceSorted(SceneEntityWeakPtr &pEntity);

    SceneEntityWeakPtr &Start()
    {
        return m_ChainStart;
    }
};


