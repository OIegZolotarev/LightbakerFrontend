/*
        LightBaker3000 Frontend project,
        (c) 2023 CrazyRussian
*/


#include "application.h"
#include "common.h"
#include "viewports_orchestrator.h" 

ViewportsOrchestrator::~ViewportsOrchestrator()
{
    for (auto &it : m_lstViewports)
        delete it;
}

void ViewportsOrchestrator::Init(nlohmann::json && data)
{
    m_savedViewports = data;
}

Viewport *ViewportsOrchestrator::AddNewViewport(const char *name, IPlatformWindow *pHostWindow, Viewport *pCloneFrom,
                                                bool initialyVisible /*= true*/)
{
    Viewport *pResult = new Viewport(name, pHostWindow, pCloneFrom);

    m_lstViewports.push_back(pResult);
    pHostWindow->AddEventHandler(pResult);
    return pResult;
}

void ViewportsOrchestrator::RenderViewports(IPlatformWindow *pWindow, float flFrameDelta)
{
    for (auto &it : m_lstViewports)
    {
        if (!it->IsVisible())
            continue;

        if (it->GetPlatformWindow() != pWindow)
            continue;

        

        it->RenderFrame(flFrameDelta);
    }
}

void ViewportsOrchestrator::DisplayViewports(IPlatformWindow *pWindow)
{
    for (auto & it: m_lstViewports)
    {
        if (it->GetPlatformWindow() != pWindow)
            continue;

        it->DisplayRenderedFrame();
    }
}

void ViewportsOrchestrator::SaveViewports(nlohmann::json &persistentData)
{
    nlohmann::json block;

    for (auto &it : m_lstViewports)
    {
        nlohmann::json descr;
        it->SaveState(descr);
        block.push_back(descr);
    }

    persistentData["Viewports"] = block;
}

bool ViewportsOrchestrator::LoadViewports()
{
    for (auto &it : m_savedViewports)
    {
        Viewport *pViewport = Viewport::LoadState(it);

        if (pViewport)
        {
            pViewport->RegisterEventHandlerAtHost();
            m_lstViewports.push_back(pViewport);
        }
    }

    return true;
}
