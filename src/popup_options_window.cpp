/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#include "ui_common.h"
#include "popup_options_window.h"
#include "application.h"
#include "imgui.h"
#include "imgui_internal.h"

#include "ui_options_pages.h"
#include "Camera.h"


using namespace ProgramOptions;

extern uiOptionPage_t g_OptionsPages[(int)OptionsPage::Total];


void RegisterOptions()
{
	VariantValue* opt = nullptr;

	BeginOptionPage(OptionsPage::General, "General");
	AddOption(ApplicationSettings::RebakeSceneAfterChanges, "Rebake scene after edits", PropertiesTypes::Bool);
	AddOption(ApplicationSettings::ShowGround, "Display ground", PropertiesTypes::Bool);

	AddGroup("Background");
	AddOption(ApplicationSettings::UseGradientBackground, "Use gradient", PropertiesTypes::Bool);
	AddOption(ApplicationSettings::BackgroundColor1, "Color 1", PropertiesTypes::ColorRGB);	
	AddOption(ApplicationSettings::BackgroundColor2, "Color 2 (gradient)", PropertiesTypes::ColorRGB);

	BeginOptionPage(OptionsPage::Camera, "Camera");

	opt = AddOption(ApplicationSettings::CameraControlScheme, "Camera control scheme", PropertiesTypes::Enum);
	opt->AddEnumValue("Valve Hammer Editor", (int)CameraControlScheme::ValveHammerEditor);
	opt->AddEnumValue("Blender", (int)CameraControlScheme::Blender);

	AddOption(ApplicationSettings::CameraMovementSpeed, "Movement speed", PropertiesTypes::Float);
	
	opt = AddOption(ApplicationSettings::CameraAccel, "Acceleration", PropertiesTypes::Float);
	opt->SetNumericalLimits(0, 300);

	opt = AddOption(ApplicationSettings::CameraDecel, "Decelaration", PropertiesTypes::Float);
	opt->SetNumericalLimits(0, 300);


	opt = AddOption(ApplicationSettings::CameraZNear, "Near Z-Plane", PropertiesTypes::Float);
	opt->SetNumericalLimits(0.001, 10);

	opt = AddOption(ApplicationSettings::CameraZFar, "Far Z-Plane", PropertiesTypes::Float);
	opt->SetNumericalLimits(10, 1000000);

	opt = AddOption(ApplicationSettings::CameraFov, "Field of view", PropertiesTypes::Float);
	opt->SetNumericalLimits(1, 179);

	BeginOptionPage(OptionsPage::Keyboard, "Keyboard");
	
}

OptionsDialog::OptionsDialog() : IImGUIPopup(PopupWindows::ProgramOptions)
{
	//RegisterOptions();
}

OptionsDialog::~OptionsDialog()
{

}

void OptionsDialog::Render()
{
	if (!RenderHeader())
		return;

	if (ImGui::Button("OK"))
		OnOkPressed();

	ImGui::SameLine();

	if (ImGui::Button("Cancel"))
		OnCancelPressed();


	if (ImGui::BeginTable("###LayoutLeftRight", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_SizingStretchProp, ImVec2(-FLT_MIN,-20)))
	{

		ImGui::TableSetupColumn("One", ImGuiTableColumnFlags_WidthStretch, 20 );
		ImGui::TableSetupColumn("Two", ImGuiTableColumnFlags_WidthStretch, 80);
		

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);



		ImVec2 size(-FLT_MIN,-FLT_MIN);

		uiOptionPage_t* pageToRender = nullptr;

		if (ImGui::BeginListBox("###Category", size))
		{
			for (int i = 0; i < ARRAYSIZE(g_OptionsPages); i++)
			{
				auto page = &g_OptionsPages[i];

				if (page->selected)
					pageToRender = page;

				if (ImGui::Selectable(page->pageDescription.c_str(), &page->selected, ImGuiSelectableFlags_None))
				{
					pageToRender = page;

					for (int k = 0; k < ARRAYSIZE(g_OptionsPages); k++)
					{
						if (k == i) continue;
						g_OptionsPages[k].selected = false;
					}
				}
			}

			ImGui::EndListBox();
		}



		if (pageToRender)
		{
			ImGui::TableSetColumnIndex(1);
			RenderOptionsPages(pageToRender);
		}

		ImGui::EndTable();
	}

	RenderFooter();
}

void OptionsDialog::RenderFooter()
{


	ImGui::EndPopup();
}

bool OptionsDialog::RenderHeader()
{
	static const char* key = "Options";
	auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();
	auto scene = sceneRenderer->GetScene();

	if (m_bVisible)
	{
		ImGui::OpenPopup(key);
	}

	if (!ImGui::BeginPopupModal(key, &m_bVisible, ImGuiWindowFlags_NoResize))
		return false;

	return true;
}

void OptionsDialog::RenderOptionsPages(ProgramOptions::uiOptionPage_t* page)
{

		if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_SizingStretchProp))
		{
			ImGui::TableSetupColumn("One", ImGuiTableColumnFlags_WidthStretch, 50);
			ImGui::TableSetupColumn("Two", ImGuiTableColumnFlags_WidthStretch, 50);

			int i = 0;

			for (auto& it : page->items)
			{
				ImGui::PushID(i++); // Use field index as identifier.

				// Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
				ImGui::TableNextRow();

				
				it->RenderImGUI();

				/////				

				ImGui::PopID();
			}

			ImGui::EndTable();
		}
}



void OptionsDialog::OnOpen()
{

}

void OptionsDialog::OnOkPressed()
{
	m_bVisible = false;
}

void OptionsDialog::OnCancelPressed()
{
	m_bVisible = false;
}

