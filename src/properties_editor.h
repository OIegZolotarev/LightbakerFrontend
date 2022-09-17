#pragma once
#include "object_props.h"

class ObjectPropertiesEditor
{
	ObjectPropertiesEditor();

	IObjectPropertiesBinding* m_pPropertiesBinding = nullptr;
	ImGuiID m_FlagsEditorID;

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
public:

	static ObjectPropertiesEditor* Instance();
	~ObjectPropertiesEditor();;

	void RenderEditor();
	void LoadObject(IObjectPropertiesBinding* pBindings);

	void RenderGuizmo();



};