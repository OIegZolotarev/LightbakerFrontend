/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "editing_toolbox.h"
#include "imgui_helpers.h"
#include "toolbox_panel.h"

void ToolboxPanel::RenderItems(ImGuiAxis toolbar_axis)
{
    float size = 21;
    int   n    = 0;

    auto &items = EditingToolbox::Instance()->GetAllTools();
    auto cmdRegistry = Application::Instance()->CommandsRegistry();

    auto &style = ImGui::GetStyle();
    auto & colorActivated = style.Colors[ImGuiCol_ButtonHovered];
    ImVec4 colorDefault = {1, 1, 1, 1};

    EditingToolId selectedToolId = EditingToolbox::Instance()->SelectedToolId();

    for (auto it : items)
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
            CCommand *cmd = cmdRegistry->FindCommandByGlobalId(it->GetBoundCommand());

            assert(cmd);

            ImVec4 &tint = (selectedToolId == it->GetToolId()) ? colorActivated : colorDefault;

            if (ImGuiHelpers::ButtonWithCommonIcon(cmd->GetCommonIcon(), cmd->GetDescription(), size, tint))
            {
                cmd->Execute();
            }
        }

        n++;
    }
}

ToolboxPanel::ToolboxPanel(ToolUIPanelID id, const char *name) : CommandsToolbar(id, name)
{
}
