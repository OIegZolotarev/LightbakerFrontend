/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "viewports_orchestrator.h"
#include "selection_tool.h"

SelectionTool::SelectionTool() : IEditingTool(EditingToolId::Selection)
{
    SetBoundCommand(GlobalCommands::ActivateSelectionTool);
    SetToolIcon(CommonIcons::SelectTool);
    SetDescritpion("Selection tool");
}


void SelectionTool::Render(float flFrameDelta)
{
    // throw std::logic_error("The method or operation is not implemented.");
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

    Viewport *pHoveredViewport = ViewportsOrchestrator::Instance()->GetHoveredViewport();

    if (pHoveredViewport)
    {
        ImGui::Text("Hovering above %s", pHoveredViewport->Name());
    }
    else
        ImGui::Text("Not hovering above viewport...");
    
    // Hack to make sure window fits longer titles
    // https://discourse.dearimgui.org/t/how-can-make-window-size-elongate-with-the-long-title/128/2    
    ImGui::SetCursorPosX(ImGui::CalcTextSize(GetDescrition()).x + 30);

    ImGui::End();
}

int SelectionTool::HandleKeyboardEvent(bool bWasHandled, const SDL_Event &e, const float flFrameDelta)
{
    return 0;
}

int SelectionTool::HandleMouseEvent(bool bWasHandled, const SDL_Event &e, const float flFrameDelta)
{
    int buttons = e.button.button;

    switch (e.type)
    {   
    case SDL_MOUSEBUTTONDOWN:
        
        if (buttons & SDL_BUTTON_LMASK)
        {
            m_bMouseDragValid   = true;
            m_vecMouseDragStart = m_pActiveViewport->CalcRelativeMousePos();
            return EVENT_CONSUMED;    
        }

        return 0;
        
    case SDL_MOUSEBUTTONUP:        

        if (buttons & SDL_BUTTON_LMASK)
        {
            //m_bMouseDragValid = true;
            m_vecMouseDragEnd = m_pActiveViewport->CalcRelativeMousePos();
            return EVENT_CONSUMED;
        }

        return 0;
    }

    return 0;
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
