/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

#include "common.h"

class ISelectableObject
{
protected:
	bool	m_bSelected = false;
	bool	m_bHovered = false;
public:

	virtual void RenderForSelection(int objectId, class SceneRenderer*) = 0;
	virtual	void OnHovered() = 0;
	virtual void OnUnhovered() = 0;
	virtual	void OnMouseMove(glm::vec2 delta) = 0;
	virtual void OnSelect() = 0;
	virtual	void OnUnSelect() = 0;

	bool IsSelected();
	bool IsHovered();

	void InvokeSelect();
	void SetSelected(bool state);
	void SetHovered(bool state);
};

typedef std::weak_ptr<ISelectableObject> ISelectableObjectWeakRef;

class SelectionManager
{
	std::vector<ISelectableObjectWeakRef> m_vObjects;
	SelectionManager();

	bool m_bDoSelectionTests = false;
	glm::vec2 m_vecTestPoint;

	ISelectableObjectWeakRef m_pLastHoveredObject;
	ISelectableObjectWeakRef m_pLastSelectedObject;
    ISelectableObject *m_pSelectionInvokedObject;

	static bool m_bGizmoEnabled = false;	

public:

	static void EnableGizmo(bool status)
    {
        m_bGizmoEnabled = status;
    }

	static bool IsGizmoEnabled()    
	{
        return m_bGizmoEnabled;
    }

	static SelectionManager* Instance();

	void					  PushObject(ISelectableObjectWeakRef pObject);
	ISelectableObjectWeakRef  LastHoveredObject();

	void				NewFrame(class SceneRenderer*);
	void				SetTestsFlag(bool doTests, glm::vec2 point);

	~SelectionManager();
	bool SelectHoveredObject();
	void UnSelect();
	void UnSelectEverythingBut(ISelectableObject* param1);

};

extern bool DEBUG_3D_SELECTION;