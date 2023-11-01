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

	std::vector<VariantValue> m_vPropsData;

	VariantValue* m_pCurrentFlagProp = nullptr;

	void RenderFlagsEditor();
	void RenderPropetiesPane();

	void RenderPropertyControl(VariantValue& it);

	void EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition);
	void UpdateProperty(VariantValue* it);

	glm::mat4 m_matGuizmo = glm::mat4(1);
	VariantValue* m_pGuizmoPropertyPosition = nullptr;
	VariantValue* m_pGuizmoPropertyRotation = nullptr;

	bool m_bGuizmoEdited = false;

	bool CheckObjectValidity();
	void SetupGuizmo();
	VariantValue* FindFirstPropertyByType(PropertiesTypes type);

	void RenderEditor();

	VariantValue m_OldPropertyValue;
	VariantValue m_OldPropertyValue2;

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