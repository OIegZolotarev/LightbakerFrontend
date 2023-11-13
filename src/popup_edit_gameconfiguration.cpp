/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "popup_edit_gameconfiguration.h"
#include "ui_common.h"

PopupEditGameconfiguration::PopupEditGameconfiguration() : IImGUIPopup(PopupWindows::EditGameConfiguration)
{
}

PopupEditGameconfiguration::~PopupEditGameconfiguration()
{
}

void PopupEditGameconfiguration::Render()
{
    static const char *key = "Edit game configuration";

    if (m_bVisible)
    {
        ImGui::OpenPopup(key);
    }

    if (ImGui::BeginPopupModal(key, &m_bVisible, ImGuiWindowFlags_AlwaysAutoResize))
    {
        auto ptr = m_pCurrentConfiguration.lock();

         if (ptr)
         {
             ptr->EditDialog();
         }
         else
             ImGui::Text("<Configuration in destroyed>");

        

        if (ImGui::Button("OK"))
        {
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
        }

        ImGui::EndPopup();
    }
}

void PopupEditGameconfiguration::OnOpen()
{
}

void PopupEditGameconfiguration::SetGameConfiguration(GameConfigurationWeakPtr conf)
{
    m_pCurrentConfiguration = conf;
}
