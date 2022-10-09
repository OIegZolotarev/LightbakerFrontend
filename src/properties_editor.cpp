#include "ui_common.h"
#include "object_props.h"
#include "properties_editor.h"
#include "lights.h"
#include "application.h"
#include "Camera.h"

ObjectPropertiesEditor::ObjectPropertiesEditor(): IGUIPanel(PanelsId::ObjectProperties,(char*)"Object properties")
{
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
	{
		delete m_pPropertiesBinding;
		m_pPropertiesBinding = 0;
	}
}

void ObjectPropertiesEditor::RenderEditor()
{
	if (!m_FlagsEditorID)
		m_FlagsEditorID = ImGui::GetID("Flags");

	RenderPropetiesPane();
	RenderFlagsEditor();
}

void ObjectPropertiesEditor::LoadObject(IObjectPropertiesBinding* pBindings)
{
	if (m_pPropertiesBinding)
	{
		delete m_pPropertiesBinding;
		m_pPropertiesBinding = 0;
	}

	m_vPropsData.clear();
	m_pPropertiesBinding = pBindings;
	m_pPropertiesBinding->FillProperties(m_vPropsData);

	SetupGuizmo();
}

void ObjectPropertiesEditor::UpdateProperty(propsData_t* it)
{
	m_pPropertiesBinding->UpdateObjectProperties(it,1);
}

void ObjectPropertiesEditor::RenderGuizmo()
{
	if (!CheckObjectValidity()) return;

	if (!m_pGuizmoProperty) return;
		
	ImGuizmo::BeginFrame();
	Camera* cam = Application::Instance()->GetMainWindow()->GetSceneRenderer()->GetCamera();
		
	EditTransform(cam->GetViewMatrix(), cam->GetProjectionMatrix(), &m_matGuizmo[0][0], false);
}

void ObjectPropertiesEditor::UnloadObject()
{
	if (m_pPropertiesBinding)
	{
		delete m_pPropertiesBinding;
		m_pPropertiesBinding = nullptr;
		m_vPropsData.clear();
	}
}

DockPanels ObjectPropertiesEditor::GetDockSide()
{
	return DockPanels::RightTop;
}

void ObjectPropertiesEditor::Render()
{
	RenderEditor();
}

bool ObjectPropertiesEditor::CheckObjectValidity()
{
	if (!m_pPropertiesBinding)
		return false;

	if (!m_pPropertiesBinding->IsObjectValid())
	{
		m_vPropsData.clear();
		delete m_pPropertiesBinding;
		m_pPropertiesBinding = nullptr;
		return false;
	}

	return true;
}

void ObjectPropertiesEditor::SetupGuizmo()
{
	m_pGuizmoProperty = FindFirstPropertyByType(PropertiesTypes::Position);

	if (!m_pGuizmoProperty)
	{
		ImGuizmo::Enable(false);
		return;
	}

	ImGuizmo::Enable(true);
	m_matGuizmo = glm::translate(glm::mat4(1.f), m_pGuizmoProperty->GetPosition());

}

propsData_t* ObjectPropertiesEditor::FindFirstPropertyByType(PropertiesTypes type)
{
	for (auto& it : m_vPropsData)
	{
		if (it.type == type)
			return &it;
	}

	return nullptr;
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
	if (ImGui::Begin(m_strPanelTitle))
	{
		CheckObjectValidity();

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

		
	}
	ImGui::End();
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
						UpdateProperty(&it);
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
			UpdateProperty(&it);
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

void ObjectPropertiesEditor::EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition)
{
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
	static bool useSnap = false;
	static float snap[3] = { 1.f, 1.f, 1.f };
	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	static bool boundSizing = false;
	static bool boundSizingSnap = false;

	if (editTransformDecomposition)
	{
		if (ImGui::IsKeyPressed(90))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed(69))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed(82)) // r Key
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		ImGui::SameLine();
		if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		if (ImGui::RadioButton("Universal", mCurrentGizmoOperation == ImGuizmo::UNIVERSAL))
			mCurrentGizmoOperation = ImGuizmo::UNIVERSAL;
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
		ImGui::InputFloat3("Tr", matrixTranslation);
		ImGui::InputFloat3("Rt", matrixRotation);
		ImGui::InputFloat3("Sc", matrixScale);
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

		if (mCurrentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
				mCurrentGizmoMode = ImGuizmo::WORLD;
		}
		if (ImGui::IsKeyPressed(83))
			useSnap = !useSnap;
		ImGui::Checkbox("##UseSnap", &useSnap);
		ImGui::SameLine();

		switch (mCurrentGizmoOperation)
		{
		case ImGuizmo::TRANSLATE:
			ImGui::InputFloat3("Snap", &snap[0]);
			break;
		case ImGuizmo::ROTATE:
			ImGui::InputFloat("Angle Snap", &snap[0]);
			break;
		case ImGuizmo::SCALE:
			ImGui::InputFloat("Scale Snap", &snap[0]);
			break;
		}
		ImGui::Checkbox("Bound Sizing", &boundSizing);
		if (boundSizing)
		{
			ImGui::PushID(3);
			ImGui::Checkbox("##BoundSizing", &boundSizingSnap);
			ImGui::SameLine();
			ImGui::InputFloat3("Snap", boundsSnap);
			ImGui::PopID();
		}
	}

	ImGuiIO& io = ImGui::GetIO();
	float viewManipulateRight = io.DisplaySize.x;
	float viewManipulateTop = 0;

	auto win = Application::GetMainWindow()->Get3DGLViewport();
	auto h = Application::GetMainWindow()->Height();


	ImGuizmo::SetRect(win[0], h - win[1] - win[3], win[2], win[3]);	
	bool bChanged = ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

	if (bChanged)
	{
		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);

		m_pGuizmoProperty->value.asPosition.x = matrixTranslation[0];
		m_pGuizmoProperty->value.asPosition.y = matrixTranslation[1];
		m_pGuizmoProperty->value.asPosition.z = matrixTranslation[2];

		UpdateProperty(m_pGuizmoProperty);
	}
	
}

