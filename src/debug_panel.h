/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once
#include "toolui_panel.h"

class DebugPanel: public ToolUIPanel
{
public:
	DebugPanel();

	DockPanels GetDockSide() override;
	void Render() override;
};

