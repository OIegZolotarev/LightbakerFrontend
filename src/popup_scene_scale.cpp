/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "ui_common.h"
#include "popup_scene_scale.h"
#include "application.h"

SceneScaleDialog::SceneScaleDialog(): IImGUIPopup(PopupWindows::SceneScaleDialog)
{

}

SceneScaleDialog::~SceneScaleDialog()
{

}

void SceneScaleDialog::Render()
{
	static const char* key = "Scene scale";	
	auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();
	auto scene = sceneRenderer->GetScene();
	
	if (m_bVisible)
	{		
		ImGui::OpenPopup(key);
	}

	if (!ImGui::BeginPopupModal(key, &m_bVisible, ImGuiWindowFlags_AlwaysAutoResize))
		return;
	
	if (ImGui::InputFloat("Scale", &m_flScale, .1f, 1.f))
	{
		scene->SetScale(m_flScale);

		if (scene->GetSceneScale() < 0)
			scene->SetScale(0.1f);


	}
	
	if (ImGui::Button("OK"))
	{
		m_bVisible = false;		
		
		scene->SetScale(m_flScale);
		scene->RescaleLightPositions(m_flScaleOriginal, m_flScale);
	}

	ImGui::SameLine();

	if (ImGui::Button("Cancel"))
	{		
		scene->SetScale(m_flScaleOriginal);
		m_bVisible = false;
	}

	ImGui::EndPopup();
}

void SceneScaleDialog::OnOpen()
{
	auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();
	auto scene = sceneRenderer->GetScene();

	m_flScale = scene->GetSceneScale();
	m_flScaleOriginal = m_flScale;
}
