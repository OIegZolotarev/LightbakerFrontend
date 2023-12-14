/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

#include "ui_common.h"
#include <better-enums/enum.h>

BETTER_ENUM(PanelsId,int, None,ObjectProperties, ConsoleOutput, SceneObjects, Debug, MaxPanels)

enum class DockPanels
{
	LeftUp,
	LeftBottom,
	RightTop,
	RightBottom,
	BottomLeft,
	BottomRight,
	TopLeft,
	TopRight,
};

class IPlatformWindow;

class IGUIPanel
{
protected:
	char* m_strPanelTitle = nullptr;
	PanelsId m_id = PanelsId::None;

	IPlatformWindow *m_pOwningWindow;

private:
	bool m_bDefaultPositionChecked = false;
	void ResetPosition();
public:
	//	ImGui::DockBuilderDockWindow(strTabModelDisplay, Application::GetDockId(ImGuiDir_Down));

	IGUIPanel(PanelsId id,char * title);

	void InvokeRender();
	virtual void Render();

	virtual DockPanels GetDockSide() = 0;
	void InvalidatePosition();
};

