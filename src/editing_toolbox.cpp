/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "camera_tool.h"
#include "editing_toolbox.h"
#include "selection_tool.h"
#include "viewports_orchestrator.h"

IEditingTool *EditingToolbox::FindTool(EditingToolId id)
{
    for (auto &it : m_lstToolbs)
    {
        if (it->GetToolId() == id)
            return it;
    }

    assert(false && "Unknown tool id!");
    return nullptr;
}

void EditingToolbox::Initialize()
{
    m_lstToolbs.push_back(new SelectionTool());
    m_lstToolbs.push_back(new CameraTool());

    for (auto & it: m_lstToolbs)
    {
        it->RegisterBoundCommand();
    }

    SelectTool(EditingToolId::Selection);
}

void EditingToolbox::SelectTool(EditingToolId toolId)
{
    IEditingTool *pTool = FindTool(toolId);
    m_pCurrentTool      = pTool;
}

void EditingToolbox::RenderToolViewportUI(Viewport * pViewport)
{
    if (!m_pCurrentTool)
        return;

    // InitializeToolContext();
    m_pCurrentTool->RenderViewportUI(pViewport);
}

void EditingToolbox::RenderToolViewport2DGraphics(float flFrameDelta)
{
    if (!m_pCurrentTool)
        return;

    // InitializeToolContext();
    m_pCurrentTool->RenderViewport2DGraphics(flFrameDelta);
}

void EditingToolbox::RenderTool(float flFrameDelta)
{
    if (!m_pCurrentTool)
        return;

    // InitializeToolContext();
    m_pCurrentTool->Render(flFrameDelta);
}

void EditingToolbox::RenderToolUI()
{
    if (!m_pCurrentTool)
        return;

    // InitializeToolContext();
    m_pCurrentTool->RenderUI();
}

int EditingToolbox::HandleEvent(bool bWasHandled, const SDL_Event &e, const float flFrameDelta)
{
    if (!m_pCurrentTool)
        return 0;

    InitializeToolContext();

    switch (e.type)
    {
    case SDL_MOUSEBUTTONDOWN:        
    case SDL_MOUSEBUTTONUP:
        return m_pCurrentTool->HandleMouseEvent(bWasHandled, e, flFrameDelta);        
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        return m_pCurrentTool->HandleKeyboardEvent(bWasHandled, e, flFrameDelta);
    }

    return 0;
}

void EditingToolbox::InitializeToolContext()
{
    auto viewport = ViewportsOrchestrator::Instance()->GetHoveredViewport();
    auto scene    = Scene::ActiveInstance();

    m_pCurrentTool->SetActiveViewport(viewport);
    m_pCurrentTool->SetActiveDocument(scene);
}

const std::list<IEditingTool *> &EditingToolbox::GetAllTools() const
{
    return m_lstToolbs;
}

const EditingToolId EditingToolbox::SelectedToolId() const
{
    if (m_pCurrentTool)
        return m_pCurrentTool->GetToolId();

    return EditingToolId::None;
}

