/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once
#include "toolui_panel.h"
#include "console.h"

class ConsoleOutputPanel: public ToolUIPanel
{
	DebugConsole* m_pConsole = 0;
public:
	ConsoleOutputPanel(DebugConsole * pConsole);

	DockPanels GetDockSide() override;
	void Render() override;
};

