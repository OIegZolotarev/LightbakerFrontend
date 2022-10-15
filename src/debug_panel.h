/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once
#include "igui_panel.h"

class DebugPanel: public IGUIPanel
{
public:
	DebugPanel();

	DockPanels GetDockSide() override;
	void Render() override;
};

