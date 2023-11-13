/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "common.h"
#include "selection_3d.h"
#include "application.h"

SelectionManager* SelectionManager::Instance()
{
	static SelectionManager* sInstance = new SelectionManager;
	return sInstance;
}

void SelectionManager::PushObject(ISelectableObjectWeakRef pObject)
{
	m_vObjects.push_back(pObject);
}

ISelectableObjectWeakRef SelectionManager::LastHoveredObject()
{
	return m_pLastHoveredObject;
}

void SelectionManager::NewFrame(SceneRenderer* pRenderer)
{
	// Con_Printf("%d : %f\n", ImGuizmo::IsOver(), Application::GetMainWindow()->FrameDelta());

	if (ImGuizmo::IsOver() && ImGuizmo::IsEnabled())
	{
		m_pLastHoveredObject.reset();
		Application::GetMainWindow()->ClearBackground();
		return;
	}


	glClearColor(0, 0, 0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

	//if (m_bDoSelectionTests)
	{

		for (size_t i = 0; i < m_vObjects.size(); i++)
		{
			int objectId = 1 + i * 16;

			glColor4ubv((byte*)&objectId);

			if (auto ptr = m_vObjects[i].lock())
			{
				ptr->RenderForSelection(objectId, pRenderer);

				if (m_pSelectionInvokedObject)
				{
					if (ptr.get() != m_pSelectionInvokedObject)
						ptr->SetSelected(false);
				}

			}
		}

		if (m_vObjects.size() > 0)
			glFinish();

		m_pSelectionInvokedObject = nullptr;

		int mx, my;

		SDL_GetMouseState(&mx, &my);
		int* viewport	= Application::GetMainWindow()->Get3DGLViewport();
		int h			= Application::GetMainWindow()->Height();

		mx -= viewport[0];
		my = h - my;

		//my = viewport[2] - my;

		byte pixel[4] = { 0 };

		glReadPixels(mx, my, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);

		int obj_id = *(int*)pixel;

		if (obj_id != 0)
		{
			obj_id -= 1;
			obj_id /= 16;

			for (size_t i = 0; i < m_vObjects.size(); i++)
			{
				auto ptr = m_vObjects[i].lock();
				if (!ptr)
					continue;

				if (i == obj_id)
				{
					m_pLastHoveredObject = m_vObjects[i];
					ptr->OnHovered();
					ptr->SetHovered(true);
				}
				else
				{
					ptr->OnUnhovered();					
					ptr->SetHovered(false);
				}
			}
		}



	}

	glEnable(GL_TEXTURE_2D);

	m_vObjects.clear();
	m_bDoSelectionTests = false;

	if (!DEBUG_3D_SELECTION)
	{
		Application::GetMainWindow()->ClearBackground();
	}
}

void SelectionManager::SetTestsFlag(bool doTests, glm::vec2 testPoint)
{
	m_bDoSelectionTests = doTests;
	m_vecTestPoint = testPoint;
}

SelectionManager::~SelectionManager()
{
	m_vObjects.clear();
	m_vObjects.shrink_to_fit();
}

bool SelectionManager::SelectHoveredObject()
{

	if (auto ptr = m_pLastHoveredObject.lock())
	{
		for (size_t i = 0; i < m_vObjects.size(); i++)
		{
			if (auto ptr_other = m_vObjects[i].lock())
			{
				ptr_other->SetSelected(false);
				ptr->OnUnSelect();
			}
		}

		if (ptr->IsHovered())
		{
			ptr->SetSelected(true);
			ptr->OnSelect();

			m_pLastSelectedObject = m_pLastHoveredObject;

			return true;
		}

		return false;
		
	}

	return false;
}

void SelectionManager::UnSelect()
{
	if (auto ptr = m_pLastSelectedObject.lock())
	{
		ptr->SetSelected(false);
		ptr->SetHovered(false);
		ptr->OnUnSelect();

		m_pLastSelectedObject.reset();
	}

	

// 	for (auto& it : m_vObjects)
// 	{
// 		if (auto ptr = it.lock())
// 			ptr->m_bSelected = false;
// 	}
}

void SelectionManager::UnSelectEverythingBut(ISelectableObject* object)
{
	m_pSelectionInvokedObject = object;

	auto scene = Application::GetMainWindow()->GetSceneRenderer()->GetScene();

	
	// ƒостаточно безопасно кидать без проверки типов...
	m_pLastSelectedObject = scene->GetEntityWeakRef((SceneEntity*)object);
}

SelectionManager::SelectionManager()
{

}


bool ISelectableObject::IsSelected()
{
	return m_bSelected;
}

bool ISelectableObject::IsHovered()
{
	return m_bHovered;
}

void ISelectableObject::InvokeSelect()
{
	m_bSelected = true;
	OnSelect();

	SelectionManager::Instance()->UnSelectEverythingBut(this);
}

void ISelectableObject::SetSelected(bool state)
{
	m_bSelected = state;
}

void ISelectableObject::SetHovered(bool state)
{
	m_bHovered = state;
}

