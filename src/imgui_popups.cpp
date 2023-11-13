/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "imgui_popups.h"
#include "common.h"
#include "ui_common.h"
#include "custom_font.h"
#include "popup_lb3k_config.h"
#include "popup_loadfile_dialog.h"
#include "popup_scene_scale.h"
#include "popup_options_window.h"
#include "popup_edit_gameconfiguration.h"

IImGUIPopup::IImGUIPopup(PopupWindows id)
{
	m_bVisible = false;
	m_Id = id;
}


void IImGUIPopup::SetVisible(bool bFlag)
{
	m_bVisible = bFlag;
}

IImGUIPopup* PopupsManager::FindPopupByID(PopupWindows id)
{
	for (auto& it : m_vPopups)
	{
		if (it->Id() == id)
			return it;
	}

	return nullptr;
}

PopupsManager::PopupsManager()
{
	m_vPopups.clear();
	m_vPopups.push_back(new LB3kConfigPopup());
	m_vPopups.push_back(new LoadFileDialog());
	m_vPopups.push_back(new SceneScaleDialog());
	m_vPopups.push_back(new OptionsDialog());
    m_vPopups.push_back(new PopupEditGameconfiguration());
}

PopupsManager* PopupsManager::Instance()
{
	static PopupsManager* sInstance = new PopupsManager;
	return sInstance;
}

PopupsManager::~PopupsManager()
{
	ClearPointersVector(m_vPopups);
}

void PopupsManager::ApplyFileDialogSkins()
{
	//ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeDir, "", ImGui::GetStyleColorVec4(ImGuiCol_Text), std::string(ICON_IGFD_FOLDER));	
	//ImGuiFileDialog::Instance()->SetFileStyle(IGFD_FileStyleByTypeFile, "", ImGui::GetStyleColorVec4(ImGuiCol_PlotLines), std::string(ICON_IGFD_FILE));
}

void PopupsManager::ShowPopup(PopupWindows id)
{
	auto p = FindPopupByID(id);

	assert(p);

	p->OnOpen();
	p->SetVisible(true);
}

void PopupsManager::RenderPopups()
{
	// TODO: make actual popup stack, since ImGUI can't stack ones

	for (auto& it : m_vPopups)
	{
		if (it->IsVisible())
        {
            ImGui::PushID(it);
            it->Render();
            ImGui::PopID();
        }
			
	}
}
