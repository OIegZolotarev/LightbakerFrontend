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

    ImVec2 scrolling_child_size = ImVec2(0, ImGui::GetFrameHeightWithSpacing() * 7 + 30);


    if (ImGui::BeginChild("scrolling", scrolling_child_size, ImGuiChildFlags_Border,
                          ImGuiWindowFlags_AlwaysVerticalScrollbar))
    {
        for (auto &it : m_pSelectedAssetsGroup->Assets())
        {
            //         // Display random stuff. For the sake of this trivial demo we are using basic Button() +
            //         SameLine()
            //         // If you want to create your own time line for a real application you may be better off
            //         manipulating
            //         // the cursor position yourself, aka using SetCursorPos/SetCursorScreenPos to position the
            //         widgets
            //         // yourself. You may also want to use the lower-level ImDrawList API.
            //         int num_buttons = 10 + ((line & 1) ? line * 9 : line * 3);
            //         for (int n = 0; n < num_buttons; n++)
            //         {
            //             if (n > 0)
            //                 ImGui::SameLine();
            //             ImGui::PushID(n + line * 1000);
            //             char num_buf[16];
            //             sprintf(num_buf, "%d", n);
            //             const char *label = (!(n % 15)) ? "FizzBuzz" : (!(n % 3)) ? "Fizz" : (!(n % 5)) ? "Buzz" :
            //             num_buf; float       hue   = n * 0.05f; ImGui::PushStyleColor(ImGuiCol_Button,
            //             (ImVec4)ImColor::HSV(hue, 0.6f, 0.6f)); ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
            //             (ImVec4)ImColor::HSV(hue, 0.7f, 0.7f)); ImGui::PushStyleColor(ImGuiCol_ButtonActive,
            //             (ImVec4)ImColor::HSV(hue, 0.8f, 0.8f)); ImGui::Button(label, ImVec2(40.0f + sinf((float)(line
            //             + n)) * 20.0f, 0.0f)); ImGui::PopStyleColor(3); ImGui::PopID();
            //         }

            ImGui::Button(it->Description());
        }
        float scroll_x     = ImGui::GetScrollX();
        float scroll_max_x = ImGui::GetScrollMaxX();
        ImGui::EndChild();
    }


    ImGui::End();
}
