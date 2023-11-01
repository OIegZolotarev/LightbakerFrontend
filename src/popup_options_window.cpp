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

extern uiOptionPage_t g_OptionsPages[(int)OptionsPage::Total];


void RegisterOptions()
{
	BeginOptionPage(OptionsPage::General, "General");
	AddOption(ApplicationSettings::RebakeSceneAfterChanges, "Rebake scene after edits", PropertiesTypes::Bool);
	AddOption(ApplicationSettings::ShowGround, "Display ground", PropertiesTypes::Bool);

	AddOption(ApplicationSettings::BackgroundColor1, "Background color 1", PropertiesTypes::ColorRGB);
	AddOption(ApplicationSettings::UseGradientBackground, "Use gradient background", PropertiesTypes::Bool);
	AddOption(ApplicationSettings::BackgroundColor2, "		color 2", PropertiesTypes::ColorRGB);

	BeginOptionPage(OptionsPage::Camera, "Camera");
	AddOption(ApplicationSettings::CameraMovementSpeed, "Movement speed", PropertiesTypes::Float);
	AddOption(ApplicationSettings::CameraZNear, "Near Z-Plane", PropertiesTypes::Float);
	AddOption(ApplicationSettings::CameraZFar, "Far Z-Plane", PropertiesTypes::Float);

	BeginOptionPage(OptionsPage::Keyboard, "Keyboard");
	auto opt = AddOption(ApplicationSettings::CameraControlScheme, "Camera control scheme", PropertiesTypes::Enum);
	opt->AddEnumValue("Valve Hammer Editor", (int)CameraControlScheme::ValveHammerEditor);
	opt->AddEnumValue("Blender", (int)CameraControlScheme::Blender);
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

void OptionsDialog::RenderOptionsPages(uiOptionPage_t* page)
{

		if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_SizingStretchProp))
		{
			ImGui::TableSetupColumn("One", ImGuiTableColumnFlags_WidthStretch, 50);
			ImGui::TableSetupColumn("Two", ImGuiTableColumnFlags_WidthStretch, 50);

			int i = 0;

			for (auto& it : page->properties)
			{
				ImGui::PushID(i++); // Use field index as identifier.

				// Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);

				/////
				ImGui::AlignTextToFramePadding();
				ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
				ImGui::TreeNodeEx("Field", flags, "%s", it.DisplayName());
				/////

				ImGui::TableSetColumnIndex(1);

				/////
				ImGui::SetNextItemWidth(-FLT_MIN);
				RenderPropertyControl(it);
				/////				

				ImGui::PopID();
			}

			ImGui::EndTable();
		}
}

void OptionsDialog::RenderPropertyControl(VariantValue& it)
{
	switch (it.GetType())
	{
	case PropertiesTypes::Enum:

		RenderEnumProperty(&it);
		break;
	case PropertiesTypes::Flags:
		RenderFlagsProperty(&it);
		break;
	case PropertiesTypes::Position:
		if (ImGui::DragFloat3("Vector", (float*)it.Data()))
			UpdateProperty(&it);
		break;
	case PropertiesTypes::ColorRGB:
		if (ImGui::ColorEdit3("##value", (float*)it.Data()))
			UpdateProperty(&it);
		break;
	case PropertiesTypes::ColorRGBA:
		if (ImGui::ColorEdit4("##value", (float*)it.Data()))
			UpdateProperty(&it);
		break;
	case PropertiesTypes::Float:
		if (ImGui::DragFloat("##value", (float*)it.Data(), 0.01f))
			UpdateProperty(&it);
		break;
	case PropertiesTypes::Angles:
		if (ImGui::DragFloat3("Vector", (float*)it.Data()))
			UpdateProperty(&it);
		break;
	case PropertiesTypes::Int:
		if (ImGui::DragInt("##value", (int*)it.Data(), 0.01f))
			UpdateProperty(&it);
		break;
	case PropertiesTypes::Bool:
		if (ImGui::Checkbox("##value", (bool*)it.Data()))
			UpdateProperty(&it);
		break;
	case PropertiesTypes::SizeX:
		if (ImGui::DragFloat("Vector", (float*)it.Data()))
			UpdateProperty(&it);
		break;
	default:
		break;

	}
}

void OptionsDialog::RenderFlagsProperty(VariantValue* it)
{
	if (ImGui::Button("..."))
	{
		ImGui::OpenPopup(ImGui::GetID("Flags"));
		//m_pCurrentFlagProp = &it;
	}

	ImGui::SameLine();

	std::string flagDescription = "[";

	for (auto flagValue: it->GetEnumValues())
	{
		if (it->GetFlags() & flagValue.second)
			flagDescription += flagValue.first + ",";
	}

	if (flagDescription.length() > 1)
		flagDescription[flagDescription.length() - 1] = ']';
	else
		flagDescription += "0]";

	ImGui::Text("%s", flagDescription.c_str());
}

void OptionsDialog::RenderEnumProperty(VariantValue* it)
{
	if (it->GetEnumValues().size() > 0)
	{
		int selectionIndex = it->GetInt();
		it->SetTempLabel((char*)it->GetEnumValues()[selectionIndex].first.c_str());

		if (ImGui::BeginCombo(it->DisplayName(), it->TempLabel()))
		{
			int i = 0;

			for (auto& enumValue : it->GetEnumValues())
			{
				if (ImGui::Selectable(enumValue.first.c_str(), i == selectionIndex))
				{					
					it->SetInt(i);
					it->SetTempLabel((char*)enumValue.first.c_str());
					UpdateProperty(it);
				}

				i++;
			}

			ImGui::EndCombo();
		}
	}
	else
	{
		ImGui::Text("No values defined!");
	}
}

void OptionsDialog::UpdateProperty(VariantValue* it)
{
	//throw std::logic_error("The method or operation is not implemented.");
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

