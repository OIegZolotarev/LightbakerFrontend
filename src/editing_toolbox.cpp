/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "camera_tool.h"
#include "editing_toolbox.h"
#include "selection_tool.h"

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
}

void EditingToolbox::SelectTool(EditingToolId toolId)
{
    IEditingTool *pTool = FindTool(toolId);
    m_pCurrentTool      = pTool;
}

void EditingToolbox::RenderTool(float flFrameDelta)
{
    if (!m_pCurrentTool)
        return;

    m_pCurrentTool->Render(flFrameDelta);
}

void EditingToolbox::RenderToolUI()
{
    if (!m_pCurrentTool)
        return;

    m_pCurrentTool->RenderUI();
}

int EditingToolbox::HandleEvent(bool bWasHandled, const SDL_Event &e, const float flFrameDelta)
{
    return 0;
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
