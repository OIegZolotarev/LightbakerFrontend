#pragma once
#include "igui_panel.h"

class DebugPanel: public IGUIPanel
{
public:
	DebugPanel();

	DockPanels GetDockSide() override;
	void Render() override;
};

