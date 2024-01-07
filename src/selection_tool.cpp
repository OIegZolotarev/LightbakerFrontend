/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "selection_tool.h"

SelectionTool::SelectionTool() : IEditingTool(EditingToolId::Selection)
{
    SetBoundCommand(GlobalCommands::ActivateSelectionTool);
    SetToolIcon(CommonIcons::SelectTool);
    SetDescritpion("Selection tool");
}

void SelectionTool::OnMouseClicked(Viewport *pActiveViewport, Scene *pActiveDocument)
{
    throw std::logic_error("The method or operation is not implemented.");
}

void SelectionTool::OnMouseReleased()
{
    throw std::logic_error("The method or operation is not implemented.");
}

void SelectionTool::Render(float flFrameDelta)
{
    throw std::logic_error("The method or operation is not implemented.");
}

void SelectionTool::RenderUI()
{
    throw std::logic_error("The method or operation is not implemented.");
}
