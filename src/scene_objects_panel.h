/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once
#include "toolui_panel.h"

class SceneObjectPanel : public ToolUIPanel
{
	void RenderSceneObjectsPanel();
public:

	SceneObjectPanel();

	DockPanels GetDockSide() override;
	void Render() override;
};

