/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "common.h"
#include "ui_common.h"
#include "scene_objects_panel.h"
#include "application.h"

void SceneObjectPanel::RenderSceneObjectsPanel()
{
	if (ImGui::Begin(m_strPanelTitle))
	{
		auto sceneRenderer = Application::GetMainWindow()->GetSceneRenderer();
		auto scene = sceneRenderer->GetScene();

		if (ImGui::Button("Add"))
		{

		}

		ImGui::SameLine();

		if (ImGui::Button("Delete"))
		{
			scene->DoDeleteSelection();
		}

		if (ImGui::TreeNode("root"))
		{
			auto& sceneObjects = scene->GetSceneObjects();

			auto worldSpawn = sceneObjects.begin();


			if (ImGui::Selectable((*worldSpawn)->Description(), (*worldSpawn)->IsSelected()))
			{
				(*worldSpawn)->InvokeSelect();
				sceneRenderer->FocusCameraOnObject((*worldSpawn));
			}

			if (ImGui::TreeNode("Lights"))
			{
				for (auto& it : sceneObjects)
				{
// 					if (!it->IsLightEntity())
// 						continue;

					ImGui::PushID((void*)it.get());

					auto col = it->GetColor();

					//ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(it->color[0],it->color[1],it->color[2],1));

					ImGui::ColorEdit3("ColorPreview", (float*)&col, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip  |ImGuiColorEditFlags_NoPicker  | ImGuiColorEditFlags_NoLabel);
					ImGui::SameLine();

					if (ImGui::Selectable(it->Description(), it->IsSelected()))
					{
						it->InvokeSelect();
						sceneRenderer->FocusCameraOnObject(it);
					}

					//ImGui::PopStyleColor();
					ImGui::PopID();
				}

				ImGui::TreePop();
			}

			ImGui::TreePop();
		}

	}
	ImGui::End();
}

SceneObjectPanel::SceneObjectPanel() : IGUIPanel(PanelsId::SceneObjects,(char*)"Scene objects")
{

}

DockPanels SceneObjectPanel::GetDockSide()
{
	return DockPanels::RightBottom;
}

void SceneObjectPanel::Render()
{
	RenderSceneObjectsPanel();
}
