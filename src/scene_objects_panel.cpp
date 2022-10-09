#include "common.h"
#include "ui_common.h"
#include "scene_objects_panel.h"
#include "application.h"

void SceneObjectPanel::RenderSceneObjectsPanel()
{
	if (ImGui::Begin(m_strPanelTitle))
	{
		auto sceneRenderer = Application::GetMainWindow()->GetSceneRenderer();

		if (ImGui::Button("Add"))
		{

		}

		ImGui::SameLine();

		if (ImGui::Button("Delete"))
		{
			sceneRenderer->DoDeleteSelection();			
		}

		if (ImGui::TreeNode("root"))
		{
			auto& sceneObjects = sceneRenderer->GetSceneObjects();

			if (ImGui::TreeNode("Lights"))
			{
				for (auto& it : sceneObjects)
				{
					ImGui::PushID((void*)it.get());
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(it->color[0],it->color[1],it->color[2],1));

					if (ImGui::Selectable(it->Description(), it->IsSelected()))
					{
						it->InvokeSelect();
					}

					ImGui::PopStyleColor();
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
