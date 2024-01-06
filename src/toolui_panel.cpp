/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/

#include "toolui_panel.h"
#include "application.h"
#include "ui_common.h"
#include <imgui/imgui_internal.h>

void ToolUIPanel::ResetPosition()
{
    ImGuiID dockId = 0;

    auto dockSites = Application::GetMainWindow()->GetDockSides();

    switch (GetDockSide())
    {
    case DockPanels::LeftUp:
        dockId = dockSites->idDockLeftTop;
        break;
    case DockPanels::LeftBottom:
        dockId = dockSites->idDockLeftBottom;
        break;
    case DockPanels::RightTop:
        dockId = dockSites->idDockRightTop;
        break;
    case DockPanels::RightBottom:
        dockId = dockSites->idDockRightBottom;
        break;
    case DockPanels::BottomLeft:
        dockId = dockSites->idDockBottomLeft;
        break;
    case DockPanels::BottomRight:
        dockId = dockSites->idDockBottomRight;
        break;
    case DockPanels::TopLeft:
        dockId = dockSites->idDockUpLeft;
        break;
    case DockPanels::TopRight:
        dockId = dockSites->idDockUpRight;
        break;
    case DockPanels::Top:
        dockId = dockSites->idDockTop;
        break;
    case DockPanels::Bottom:
        dockId = dockSites->idDockBottom;
        break;
    case DockPanels::Left:
        dockId = dockSites->idDockLeft;
        break;
    case DockPanels::Right:
        dockId = dockSites->idDockRight;
        break;
    default:
        break;
    }

    ImGui::DockBuilderDockWindow(m_strPanelTitle, dockId);

    //Application::GetMainWindow()->UpdateDocks();
}

ToolUIPanel::ToolUIPanel(ToolUIPanelID id, const char *title)
{
    m_id            = id;
    m_strPanelTitle = title;
}

void ToolUIPanel::InvokeRender()
{
    if (!m_bDefaultPositionChecked)
    {
        if (!Application::DelayedInitDone())
            return;

        auto pers = Application::GetPersistentStorage();

        if (!pers->IsPanelAtValidPosition(m_id))
        {
            ResetPosition();
            pers->FlagPanelIsAtValidPosition(m_id);
        }

        m_bDefaultPositionChecked = true;
    }

    Render();
}

void ToolUIPanel::Render()
{
}

void ToolUIPanel::InvalidatePosition()
{
    ResetPosition();
}
