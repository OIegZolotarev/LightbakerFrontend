#pragma once
#include "igui_panel.h"
#include "console.h"

class ConsoleOutputPanel: public IGUIPanel
{
	DebugConsole* m_pConsole = 0;
public:
	ConsoleOutputPanel(DebugConsole * pConsole);

	DockPanels GetDockSide() override;
	void Render() override;
};

