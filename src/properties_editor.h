/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once
#include "object_props.h"
#include "igui_panel.h"

class ObjectPropertiesEditor : public IGUIPanel
{
	ObjectPropertiesEditor();

	IObjectPropertiesBinding* m_pPropertiesBinding = nullptr;
	ImGuiID m_FlagsEditorID = 0;

	std::vector<propsData_t> m_vPropsData;

	propsData_t* m_pCurrentFlagProp = nullptr;

	void RenderFlagsEditor();
	void RenderPropetiesPane();

	void RenderPropertyControl(propsData_t& it);

	void EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition);
	void UpdateProperty(propsData_t* it);

	glm::mat4 m_matGuizmo = glm::mat4(1);
	propsData_t* m_pGuizmoPropertyPosition = nullptr;
	propsData_t* m_pGuizmoPropertyRotation = nullptr;

	bool m_bGuizmoEdited = false;

	bool CheckObjectValidity();
	void SetupGuizmo();
	propsData_t* FindFirstPropertyByType(PropertiesTypes type);

	void RenderEditor();

	propsData_t m_OldPropertyValue;
	propsData_t m_OldPropertyValue2;

public:

	static ObjectPropertiesEditor* Instance();
	~ObjectPropertiesEditor();;
		
	void LoadObject(IObjectPropertiesBinding* pBindings);
	void UnloadObject();
	
	void RenderGuizmo();
	
	DockPanels GetDockSide() override;
	void Render() override;

	int CurrentSerialNumber();
	void ReloadPropertyValue(int id);
private:
	int m_GizmoMode;
};