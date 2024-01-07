/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "camera_tool.h"

CameraTool::CameraTool() : IEditingTool(EditingToolId::Camera)
{
    SetBoundCommand(GlobalCommands::ActivateCameraTool);
    SetToolIcon(CommonIcons::CameraTool);
    SetDescritpion("Camera tool");
}

void CameraTool::Render(float flFrameDelta)
{
    throw std::logic_error("The method or operation is not implemented.");
}

void CameraTool::RenderUI()
{
    throw std::logic_error("The method or operation is not implemented.");
}
