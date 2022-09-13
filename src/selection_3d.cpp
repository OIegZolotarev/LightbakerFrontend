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

ISelectableObjectWeakRef SelectionManager::LastSelectedObject()
{
	return m_pLastSelectedObject;
}

void SelectionManager::NewFrame(SceneRenderer* pRenderer)
{
	glClearColor(0, 0, 0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	//if (m_bDoSelectionTests)
	{

		for (size_t i = 0; i < m_vObjects.size(); i++)
		{
			int objectId = 1 + i * 16;

			glColor4ubv((byte*)&objectId);

			if (auto ptr = m_vObjects[i].lock())
			{
				ptr->RenderForSelection(objectId, pRenderer);
			}
		}

		if (m_vObjects.size() > 0)
			glFinish();

		int mx, my;

		SDL_GetMouseState(&mx, &my);
		int* viewport	= Application::GetMainWindow()->GetGLViewport();
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
					m_pLastSelectedObject = m_vObjects[i];
					ptr->OnHovered();
					ptr->m_bHovered = true;
				}
				else
				{
					ptr->OnUnhovered();
					ptr->m_bSelected = false;
					ptr->m_bHovered = false;
				}
			}
		}



	}

	glEnable(GL_TEXTURE_2D);

	m_vObjects.clear();
	m_bDoSelectionTests = false;


	glClearColor(0.25, .25, .25, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	if (auto ptr = m_pLastSelectedObject.lock())
	{
		ptr->m_bSelected = true;
		ptr->OnSelect();
		return true;
	}

	return false;
}

SelectionManager::SelectionManager()
{

}


