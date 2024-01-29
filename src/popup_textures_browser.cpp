/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "popup_textures_browser.h"



 TexturesBrowser::TexturesBrowser() : IImGUIPopup(PopupWindows::TexturesBrowser)
{
}

TexturesBrowser::~TexturesBrowser()
{
}

void TexturesBrowser::ShowForGameConfiguration(GameConfigurationWeakPtr pConfig)
{
}

void TexturesBrowser::Render()
{
    ImGui::Text("Hello there");
}
