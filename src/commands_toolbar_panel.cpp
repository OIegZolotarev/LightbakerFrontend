/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "commands_toolbar_panel.h"
#include "application.h"
#include "common.h"
#include "imgui_helpers.h"
#include "imgui_internal.h"

// Based on https://github.com/ocornut/imgui/issues/2648
// Toolbar test [Experimental]
// Usage:
// {
//   static ImGuiAxis toolbar1_axis = ImGuiAxis_X; // Your storage for the current direction.
//   DockingToolbar("Toolbar1", &toolbar1_axis);
// }

ImVec2 g_Adj = {2, 2};

void CommandsToolbar::DockingToolbar(const char *name, int *axis)
{
    // [Option] Automatically update axis based on parent split (inside of doing it via right-click on the toolbar)
    // Pros:
    // - Less user intervention.
    // - Avoid for need for saving the toolbar direction, since it's automatic.
    // Cons:
    // - This is currently leading to some glitches.
    // - Some docking setup won't return the axis the user would expect.
    const bool TOOLBAR_AUTO_DIRECTION_WHEN_DOCKED = true;

    // ImGuiAxis_X = horizontal toolbar
    // ImGuiAxis_Y = vertical toolbar
    ImGuiAxis toolbar_axis = *(ImGuiAxis *)axis;

    // 1. We request auto-sizing on one axis
    // Note however this will only affect the toolbar when NOT docked.
    ImVec2 requested_size = (toolbar_axis == ImGuiAxis_X) ? ImVec2(-1.0f, 0.0f) : ImVec2(0.0f, -1.0f);
    ImGui::SetNextWindowSize(requested_size);

    // 2. Specific docking options for toolbars.
    // Currently they add some constraint we ideally wouldn't want, but this is simplifying our first implementation
    ImGuiWindowClass window_class;
    window_class.DockingAllowUnclassed = true;
    window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoCloseButton;
    window_class.DockNodeFlagsOverrideSet |=
        ImGuiDockNodeFlags_HiddenTabBar; // ImGuiDockNodeFlags_NoTabBar // FIXME: Will need a working Undock widget
                                         // for _NoTabBar to work
    // window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingSplitOther;
    window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverMe;
    window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverOther;
    if (toolbar_axis == ImGuiAxis_X)
        window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoResizeY;
    else
        window_class.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoResizeX;
    
    ImGui::SetNextWindowClass(&window_class);

    // 3. Begin into the window

    int flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar;
    
    const float  font_size = ImGui::GetFontSize();
    const ImVec2 icon_size(ImFloor(font_size * g_Adj.x), ImFloor(font_size * g_Adj.y));
    ImGui::Begin(name, NULL, flags);

    // 4. Overwrite node size
    ImGuiDockNode *node = ImGui::GetWindowDockNode();
    if (node != NULL)
    {
        // Overwrite size of the node
        ImGuiStyle &    style             = ImGui::GetStyle();
        const ImGuiAxis toolbar_axis_perp = (ImGuiAxis)(toolbar_axis ^ 1);
        const float     TOOLBAR_SIZE_WHEN_DOCKED =
            style.WindowPadding[toolbar_axis_perp] * 2.0f + icon_size[toolbar_axis_perp];
        node->WantLockSizeOnce        = true;
        
        node->Size[toolbar_axis_perp] = node->SizeRef[toolbar_axis_perp] = TOOLBAR_SIZE_WHEN_DOCKED;
        
        // FIXME: for now code assumes that there is no windows in upper heirarchy         
        // BUT, in configurations like those will fail
        // 
        // 
//         node->ParentNode->WantLockSizeOnce        = true;
//         node->ParentNode->Size[toolbar_axis_perp] = node->SizeRef[toolbar_axis_perp] = TOOLBAR_SIZE_WHEN_DOCKED;

        if (TOOLBAR_AUTO_DIRECTION_WHEN_DOCKED)
        {
            if (node->ParentNode && node->ParentNode->SplitAxis != ImGuiAxis_None)
                toolbar_axis = (ImGuiAxis)(node->ParentNode->SplitAxis ^ 1);

//             if (node->ParentNode)
//             {
//                 if (node->ParentNode->Size.x > node->ParentNode->Size.y)
//                     toolbar_axis = ImGuiAxis::ImGuiAxis_X;
//                 else
//                     toolbar_axis = ImGuiAxis::ImGuiAxis_Y;
//             }
        }
    }

    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 5.0f));

    float size = 21;
    int   n    = 0;
    for (auto it : m_lstItems)
    {
        if (n > 0 && toolbar_axis == ImGuiAxis_X)
            ImGui::SameLine();

        if (it == nullptr)
        {
            if (toolbar_axis == ImGuiAxis_X)
                ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
            else
                ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);
        }
        else
        {
            CCommand *cmd = it;

            if (ImGuiHelpers::ButtonWithCommonIcon(cmd->GetCommonIcon(), cmd->GetDescription(), size))
            {
                cmd->Execute();
            }
        }

        n++;
    }
    ImGui::PopStyleVar(2);

    // 6. Context-menu to change axis
    if (node == NULL || !TOOLBAR_AUTO_DIRECTION_WHEN_DOCKED)
    {
        if (ImGui::BeginPopupContextWindow())
        {
            ImGui::TextUnformatted(name);
            ImGui::Separator();
            if (ImGui::MenuItem("Horizontal", "", (toolbar_axis == ImGuiAxis_X)))
                toolbar_axis = ImGuiAxis_X;
            if (ImGui::MenuItem("Vertical", "", (toolbar_axis == ImGuiAxis_Y)))
                toolbar_axis = ImGuiAxis_Y;
            ImGui::EndPopup();
        }
    }

    ImGui::End();

    // Output user stored data
    *axis = (int)toolbar_axis;
}

CommandsToolbar::CommandsToolbar(ToolUIPanelID id, const char *name) : ToolUIPanel(id, name)
{
    m_ToolbarAxis = ImGuiAxis::ImGuiAxis_X;
}

CommandsToolbar::~CommandsToolbar()
{
    m_lstItems.clear();
}

void CommandsToolbar::AddCommand(CCommand *cmd)
{
    m_lstItems.push_back(cmd);
}

void CommandsToolbar::AddCommand(GlobalCommands id)
{
    auto cmd = Application::CommandsRegistry()->FindCommandByGlobalId(id);

    if (cmd)
    {
        AddCommand(cmd);
    }
}

void CommandsToolbar::AddSeparator()
{
    m_lstItems.push_back(nullptr);
}

void CommandsToolbar::SetDefaultDockSide(DockPanels id)
{
    m_DefaultDockSide = id;
}

DockPanels CommandsToolbar::GetDockSide()
{
    return m_DefaultDockSide;
}

void CommandsToolbar::Render()
{
    DockingToolbar(m_strPanelTitle, &m_ToolbarAxis);
}
