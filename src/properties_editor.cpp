#include "ui_common.h"
#include "object_props.h"
#include "properties_editor.h"
#include "lights.h"

ObjectPropertiesEditor::ObjectPropertiesEditor()
{
	m_FlagsEditorID = ImGui::GetID("Flags");

	//LightPropertiesBinding b;
	//b.FillProperties(m_vPropsData);
}

ObjectPropertiesEditor* ObjectPropertiesEditor::Instance()
{
	static ObjectPropertiesEditor* sInstance = new ObjectPropertiesEditor;
	return sInstance;
}

ObjectPropertiesEditor::~ObjectPropertiesEditor()
{
	if (m_pPropertiesBinding)
		delete m_pPropertiesBinding;
}

void ObjectPropertiesEditor::RenderEditor()
{
	RenderPropetiesPane();
	RenderFlagsEditor();
}

void ObjectPropertiesEditor::LoadObject(IObjectPropertiesBinding* pBindings)
{
	if (m_pPropertiesBinding)
		delete m_pPropertiesBinding;
}

void ObjectPropertiesEditor::UpdateProperty(propsData_t* it)
{
	
}

void ObjectPropertiesEditor::RenderFlagsEditor()
{
	if (ImGui::BeginPopupModal("Flags", 0, ImGuiWindowFlags_AlwaysAutoResize))
	{
		assert(m_pCurrentFlagProp);

		for (auto& it : m_pCurrentFlagProp->m_EnumOrFlagsValues)
		{
			if (ImGui::CheckboxFlags(it.first.c_str(), &m_pCurrentFlagProp->value.asFlags, it.second))
				UpdateProperty(m_pCurrentFlagProp);
		}

		ImGui::SetNextItemWidth(100);
		if (ImGui::Button("OK"))
		{
			ImGui::CloseCurrentPopup();
		}


		ImGui::EndPopup();
	}
}

void ObjectPropertiesEditor::RenderPropetiesPane()
{
	if (ImGui::Begin("Properties"))
	{
		if (m_vPropsData.size() > 0)
		{
			
			
			if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
			{
				int i = 0;

				for (auto& it : m_vPropsData)
				{
					ImGui::PushID(i++); // Use field index as identifier.

					// Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::AlignTextToFramePadding();
					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
					ImGui::TreeNodeEx("Field", flags, "%s", it.display_name.c_str());

					ImGui::TableSetColumnIndex(1);
					ImGui::SetNextItemWidth(-FLT_MIN);

					RenderPropertyControl(it);

					ImGui::PopID();
				}

				ImGui::EndTable();
			}
		}
		else
		{
			if (!m_pPropertiesBinding)
				ImGui::Text("No object selected");
			else
				ImGui::Text("Object has no properties");
		}

		ImGui::End();
	}
}

void ObjectPropertiesEditor::RenderPropertyControl(propsData_t& it)
{
	switch (it.type)
	{
	case PropertiesTypes::Enum:

		if (it.m_EnumOrFlagsValues.size() > 0)
		{
			int selectionIndex = it.value.asInt;
			it.m_TempLabel = (char*)it.m_EnumOrFlagsValues[selectionIndex].first.c_str();

			if (ImGui::BeginCombo(it.display_name.c_str(), it.m_TempLabel))
			{
				int i = 0;

				for (auto& enumValue : it.m_EnumOrFlagsValues)
				{
					if (ImGui::Selectable(enumValue.first.c_str(), i == selectionIndex))
					{
						it.value.asInt = i;
						it.m_TempLabel = (char*)enumValue.first.c_str();
						UpdateProperty(it);
					}

					i++;
				}

				ImGui::EndCombo();
			}
		}
		else
		{
			ImGui::Text("No values defined!");
		}

		break;
	case PropertiesTypes::Flags:
	{
		if (ImGui::Button("..."))
		{
			ImGui::OpenPopup(m_FlagsEditorID);
			m_pCurrentFlagProp = &it;
		}

		ImGui::SameLine();

		std::string flagDescription = "[";

		for (auto flagValue : it.m_EnumOrFlagsValues)
		{ 
			if (it.value.asFlags & flagValue.second)
				flagDescription += flagValue.first + ",";
		}

		if (flagDescription.length() > 1)
			flagDescription[flagDescription.length() - 1] = ']';
		else
			flagDescription += "0]";

		ImGui::Text("%s", flagDescription.c_str());

	}
		break;
	case PropertiesTypes::Position:		
		if (ImGui::DragFloat3("Vector", &it.value.asPosition.x))
			UpdateProperty(&it);
		break;
	case PropertiesTypes::ColorRGB:
		if (ImGui::ColorEdit3("##value", &it.value.asColorRGB.x))
			UpdateProperty(&it);
		break;
	case PropertiesTypes::ColorRGBA:
		if (ImGui::ColorEdit4("##value", &it.value.asColorRGB.x))
			UpdateProperty(it);
		break;
	case PropertiesTypes::Float:
		if (ImGui::DragFloat("##value", &it.value.asFloat, 0.01f))
			UpdateProperty(&it);
		break;
	case PropertiesTypes::Angles:
		if (ImGui::DragFloat3("Vector", &it.value.asPosition.x))
			UpdateProperty(&it);
		break;
	case PropertiesTypes::Int:
		if (ImGui::DragInt("##value", &it.value.asInt, 0.01f))
			UpdateProperty(&it);
		break;
	case PropertiesTypes::SizeX:
		if (ImGui::DragFloat("Vector", &it.value.asPosition.x))
			UpdateProperty(&it);
		break;
	default:
		break;

	}
}
