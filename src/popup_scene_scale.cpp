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
	
	if (m_bVisible)
	{		
		ImGui::OpenPopup(key);
	}

	if (!ImGui::BeginPopupModal(key, &m_bVisible, ImGuiWindowFlags_AlwaysAutoResize))
		return;
	
	if (ImGui::InputFloat("Scale", &m_flScale, .1f, 1.f))
	{
		sceneRenderer->SetSceneScale(m_flScale);

		if (sceneRenderer->GetSceneScale() < 0)
			sceneRenderer->SetSceneScale(0.1f);


	}
	
	if (ImGui::Button("OK"))
	{
		m_bVisible = false;		
		
		sceneRenderer->SetSceneScale(m_flScale);
		sceneRenderer->RescaleLightPositions(m_flScaleOriginal, m_flScale);
	}

	ImGui::SameLine();

	if (ImGui::Button("Cancel"))
	{		
		sceneRenderer->SetSceneScale(m_flScaleOriginal);
		m_bVisible = false;
	}

	ImGui::EndPopup();
}

void SceneScaleDialog::OnOpen()
{
	auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();
	m_flScale = sceneRenderer->GetSceneScale();
	m_flScaleOriginal = m_flScale;
}
