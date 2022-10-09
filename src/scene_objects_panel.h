#pragma once
#include "igui_panel.h"

class SceneObjectPanel : public IGUIPanel
{
	void RenderSceneObjectsPanel();
public:

	SceneObjectPanel();

	DockPanels GetDockSide() override;
	void Render() override;
};

