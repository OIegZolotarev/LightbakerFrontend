/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "texture_application_tool.h"

TextureApplicationTool::TextureApplicationTool() : IEditingTool(EditingToolId::TexturesApplication)
{
    SetToolIcon(CommonIcons::TextureTool);
    SetDescritpion("Texture application tool");
}

void TextureApplicationTool::Render(float flFrameDelta)
{
}

void TextureApplicationTool::RenderUI()
{


    ImGui::Begin(GetDescrition());

    if (!m_pActiveDocument)
    {
        ImGui::Text("No active document");
        ImGui::End();
        return;    
    }

    auto conf = m_pActiveDocument->UsedGameConfiguration();

    if (conf.expired())
    {
        ImGui::Text("No active game configuration");
        ImGui::End();
        return;
    }

    ImGui::End();
}
