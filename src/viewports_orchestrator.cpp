/*
        LightBaker3000 Frontend project,
        (c) 2023 CrazyRussian
*/

#include "application.h"
#include "viewports_orchestrator.h"
#include "common.h"

ViewportsOrchestrator::~ViewportsOrchestrator()
{
    for (auto &it : m_lstViewports)
        delete it;
}

void ViewportsOrchestrator::Init(nlohmann::json &&data)
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

void ViewportsOrchestrator::FlagRepaintAll()
{
    for (auto & it:m_lstViewports)
    {
        it->FlagUpdate();
    }
}

void ViewportsOrchestrator::RenderViewports(IPlatformWindow *pWindow, float flFrameDelta)
{
    BT_PROFILE("ViewportsOrchestrator::RenderViewports()");

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
    BT_PROFILE("ViewportsOrchestrator::DisplayViewports()");

    for (auto &it : m_lstViewports)
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

void ViewportsOrchestrator::DestroyWindowViewports(IPlatformWindow *wind)
{
    m_lstViewports.remove_if([&](Viewport *pViewport) {
        if (pViewport->GetPlatformWindow() == wind)
        {
            delete pViewport;
            return true;
        }
        return false;
    });
}
