/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "edit_history.h"
#include <xutility>
#include "application.h"
#include "properties_editor.h"



CPropertyChangeAction::CPropertyChangeAction(size_t serialNumber, VariantValue oldValue, VariantValue* newValue)
{
	m_SerialNumber = serialNumber;
	m_OldValue = oldValue;
	m_NewValue = *newValue;
}


void CPropertyChangeAction::Redo()
{
    assert(false && "fixme");
    // 	auto sceneRender = Application::GetMainWindow()->GetSceneRenderer();
// 	auto scene = sceneRender->GetScene();
// 
// 	SceneEntityWeakPtr pLight = scene->GetEntityBySerialNumber(m_SerialNumber);
// 
// 	if (auto ptr = pLight.lock())
// 	{
// 		lightDefWPtr_t lightPtr = std::dynamic_pointer_cast<Lb3kLightEntity>(ptr);
// 
// 		auto b = new LightPropertiesBinding(lightPtr);
// 		b->UpdateObjectProperties(&m_NewValue, 1);
// 		delete b;
// 
// 		if (ObjectPropertiesEditor::Instance()->CurrentSerialNumber() == m_SerialNumber)
// 		{
// 			ObjectPropertiesEditor::Instance()->ReloadPropertyValue(m_OldValue.GetId());
// 		}
// 	}
}

void CPropertyChangeAction::Undo()
{
    assert(false && "fixme");
    // 	auto sceneRender = Application::GetMainWindow()->GetSceneRenderer();
// 	auto scene = sceneRender->GetScene();
// 
// 	SceneEntityWeakPtr pLight = scene->GetEntityBySerialNumber(m_SerialNumber);
// 
// 	if (auto ptr = pLight.lock())
// 	{
// 		lightDefWPtr_t lightPtr = std::dynamic_pointer_cast<Lb3kLightEntity>(ptr);
// 
// 		auto b = new LightPropertiesBinding(lightPtr);
// 		b->UpdateObjectProperties(&m_OldValue, 1);
// 		delete b;
// 
// 		if (ObjectPropertiesEditor::Instance()->CurrentSerialNumber() == m_SerialNumber)
// 		{
// 			ObjectPropertiesEditor::Instance()->ReloadPropertyValue(m_OldValue.GetId());
// 		}
// 	}
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
 	m_Object = new SceneEntity(*pObject);
 	m_Object->SetSelected(false);
 }

CDeleteLightAction::~CDeleteLightAction()
{
	if (m_Object)
		delete m_Object;
}

void CDeleteLightAction::Redo()
{
	auto scene = Application::GetMainWindow()->GetSceneRenderer()->GetScene();
	scene->DeleteEntityWithSerialNumber(m_Object->GetSerialNumber());
}

void CDeleteLightAction::Undo()
{
	auto scene = Application::GetMainWindow()->GetSceneRenderer()->GetScene();

	auto objPtr = std::make_shared<SceneEntity>(*m_Object);

	scene->AddEntityWithSerialNumber(objPtr, m_Object->GetSerialNumber());
}

EditTransaction::EditTransaction()
{    
}

EditTransaction::~EditTransaction()
{
    for (auto & it: m_lstActions)
		delete it;

	m_lstActions.clear();
}

void EditTransaction::Redo()
{
    for (auto &it : m_lstActions)
        it->Redo();
}

void EditTransaction::Undo()
{
    for (auto &it : m_lstActions)
        it->Undo();
}

bool EditTransaction::Empty()
{
    return m_lstActions.empty();
}
