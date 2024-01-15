/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "ui_common.h"
#include "popup_file_dialog.h"
#include "application.h"


FileDialog::FileDialog() : IImGUIPopup(PopupWindows::FileDialog)
{
    m_Key = "LoadmodelDlgKey";
    m_Title = "Load model";
    m_Filters = ".obj, .bsp";
}

FileDialog::~FileDialog()
{

}

void FileDialog::Render()
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

void FileDialog::SetTitle(const char *title)
{
    m_Key = title;
}

void FileDialog::SetFilters(const char *filters)
{
    m_Filters = filters;
}

void FileDialog::SetOnSelectCallback(pfnOnSelectCallback callback)
{
    m_LoadCallback = callback;
}

FileDialog *FileDialog::Instance()
{
    return (FileDialog*)PopupsManager::Instance()->FindPopupByID(PopupWindows::FileDialog);
}

void FileDialog::OpenPopup()
{
    if (m_bSchedulePopupOpen)
    {

		ImGui::OpenPopup(GetKey());
        ImGuiFileDialog::Instance()->OpenDialog(GetKey(), m_Title.c_str(), m_Filters.c_str(), ".",-1,nullptr, ImGuiFileDialogFlags_NoDialog);
        m_bSchedulePopupOpen = false;
    }        
}

void FileDialog::Show()
{
    PopupsManager::Instance()->ShowPopup(Id());
}
