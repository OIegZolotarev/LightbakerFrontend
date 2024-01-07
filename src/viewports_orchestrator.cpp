/*
        LightBaker3000 Frontend project,
        (c) 2023 CrazyRussian
*/

#include "application.h"
#include "common.h"
#include "viewports_orchestrator.h"
#include "imgui_internal.h"

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
    for (auto &it : m_lstViewports)
    {
        it->FlagUpdate();
    }
}

void ViewportsOrchestrator::RenderViewports(IPlatformWindow *pWindow, float flFrameDelta)
{
    BT_PROFILE("ViewportsOrchestrator::RenderViewports()");

    m_pHoveredViewport = nullptr;

    for (auto &it : m_lstViewports)
    {
        if (!it->IsVisible())
            continue;

        if (it->GetPlatformWindow() != pWindow)
            continue;

        if (it->GetMouseHoveringStatus() == ViewportMouseHover::Hovered)
        {
            assert(!m_pHoveredViewport && "Error in hovering logic - multiple viewports seems to be hovered");
            m_pHoveredViewport = it;
        }

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

void ViewportsOrchestrator::CloneViewportToLeastClutteredWindow(Viewport *param1)
{
    int              bestCount  = 999999;
    IPlatformWindow *bestWindow = nullptr;

    for (auto &it : Application::Instance()->GetAllWindows())
    {
        int viewportsCount = CountViewports(it);

        if (viewportsCount < bestCount)
        {
            bestCount  = viewportsCount;
            bestWindow = it;
        }
    }

    assert(bestWindow);

    static int counter = 1;

    std::string name = std::format("Viewport {0}", counter++);

    AddNewViewport(name.c_str(), bestWindow, param1);
}

void ViewportsOrchestrator::EnsureAtLeastOneViewportExists()
{
    bool hasVisible = false;

    for (auto &vp : m_lstViewports)
    {
        if (vp->IsVisible())
        {
            hasVisible = true;
            break;
        }
    }

    if (hasVisible)
        return;

    auto dockSides = Application::GetMainWindow()->GetDockSides();

    Viewport *pTarget = nullptr;

    if (m_lstViewports.size() > 0)
    {
        auto it       = m_lstViewports.begin();
        auto viewport = *it;

        pTarget = viewport;
    }
    else
        pTarget = AddNewViewport("Main", Application::GetMainWindow(), 0);

    pTarget->SetVisible(true);
    ImGui::DockBuilderDockWindow(pTarget->Name(), dockSides->idDockCenter);
    Application::GetMainWindow()->UpdateDocks();
}

Viewport *ViewportsOrchestrator::GetHoveredViewport()
{
    return m_pHoveredViewport;
}

int ViewportsOrchestrator::CountViewports(const IPlatformWindow *it)
{
    int result = 0;

    for (auto &vp : m_lstViewports)
    {
        if (vp->GetPlatformWindow() == it)
            result++;
    }

    return result;
}
