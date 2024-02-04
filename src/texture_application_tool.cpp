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

    auto ptr = conf.lock();
    auto assetsProvider = ptr->GetMaterialAssetsProvider();

    if (assetsProvider == nullptr)
    {
        ImGui::Text("Game configuration does not provide assets");
        ImGui::End();
        return;
    }

    if (assetsProvider->AssetsGroups().size() == 0)
    {
        ImGui::Text("No assets found for game configuration");
        ImGui::End();
        return;
    }

    ImGui::Text("Category");

    if (!m_pSelectedAssetsGroup)
    {
        m_pSelectedAssetsGroup = assetsProvider->AssetsGroups()[0];
    }

    if (ImGui::BeginCombo("##ListBoxTextureCategory", m_pSelectedAssetsGroup ? m_pSelectedAssetsGroup->Description() : "<Not selected>"))
    {
        for (auto &it : assetsProvider->AssetsGroups())
        {
            if (ImGui::Selectable(it->Description(), m_pSelectedAssetsGroup == it))
            {
                m_pSelectedAssetsGroup = it;
            }
        }

        ImGui::EndCombo();
    }

    ImVec2 scrolling_child_size = ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 15 + 30);


    if (ImGui::BeginChild("scrolling", scrolling_child_size, ImGuiChildFlags_Border,
                          ImGuiWindowFlags_AlwaysVerticalScrollbar))
    {


        int i = 0;

        if (m_pSelectedAssetsGroup != nullptr)
        {
            for (auto &it : m_pSelectedAssetsGroup->Assets())
            {

                auto texture = it->GetEditorImage();

                if (i < 10)
                {
                    ImGui::SameLine();
                }
                else
                {

                    i = 0;
                }

                ImGui::ImageButton((ImTextureID)texture->GLTextureNum(),
                                   ImVec2(texture->Width() * 2, texture->Height() * 2));

                i++;
            }
        }

        float scroll_x     = ImGui::GetScrollX();
        float scroll_max_x = ImGui::GetScrollMaxX();
        ImGui::EndChild();
    }


    ImGui::End();
}
