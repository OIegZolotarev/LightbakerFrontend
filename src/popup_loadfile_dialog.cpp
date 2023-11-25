/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "ui_common.h"
#include "popup_loadfile_dialog.h"
#include "application.h"


LoadFileDialog::LoadFileDialog() : IImGUIPopup(PopupWindows::LoadfileDialog)
{
    m_Key = "LoadmodelDlgKey";
    m_Title = "Load model";
    m_Filters = ".obj, .bsp";
}

LoadFileDialog::~LoadFileDialog()
{

}

void LoadFileDialog::Render()
{
    ImVec2 minSize = ImVec2(1280 / 2, 720 / 2);

	//ImGui::Button("ad");

    if (ImGuiFileDialog::Instance()->Display(m_Key.c_str(), ImGuiWindowFlags_NoCollapse, minSize, minSize))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			
            if (m_LoadCallback)
                m_LoadCallback(filePathName);

		}

		m_bVisible = false;	
	}
}

void LoadFileDialog::SetTitle(const char *title)
{
    m_Key = title;
}

void LoadFileDialog::SetFilters(const char *filters)
{
    m_Filters = filters;
}

void LoadFileDialog::SetOnSelectCallback(pfnOnSelectCallback callback)
{
    m_LoadCallback = callback;
}

LoadFileDialog *LoadFileDialog::Instance()
{
    return (LoadFileDialog*)PopupsManager::Instance()->FindPopupByID(PopupWindows::LoadfileDialog);
}

void LoadFileDialog::OpenPopup()
{
    if (m_bSchedulePopupOpen)
    {

		ImGui::OpenPopup(GetKey());
        ImGuiFileDialog::Instance()->OpenDialog(GetKey(), m_Title.c_str(), m_Filters.c_str(), ".",-1,nullptr, ImGuiFileDialogFlags_NoDialog);
        m_bSchedulePopupOpen = false;
    }        
}

void LoadFileDialog::Show()
{
    PopupsManager::Instance()->ShowPopup(Id());
}
