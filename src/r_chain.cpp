/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "r_chain.h"

RenderChain::RenderChain()
{
    m_EyePos = {0, 0, 0};

    m_ChainStart      = SceneEntityWeakPtr();
    m_ChainStart      = SceneEntityWeakPtr();
    m_flClosestEntity = m_flFarthestEntity = 0;
}

RenderChain::~RenderChain()
{
}

void RenderChain::Reset(glm::vec3 eyePos)
{
    m_EyePos = eyePos;

    m_ChainStart      = SceneEntityWeakPtr();
    m_ChainStart      = SceneEntityWeakPtr();
    m_flClosestEntity = m_flFarthestEntity = 0;
}

void RenderChain::AddDistanceSorted(SceneEntityWeakPtr &pEntity)
{
    BT_PROFILE("RenderChain::AddDistanceSorted");

    auto ptr = pEntity.lock();

    if (!ptr)
        return;

    if (!ptr->Next().expired())
    {
        SceneEntityWeakPtr w;
        ptr->SetNext(w);
    }

    if (m_ChainStart.expired())
    {
        m_ChainStart       = pEntity;
        m_ChainEnd         = pEntity;
        m_flClosestEntity  = glm::distance2(m_EyePos, ptr->GetPosition());
        m_flFarthestEntity = m_flClosestEntity;

        return;
    }

    float flDist = glm::distance2(m_EyePos, ptr->GetPosition());

    if (flDist < m_flClosestEntity)
    {
        auto chainEnd = m_ChainEnd.lock();

        // Bail out, chain will rebuild next frame
        if (!chainEnd)
            return;

        chainEnd->SetNext(pEntity);

        m_ChainEnd        = pEntity.lock();
        m_flClosestEntity = flDist;
    }
    else
    {
        auto chainStart = m_ChainStart.lock();

        // Bail out, chain will rebuild next frame
        if (!chainStart)
            return;

        auto oldSn = chainStart->GetSerialNumber();
        auto newSn = ptr->GetSerialNumber();

        ptr->SetNext(m_ChainStart);
        m_ChainStart = pEntity;
    }
}
