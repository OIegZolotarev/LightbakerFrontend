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

void SceneEntity::LoadPropertiesToPropsEditor(IObjectPropertiesBinding* binder)
{
	auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();
	auto scene = sceneRenderer->GetScene();

	auto weakRef = scene->GetEntityWeakRef(this);
	scene->HintSelected(weakRef);

	ObjectPropertiesEditor::Instance()->LoadObject(binder);
}

SceneEntity::SceneEntity()
{
	m_SerialNumber = 0;
	m_Position = { 0,0,0 };

	m_Mins = { -4, -4, -4 };
	m_Maxs = { 4, 4, 4 };

	m_Color = { 0,0,0 };
	m_EditorIcon = nullptr;
}
  
SceneEntity::SceneEntity(SceneEntity& other)
{
	//m_SerialNumber = other.m_nSerialNumber;
	m_Position = other.m_Position;

	m_Mins = other.m_Mins;
	m_Maxs = other.m_Maxs;

	m_Color = other.m_Color;
	m_EditorIcon = other.m_EditorIcon;

	if (m_vProperties.size())
	{
		for (auto& kv : other.m_vProperties)
		{
			m_vProperties.insert(kv);

		}
	}
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

void SceneEntity::OnSelect()
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

void SceneEntity::CopyProperties(std::unordered_map<std::string, std::string> propsmap)
{
	m_vProperties = std::move(propsmap);
	
	if (m_vProperties.contains("classname"))
	{
		m_ClassName = m_vProperties["classname"];
	}

}

TPropertiesMap & SceneEntity::GetProperties()
{
	return m_vProperties;
}

EntityClasses SceneEntity::EntityClass()
{
	return EntityClasses::GenericEntity;
}

