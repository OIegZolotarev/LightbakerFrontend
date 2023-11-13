/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "ui_common.h"
#include "popup_loadfile_dialog.h"
#include "application.h"


LoadFileDialog::LoadFileDialog() : IImGUIPopup(PopupWindows::LoadfileDialog)
{

}

LoadFileDialog::~LoadFileDialog()
{

}

void LoadFileDialog::Render()
{
	if (m_bVisible)
		ImGuiFileDialog::Instance()->OpenDialog("LoadmodelDlgKey", "Load model", ".obj | .bsp", ".");

	ImVec2 minSize = ImVec2(1280 / 2, 720 / 2);

	if (ImGuiFileDialog::Instance()->Display("LoadmodelDlgKey", 32, minSize))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			// action

			//Application::Instance()->DoLoadModel(filePathName.c_str(), true);
			Application::GetMainWindow()->GetSceneRenderer()->LoadModel(filePathName.c_str(), LRF_LOAD_ALL);

		}

		m_bVisible = false;



		ImGuiFileDialog::Instance()->Close();
	}
}
