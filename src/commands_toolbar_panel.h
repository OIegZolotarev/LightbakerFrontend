/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/


#pragma once
#include "toolui_panel.h"
#include "commands_registry.h"
#include "imgui_internal.h"


class CommandsToolbar : public ToolUIPanel
{
    DockPanels            m_DefaultDockSide;

    int  m_ToolbarAxis;
    void DockingToolbar(const char *name, int * axis);

    virtual void RenderItems(ImGuiAxis toolbar_axis);

protected:
    // nulls treated as separators
    std::list<CCommand *> m_lstItems;

public:
    CommandsToolbar(ToolUIPanelID id, const char *name);
    ~CommandsToolbar();

    void AddCommand(CCommand *cmd);
    void AddCommand(GlobalCommands id);
    void AddSeparator();

    void       SetDefaultDockSide(DockPanels id);
    DockPanels GetDockSide() override;

    void Render() override;
};
