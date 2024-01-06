/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

#include "ui_common.h"
#include <better-enums/enum.h>

// Serializable panel ID, used to restore position when application is loaded
BETTER_ENUM(ToolUIPanelID,int, None,ObjectProperties, ConsoleOutput, SceneObjects, Debug, MainToolbar, Toolbar2, MaxPanels)

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
	Top,
	Bottom,
	Left,
	Right,
};

class IPlatformWindow;

class ToolUIPanel
{
protected:
	const char* m_strPanelTitle;
    ToolUIPanelID m_id            = ToolUIPanelID::None;

	IPlatformWindow *m_pOwningWindow;
private:
	
	bool m_bDefaultPositionChecked = false;
	void ResetPosition();

public:
	//	ImGui::DockBuilderDockWindow(strTabModelDisplay, Application::GetDockId(ImGuiDir_Down));

	ToolUIPanel(ToolUIPanelID id, const char *title);

	void InvokeRender();
	virtual void Render();

	virtual DockPanels GetDockSide() = 0;
	void InvalidatePosition();
};

