/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once

#include "commands_toolbar_panel.h"

class ToolboxPanel : public CommandsToolbar
{
    void RenderItems(ImGuiAxis toolbar_axis) override;

public:
    ToolboxPanel(ToolUIPanelID id, const char *name);
};
