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

public:

	static ObjectPropertiesEditor* Instance();
	~ObjectPropertiesEditor();;

	void RenderEditor();
	void LoadObject(IObjectPropertiesBinding* pBindings);
private:
	void UpdateProperty(propsData_t* it);
};