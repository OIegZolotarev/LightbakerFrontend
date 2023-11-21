/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "imgui_popups.h"
#include "game_configuration.h"

class PopupEditGameconfiguration : public IImGUIPopup
{
    float m_flScale         = 0;
    float m_flScaleOriginal = 0;

    GameConfigurationWeakPtr m_pCurrentConfiguration;
    
    GameConfiguration *m_pEditedConfiguration = nullptr;

 public:
 
    PopupEditGameconfiguration();
    ~PopupEditGameconfiguration();

    void Render() override;
    void OnOpen() override;

    void SetGameConfiguration(GameConfigurationWeakPtr conf);
};