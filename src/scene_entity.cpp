/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#include "scene_entity.h"
#include "properties_editor.h"
#include "application.h"


void SceneEntity::SetClassName(const char* name)
{
	m_ClassName = std::string(name);
	m_ClassNameHash = std::hash<const char*>{} (name);
}

void SceneEntity::FlagDataLoaded()
{
	m_bDataLoaded = true;
}

void SceneEntity::InvokeSelect()
{
    auto weakRef = m_pScene->GetEntityWeakRef(this);
	m_bSelected = true;

    OnSelect(weakRef);
    SelectionManager::Instance()->UnSelectEverythingBut(this);    
}

std::string &SceneEntity::GetClassName()
{
    return m_ClassName;
}

void SceneEntity::LoadPropertiesToPropsEditor(IObjectPropertiesBinding* binder)
{
	auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();
	auto scene = sceneRenderer->GetScene();

	auto weakRef = scene->GetEntityWeakRef(this);
	scene->HintSelected(weakRef);

	ObjectPropertiesEditor::Instance()->LoadObject(binder);
}

SceneEntity::SceneEntity(Scene * pScene)
{
    m_pScene = pScene;

	m_SerialNumber = 0;
	m_Position = { 0,0,0 };

	m_Mins = { -4, -4, -4 };
	m_Maxs = { 4, 4, 4 };

	m_Color = { 0,0,0 };
	m_EditorIcon = nullptr;
}
  
SceneEntity::SceneEntity(SceneEntity& other)
{
    m_pScene = other.m_pScene;
	m_Position = other.m_Position;

	m_Mins = other.m_Mins;
	m_Maxs = other.m_Maxs;

	m_Color = other.m_Color;
	m_EditorIcon = other.m_EditorIcon;
}

void SceneEntity::RenderLightshaded()
{
	auto sceneRenderer = Application::GetMainWindow()->GetSceneRenderer();	
	sceneRenderer->RenderGenericEntity(this);
}

void SceneEntity::RenderUnshaded()
{

}

void SceneEntity::RenderBoundingBox()
{

}

void SceneEntity::RenderDebug()
{

}

void SceneEntity::RenderGroupShaded()
{

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

void SceneEntity::OnSelect(ISelectableObjectWeakRef myWeakRef)
{
	
}

void SceneEntity::OnUnSelect()
{
	
}

void SceneEntity::OnUnhovered()
{
	
}

const char* SceneEntity::Description()
{
	return m_ClassName.c_str();
}

bool SceneEntity::IsLightEntity()
{
	return false;
}

EntityClasses SceneEntity::EntityClass()
{
	return EntityClasses::GenericEntity;
}

