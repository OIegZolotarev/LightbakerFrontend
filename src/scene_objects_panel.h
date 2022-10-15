/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

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

