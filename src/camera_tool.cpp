/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "camera_tool.h"

CameraTool::CameraTool() : IEditingTool(EditingToolId::Camera)
{
    SetBoundCommand(GlobalCommands::ActivateCameraTool);
    SetToolIcon(CommonIcons::CameraTool);
    SetDescritpion("Camera tool");
}

void CameraTool::OnMouseClicked(Viewport *pActiveViewport, Scene *pActiveDocument)
{
    throw std::logic_error("The method or operation is not implemented.");
}

void CameraTool::OnMouseReleased()
{
    throw std::logic_error("The method or operation is not implemented.");
}

void CameraTool::Render(float flFrameDelta)
{
    throw std::logic_error("The method or operation is not implemented.");
}

void CameraTool::RenderUI()
{
    throw std::logic_error("The method or operation is not implemented.");
}
