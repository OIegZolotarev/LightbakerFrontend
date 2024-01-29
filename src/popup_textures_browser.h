/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once
#include "game_configuration.h"
#include "imgui_popups.h"

class TexturesBrowser : public IImGUIPopup
{
    GameConfigurationWeakPtr m_pGameConfiguration;

public:
    TexturesBrowser();
    ~TexturesBrowser();

    static void ShowForGameConfiguration(GameConfigurationWeakPtr pConfig);

    void Render() override;
};