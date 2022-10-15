/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "ui_common.h"
#include "igui_panel.h"
#include "console_output_panel.h"

ConsoleOutputPanel::ConsoleOutputPanel(DebugConsole* pConsole): IGUIPanel(PanelsId::ConsoleOutput,(char*)"Console")
{
	m_pConsole = pConsole;
}

DockPanels ConsoleOutputPanel::GetDockSide()
{
	return DockPanels::BottomLeft;
}

void ConsoleOutputPanel::Render()
{
	if (m_pConsole) m_pConsole->Draw(m_strPanelTitle);
}
