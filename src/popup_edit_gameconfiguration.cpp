/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "popup_edit_gameconfiguration.h"
#include "ui_common.h"
#include "popup_file_dialog.h"

PopupEditGameconfiguration::PopupEditGameconfiguration() : IImGUIPopup(PopupWindows::EditGameConfiguration)
{
    m_Key = "Edit game configuration";
}

PopupEditGameconfiguration::~PopupEditGameconfiguration()
{
}

void PopupEditGameconfiguration::Render()
{
  
    {
        auto ptr = m_pCurrentConfiguration.lock();

         if (ptr)
         {
             m_pEditedConfiguration->EditDialog();
         }
         else
             ImGui::Text("<Configuration in destroyed>");

        if (ImGui::Button("OK"))
        {
            GameConfigurationsManager::Instance()->UpdateGameConfiguration(m_pCurrentConfiguration,
                                                                           m_pEditedConfiguration);

            // GameConfigurationsManager takes ownership
            //delete m_pEditedConfiguration;
            m_bVisible = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            delete m_pEditedConfiguration;
            m_bVisible = false;
        }        
    }
}

void PopupEditGameconfiguration::OnOpen()
{
}

void PopupEditGameconfiguration::SetGameConfiguration(GameConfigurationWeakPtr conf)
{
    m_pCurrentConfiguration = conf;

    auto ptr = m_pCurrentConfiguration.lock();

    if (!ptr)
    {
        m_pEditedConfiguration = nullptr;
        return;
    }

    m_pEditedConfiguration = ptr->Clone();
}
