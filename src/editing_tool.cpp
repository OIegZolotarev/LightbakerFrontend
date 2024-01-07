/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"
#include "editing_tool.h"
#include "editing_toolbox.h"
#include "text_utils.h"

IEditingTool::IEditingTool(EditingToolId id)
{
    m_id = id;
}

IEditingTool::~IEditingTool()
{
}

int IEditingTool::HandleMouseEvent(bool bWasHandled, const SDL_Event &e, const float flFrameDelta)
{
    return 0;
}

int IEditingTool::HandleKeyboardEvent(bool bWasHandled, const SDL_Event &e, const float flFrameDelta)
{
    return 0;
}

void IEditingTool::Render(float flFrameDelta)
{
}

void IEditingTool::RenderUI()
{
}

const EditingToolId IEditingTool::GetToolId() const
{
    return m_id;
}

const GlobalCommands IEditingTool::GetBoundCommand() const
{
    return m_BoundCommand;
}

void IEditingTool::SetActiveViewport(Viewport *pViewport)
{
    m_pActiveViewport = pViewport;
}

void IEditingTool::SetActiveDocument(Scene *pScene)
{
    m_pActiveDocument = pScene;
}

void IEditingTool::SetBoundCommand(GlobalCommands cmd)
{
    m_BoundCommand = cmd;
}

void IEditingTool::SetToolIcon(CommonIcons icon)
{
    m_ToolIcon = icon;
}

void IEditingTool::SetDescritpion(const char *strDescription)
{
    strlcpy(m_strDescription, strDescription, sizeof(m_strDescription));
}

const char *IEditingTool::GetDescrition() const
{
    return m_strDescription;
}

void IEditingTool::RegisterBoundCommand()
{
    CCommand *newCommand = new CCommand;
    newCommand->SetId(m_BoundCommand);
    newCommand->SetDescription(m_strDescription);
    newCommand->SetKeyStroke(nullptr);
    newCommand->SetCommonIcon(m_ToolIcon);
    newCommand->SetFlags(0);
    newCommand->SetCallback([&]() { EditingToolbox::Instance()->SelectTool(m_id); });

    Application::CommandsRegistry()->RegisterCommand(newCommand);
}
