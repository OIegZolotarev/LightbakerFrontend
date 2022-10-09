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
	propsData_t* m_pGuizmoProperty = nullptr;

	bool CheckObjectValidity();
	void SetupGuizmo();
	propsData_t* FindFirstPropertyByType(PropertiesTypes type);

	void RenderEditor();

public:

	static ObjectPropertiesEditor* Instance();
	~ObjectPropertiesEditor();;
		
	void LoadObject(IObjectPropertiesBinding* pBindings);
	void UnloadObject();
	
	void RenderGuizmo();
	
	DockPanels GetDockSide() override;
	void Render() override;

};