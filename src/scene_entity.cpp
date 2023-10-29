/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#include "scene_entity.h"


SceneEntity::SceneEntity()
{
	m_SerialNumber = 0;
	m_Position = { 0,0,0 };
	m_Color = { 0,0,0 };
	m_EditorIcon = nullptr;
}
  
bool SceneEntity::IsDataLoaded()
{
	return m_bDataLoaded;
}

void SceneEntity::OnHovered()
{
	
}

void SceneEntity::OnMouseMove(glm::vec2 delta)
{
	
}

void SceneEntity::OnSelect()
{
	
}

void SceneEntity::OnUnSelect()
{
	
}

void SceneEntity::OnUnhovered()
{
	
}

void SceneEntity::RenderForSelection(int objectId, class SceneRenderer*)
{
	
}

const char* SceneEntity::Description()
{
	return "<No description>";
}

