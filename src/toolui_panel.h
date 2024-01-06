/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

#include "ui_common.h"
#include <better-enums/enum.h>

// Serializable panel ID, used to restore position when application is loaded
BETTER_ENUM(ToolUIPanelID,int, None,ObjectProperties, ConsoleOutput, SceneObjects, Debug, MaxPanels)

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

class ToolUIPanel
{
protected:
	char* m_strPanelTitle = nullptr;
    ToolUIPanelID m_id            = ToolUIPanelID::None;

	IPlatformWindow *m_pOwningWindow;
private:
	
	bool m_bDefaultPositionChecked = false;
	void ResetPosition();

public:
	//	ImGui::DockBuilderDockWindow(strTabModelDisplay, Application::GetDockId(ImGuiDir_Down));

	ToolUIPanel(ToolUIPanelID id, char *title);

	void InvokeRender();
	virtual void Render();

	virtual DockPanels GetDockSide() = 0;
	void InvalidatePosition();
};

