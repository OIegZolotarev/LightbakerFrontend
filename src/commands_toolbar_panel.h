/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/


#pragma once
#include "toolui_panel.h"
#include "commands_registry.h"


class CommandsToolbar : public ToolUIPanel
{
    // nulls treated as separators
    std::list<CCommand *> m_lstItems;
    DockPanels            m_DefaultDockSide;

    int  m_ToolbarAxis;
    void DockingToolbar(const char *name, int * axis);


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
