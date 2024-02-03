/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "game_configuration.h"
#include "imgui_popups.h"

class PopupEditGameconfiguration : public IImGUIPopup
{
    GameConfigurationWeakPtr m_pCurrentConfiguration;
    GameConfiguration *m_pEditedConfiguration = nullptr;

public:
    PopupEditGameconfiguration();
    ~PopupEditGameconfiguration();

    void Render() override;
    void OnOpen() override;

    void SetGameConfiguration(GameConfigurationWeakPtr conf);
};