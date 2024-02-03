/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"
#include "popup_select_from_list_dialog.h"

SelectFromListDialog::SelectFromListDialog() : IImGUIPopup(PopupWindows::SelectItemFromList)
{
    m_Key = "Select item from the list";
}

SelectFromListDialog::~SelectFromListDialog()
{
}

void SelectFromListDialog::Render()
{
    // if (ImGui::BeginChild("PopupContent", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())))
    {
        ImGui::Text("Available items");
        if (ImGui::BeginListBox("##ListBoxItemns"))
        {
            for (auto &it : m_Items)
            {
                if (ImGui::Selectable(it->Description(), it == m_pSelected))
                {
                    m_pSelected = it;
                }
            }

            ImGui::EndListBox();
        }
    }

    // ImGui::EndChild();

    
    if (ImGui::Button("OK", ImVec2(75,0)))
    {
        if (m_Callback)
        {
            m_Callback(m_pSelected);
        }
    }

    ImGui::SameLine();

    
    if (ImGui::Button("Cancel", ImVec2(75, 0)))
    {
        if (m_Callback)
        {
            m_Callback(0);
        }

        Close();
    }
}

void SelectFromListDialog::SetCallback(pfnSelectDialogCallback callback)
{
    m_Callback = callback;
}

void SelectFromListDialog::AddItem(IUserPresentableObject *pObject)
{
    m_Items.push_back(pObject);
}

void SelectFromListDialog::ClearItems()
{
    m_Items.clear();
}
