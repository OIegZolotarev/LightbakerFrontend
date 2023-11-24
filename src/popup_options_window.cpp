/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "popup_options_window.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "ui_common.h"

#include "Camera.h"
#include "helpers.h"
#include "imgui_popups.h"
#include "popup_edit_gameconfiguration.h"
#include "ui_options_pages.h"
#include "ui_styles_manager.h"

using namespace ProgramOptions;

extern uiOptionPage_t g_OptionsPages[(int)OptionsPage::Total];

void RegisterOptions()
{
    VariantValue *opt = nullptr;

    BeginOptionPage(OptionsPage::General, "General");

    AddGroup("Scene");
    AddOption(ApplicationSettings::RebakeSceneAfterChanges, "Auto-rebake after changes", PropertiesTypes::Bool);
    AddOption(ApplicationSettings::ShowGround, "Display ground", PropertiesTypes::Bool);

    AddGroup("Background");
    AddOption(ApplicationSettings::UseGradientBackground, "Use gradient", PropertiesTypes::Bool);
    AddOption(ApplicationSettings::BackgroundColor1, "Color 1", PropertiesTypes::ColorRGB);
    AddOption(ApplicationSettings::BackgroundColor2, "Color 2 (gradient)", PropertiesTypes::ColorRGB);

    BeginOptionPage(OptionsPage::Camera, "Camera");

    // Порядок перечислений должен совпадать с основным перечислением!!
    opt = AddOption(ApplicationSettings::CameraControlScheme, "Camera control scheme", PropertiesTypes::Enum);
    opt->AddEnumValue("Valve Hammer Editor", (int)CameraControlScheme::ValveHammerEditor);
    opt->AddEnumValue("Blender", (int)CameraControlScheme::Blender);
    opt->AddEnumValue("Blender (touchpad)", (int)CameraControlScheme::BlenderTouchpad);

    opt = AddOption(ApplicationSettings::CameraMouseSensivityRotating, "Mouse sensitivity (rotation)",
                    PropertiesTypes::Float);
    opt->SetNumericalLimits(0.1, 10);

    opt = AddOption(ApplicationSettings::CameraMouseSensivityPaning, "Mouse sensitivity (pan)", PropertiesTypes::Float);
    opt->SetNumericalLimits(0.1, 10);

    opt = AddOption(ApplicationSettings::CameraMouseSensivityZooming, "Mouse sensitivity (zoom)",
                    PropertiesTypes::Float);
    opt->SetNumericalLimits(0.1, 10);

    AddGroup("Movement");

    AddOption(ApplicationSettings::CameraMovementSpeed, "Speed", PropertiesTypes::Float);

    opt = AddOption(ApplicationSettings::CameraAccel, "Acceleration", PropertiesTypes::Float);
    // opt->SetNumericalLimits(0, 300);

    opt = AddOption(ApplicationSettings::CameraDecel, "Deceleration", PropertiesTypes::Float);
    // opt->SetNumericalLimits(0, 300);

    AddGroup("Parameters");

    opt = AddOption(ApplicationSettings::CameraFov, "Field of view", PropertiesTypes::Float);
    opt->SetNumericalLimits(1, 179);

    opt = AddOption(ApplicationSettings::CameraZNear, "Near plane", PropertiesTypes::Float);
    opt->SetNumericalLimits(0.001, 10);

    opt = AddOption(ApplicationSettings::CameraZFar, "Far plane", PropertiesTypes::Float);
    opt->SetNumericalLimits(10, 1000000);

    BeginOptionPage(OptionsPage::Appearence, "Appeareance");

    AddGroup("Appearance");
    opt = AddOption(ApplicationSettings::GUIColorScheme, "UI colors", PropertiesTypes::Enum);
    UIStyles::Manager::Instance()->PopulateStylesOption(opt);

    AddGroup("Grid");
    opt = AddOption(ApplicationSettings::GridAxisColor, "Axis color", PropertiesTypes::ColorRGB);
    opt = AddOption(ApplicationSettings::GridMainColor, "Main color", PropertiesTypes::ColorRGB);
    opt = AddOption(ApplicationSettings::Grid64thLineColor, "64th lines color", PropertiesTypes::ColorRGB);
    opt = AddOption(ApplicationSettings::Grid1024thLineColor, "1024th lines color", PropertiesTypes::ColorRGB);
    opt = AddOption(ApplicationSettings::GridCustomColor, "Custom color", PropertiesTypes::ColorRGB);
    opt = AddOption(ApplicationSettings::GridCustomStep, "Highlight every n'th line", PropertiesTypes::Int);
}

OptionsDialog::OptionsDialog() : IImGUIPopup(PopupWindows::ProgramOptions)
{
    m_Key = "Preferences";
}

OptionsDialog::~OptionsDialog()
{
}

void OptionsDialog::Render()
{
    if (!RenderHeader())
        return;

    ImVec2 size(-FLT_MIN, -FLT_MIN);

    uiOptionPage_t *pageToRender = nullptr;

    if (ImGui::BeginTabBar("###Category"))
    {
        for (int i = 0; i < ARRAYSIZE(g_OptionsPages); i++)
        {
            auto page = &g_OptionsPages[i];

            if (page->selected)
                pageToRender = page;

            // if (ImGui::Selectable(page->pageDescription.c_str(), &page->selected, ImGuiSelectableFlags_None))
            if (ImGui::BeginTabItem(page->pageDescription.c_str()))
            {
                pageToRender = page;

                for (int k = 0; k < ARRAYSIZE(g_OptionsPages); k++)
                {
                    if (k == i)
                        continue;
                    g_OptionsPages[k].selected = false;
                }

                ImGui::EndTabItem();
            }
        }

        if (ImGui::BeginTabItem("Game configurations"))
        {
            if (ImGui::BeginChild("ChildId", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())))
            {
                RenderGameConfigurationsPage();
                ImGui::EndChild();
            }

            ImGui::EndTabItem();
        }

        // ImGui::EndListBox();
        ImGui::EndTabBar();
    }

    if (pageToRender)
    {
        if (ImGui::BeginChild("ChildId", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())))
        {
            RenderOptionsPages(pageToRender);
            ImGui::EndChild();
        }
    }

    RenderFooter();
}

void OptionsDialog::RenderFooter()
{
    if (ImGui::Button("OK"))
        OnOkPressed();

    ImGui::SameLine();

    if (ImGui::Button("Cancel"))
        OnCancelPressed();    
}

bool OptionsDialog::RenderHeader()
{
    auto sceneRenderer     = Application::Instance()->GetMainWindow()->GetSceneRenderer();
    auto scene             = sceneRenderer->GetScene();

// 
//     if (!ImGui::BeginPopupModal(key, &m_bVisible, ImGuiWindowFlags_NoResize))
//         return false;

    
    ImGui::SetWindowSize(ImVec2(600, 400));

    return true;
}

void OptionsDialog::RenderOptionsPages(ProgramOptions::uiOptionPage_t *page)
{
    if (ImGui::BeginChildFrame(53, ImVec2(0, 0)))
    {
        if (ImGui::BeginTable("split", 2, ImGuiTableFlags_SizingStretchProp))
        {
            ImGui::TableSetupColumn("One", ImGuiTableColumnFlags_WidthStretch, 50);
            ImGui::TableSetupColumn("Two", ImGuiTableColumnFlags_WidthStretch, 50);

            int i = 0;

            for (auto &it : page->items)
            {
                ImGui::PushID(i++); // Use field index as identifier.

                // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
                ImGui::TableNextRow();

                it->RenderImGUI();

                ImGui::PopID();
            }

            ImGui::EndTable();
        }
        ImGui::EndChildFrame();
    }
}

void OptionsDialog::RenderGameConfigurationsPage()
{
    static GameConfigurationWeakPtr selectedConf;

    ImGui::SeparatorText("Registered configurations:");

    if (ImGui::BeginTable("###GroupsLeftRight", 2))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);

        ImGui::SetNextItemWidth(-1);

        if (ImGui::BeginListBox("###RegisteredConfigurations"))
        {
            auto items = GameConfigurationsManager::Instance()->AllConfigurationsWeakPtr();
            for (auto &it : items)
            {
                auto ptr = it.lock();

                if (!ptr)
                    continue;

                auto desc = ptr->Description();
                

                if (ImGui::Selectable(desc, equals(selectedConf, ptr)))
                {
                    selectedConf = it;
                }
            }

            ImGui::EndListBox();
        }

        ImGui::TableSetColumnIndex(1);

        if (ImGui::Button("Add"))
        {
        }

        if (ImGui::Button("Edit..."))
        {
                auto popup = (PopupEditGameconfiguration *)PopupsManager::Instance()->FindPopupByID(

        }

        if (ImGui::Button("Remove"))
        {
        }

        ImGui::EndTable();
    }
}

void OptionsDialog::OnOpen()
{
}

int OptionsDialog::RenderingFlags()
{
    return ImGuiWindowFlags_NoResize;
}

void OptionsDialog::OnOkPressed()
{
    m_bVisible = false;
}

void OptionsDialog::OnCancelPressed()
{
    m_bVisible = false;
}

void GameConfigurationListBinding::AddNewItem()
{
    // TODO: implement
}

const char *GameConfigurationListBinding::ItemDescription(size_t index)
{
    auto items = GameConfigurationsManager::Instance()->AllConfigurationsWeakPtr();

    auto b = items.begin();
    std::advance(b, index);

    auto ptr = b->lock();

    if (ptr)
    {
        return ptr->Name();
    }
    else
    {
        return "<Item destroyed>";
    }
}

size_t GameConfigurationListBinding::ItemsCount()
{
    auto items = GameConfigurationsManager::Instance()->AllConfigurationsWeakPtr();
    return items.size();
}

void GameConfigurationListBinding::MoveItemDown()
{
    if (!m_SelectedConf.lock())
        return;

    // TODO: implement
    //throw std::logic_error("The method or operation is not implemented.");
}

void GameConfigurationListBinding::MoveItemUp()
{
    if (!m_SelectedConf.lock())
        return;

    // TODO: implement
    // throw std::logic_error("The method or operation is not implemented.");
}

void GameConfigurationListBinding::OpenItemEditor()
{
    if (m_SelectedConf.lock())
    {
        auto popup =
            (PopupEditGameconfiguration *)PopupsManager::Instance()->FindPopupByID(PopupWindows::EditGameConfiguration);
        popup->SetGameConfiguration(m_SelectedConf);
        PopupsManager::Instance()->ShowPopup(PopupWindows::EditGameConfiguration);
    }
}

void GameConfigurationListBinding::RemoveItem(size_t item)
{
    auto & items = GameConfigurationsManager::Instance()->AllConfigurations();

    auto it = items.begin();
    std::advance(it, item);
    items.erase(it);
}

void GameConfigurationListBinding::SortItems(SortDirection dir)
{
    throw std::logic_error("The method or operation is not implemented.");
}

void GameConfigurationListBinding::SetSelectedItem(size_t index)
{
    auto items = GameConfigurationsManager::Instance()->AllConfigurationsWeakPtr();

    auto b = items.begin();
    std::advance(b, index);
    m_SelectedConf = *b;
}
