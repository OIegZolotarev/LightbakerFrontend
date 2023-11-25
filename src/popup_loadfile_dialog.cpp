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
	//if ()
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			//std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			// action

			//Application::Instance()->DoLoadModel(filePathName.c_str(), true);
			Application::GetMainWindow()->GetSceneRenderer()->LoadModel(filePathName.c_str(), LRF_LOAD_ALL);

		}

		m_bVisible = false;	
	}
}

bool LoadFileDialog::BeginRendering()
{    
	ImVec2 minSize = ImVec2(1280 / 2, 720 / 2);
	return ImGuiFileDialog::Instance()->Display(m_Key.c_str(), 32, minSize);
}

void LoadFileDialog::EndRendering()
{
    ImGuiFileDialog::Instance()->Close();
}

void LoadFileDialog::SetTitle(const char *title)
{
}

void LoadFileDialog::SetFilters(const char *filters)
{
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
        ImGuiFileDialog::Instance()->OpenDialog(GetKey(), m_Title.c_str(), m_Filters.c_str(), ".");
        m_bSchedulePopupOpen = false;
    }        
}
