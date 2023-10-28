/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "edit_history.h"
#include <xutility>
#include "application.h"
#include "properties_editor.h"



CPropertyChangeAction::CPropertyChangeAction(int serialNumber, propsData_t oldValue, propsData_t newValue)
{
	m_SerialNumber = serialNumber;
	m_OldValue = oldValue;
	m_NewValue = newValue;
}


void CPropertyChangeAction::Redo()
{
	auto sceneRender = Application::GetMainWindow()->GetSceneRenderer();
	auto scene = sceneRender->GetScene();

	SceneEntityWeakPtr pLight = scene->GetEntityBySerialNumber(m_SerialNumber);

	if (auto ptr = pLight.lock())
	{
		lightDefWPtr_t lightPtr = std::dynamic_pointer_cast<lightDef_s>(ptr);

		auto b = new LightPropertiesBinding(lightPtr);
		b->UpdateObjectProperties(&m_NewValue, 1);
		delete b;

		if (ObjectPropertiesEditor::Instance()->CurrentSerialNumber() == m_SerialNumber)
		{
			ObjectPropertiesEditor::Instance()->ReloadPropertyValue(m_OldValue.id);
		}
	}
}

void CPropertyChangeAction::Undo()
{
	auto sceneRender = Application::GetMainWindow()->GetSceneRenderer();
	auto scene = sceneRender->GetScene();

	SceneEntityWeakPtr pLight = scene->GetEntityBySerialNumber(m_SerialNumber);

	if (auto ptr = pLight.lock())
	{
		lightDefWPtr_t lightPtr = std::dynamic_pointer_cast<lightDef_s>(ptr);

		auto b = new LightPropertiesBinding(lightPtr);
		b->UpdateObjectProperties(&m_OldValue, 1);
		delete b;

		if (ObjectPropertiesEditor::Instance()->CurrentSerialNumber() == m_SerialNumber)
		{
			ObjectPropertiesEditor::Instance()->ReloadPropertyValue(m_OldValue.id);
		}
	}
}


CEditHistory::CEditHistory()
{
	m_lstActions.clear();	
	m_Position = -1;

}

CEditHistory::~CEditHistory()
{

}

void CEditHistory::PushAction(IEditAction* pAction)
{
#ifdef DEBUG_EDIT_HISTORY
	Con_Printf("PushAction()\n");
#endif 
	if (m_Position == -1)
		m_lstActions.clear();
	else
	{
		auto it = m_lstActions.begin();
		std::advance(it, m_Position + 1);

	 	while (it != m_lstActions.end())
 			m_lstActions.erase(it++);
	}

	m_lstActions.push_back(pAction);
	m_Position = m_lstActions.size() - 1;

#ifdef DEBUG_EDIT_HISTORY
	Con_Printf("Actions stack size: %d\n", m_lstActions.size());
#endif
}

void CEditHistory::Undo()
{

	if (m_Position == -1)
		return;

	auto it = m_lstActions.begin();	
	std::advance(it, m_Position);

	(*it)->Undo();
	m_Position--;

	Application::ScheduleCompilationIfNecceseary();
}

void CEditHistory::Redo()
{
	if (m_Position == m_lstActions.size() - 1)
		return;
	
	m_Position++;

	auto it = m_lstActions.begin();
	std::advance(it, m_Position);
	

	(*it)->Redo();
	
	Application::ScheduleCompilationIfNecceseary();
}

CDeleteLightAction::CDeleteLightAction(SceneEntityPtr pObject)
{
	m_Object = pObject;
	m_Object->SetSelected(false);
}

void CDeleteLightAction::Redo()
{
	auto scene = Application::GetMainWindow()->GetSceneRenderer()->GetScene();
	scene->DeleteEntityWithSerialNumber(m_Object->GetSerialNumber());
}

void CDeleteLightAction::Undo()
{
	auto scene = Application::GetMainWindow()->GetSceneRenderer()->GetScene();
	scene->AddEntityWithSerialNumber(m_Object, m_Object->GetSerialNumber());
}
