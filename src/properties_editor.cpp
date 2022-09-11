#include "ui_common.h"
#include "object_props.h"
#include "properties_editor.h"
#include "lights.h"



void RenderPropertiesEditor()
{
	LightPropertiesBinding b;

	static std::vector<propsData_t> props;

	if (!props.size())
		b.FillProperties(props);

	if (ImGui::Begin("Properties"))
	{
		if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
		{			
			int i = 0;

			for (auto& it : props)
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

				switch (it.type)
				{
				case PropertiesTypes::Enum:

					if (it.m_EnumValues.size() > 0)
					{
						int selectionIndex = it.value.asInt;
						it.m_TempLabel = (char*)it.m_EnumValues[selectionIndex].first.c_str();

						if (ImGui::BeginCombo(it.display_name.c_str(), it.m_TempLabel))
						{
							int i = 0;

							for (auto& enumValue : it.m_EnumValues)
							{		
								if (ImGui::Selectable(enumValue.first.c_str(), i == selectionIndex))
								{
									it.value.asInt = i;
									it.m_TempLabel = (char*)enumValue.first.c_str();
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
					
					ImGui::Text("%d", it.value.asInt);
					ImGui::SameLine();
					
					if (ImGui::Button("..."))
					{
						// Show modal...
					}

					break;
				case PropertiesTypes::Position:
					//ImGui::Text("[%.3f;%.3f;%.3f]", it.value.asPosition.x, it.value.asPosition.y, it.value.asPosition.z);
					ImGui::DragFloat3("Vector", &it.value.asPosition.x);
					break;
				case PropertiesTypes::ColorRGB:
					ImGui::ColorEdit3("##value", &it.value.asColorRGB.x);
					break;
				case PropertiesTypes::ColorRGBA:
					ImGui::ColorEdit4("##value", &it.value.asColorRGB.x);
					break;
				case PropertiesTypes::Float:
					ImGui::DragFloat("##value", &it.value.asFloat, 0.01f);
					break;
				case PropertiesTypes::Angles:
					ImGui::DragFloat3("Vector", &it.value.asPosition.x);
					break;
				case PropertiesTypes::Int:
					ImGui::DragInt("##value", &it.value.asInt, 0.01f);
					break;
				case PropertiesTypes::SizeX:
					ImGui::DragFloat("Vector", &it.value.asPosition.x);
					break;
				default:
					break;

				}

				

				ImGui::PopID();
			}

			ImGui::EndTable();
		}

		ImGui::End();
	}
}
