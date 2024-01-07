/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "selection_tool.h"

SelectionTool::SelectionTool() : IEditingTool(EditingToolId::Selection)
{
    SetBoundCommand(GlobalCommands::ActivateSelectionTool);
    SetToolIcon(CommonIcons::SelectTool);
    SetDescritpion("Selection tool");
}

void SelectionTool::OnMouseClicked(Viewport *pActiveViewport, Scene *pActiveDocument)
{
    throw std::logic_error("The method or operation is not implemented.");
}

void SelectionTool::OnMouseReleased()
{
    throw std::logic_error("The method or operation is not implemented.");
}

void SelectionTool::Render(float flFrameDelta)
{
    throw std::logic_error("The method or operation is not implemented.");
}

void SelectionTool::RenderUI()
{
    if (!ImGui::Begin(GetDescrition(), 0, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::SetCursorPosX(ImGui::CalcTextSize(GetDescrition()).x + 30);
        ImGui::End();
        return;
    }

    
    RenderModeSelectorUI();

    
    // Hack to make sure window fits longer titles
    // https://discourse.dearimgui.org/t/how-can-make-window-size-elongate-with-the-long-title/128/2
    
    ImGui::SetCursorPosX(ImGui::CalcTextSize(GetDescrition()).x + 30);
    ImGui::End();
}

void SelectionTool::RenderModeSelectorUI()
{
    std::pair<const char *, SelectionModes> modes[] = {    
        {"Picker", SelectionModes::Picker},
        {"Paint", SelectionModes::Paint},
        {"Box", SelectionModes::BoxSelection},
    };

    ImGui::Text("Selection mode:");

    for (auto & it: modes)
    {
        if (ImGui::RadioButton(it.first, m_SelectionMode == it.second))
        {
            m_SelectionMode = it.second;
        }
    }

}
