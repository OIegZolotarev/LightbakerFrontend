/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "ui_common.h"
#include "object_props.h"
#include "properties_editor.h"
#include "lights.h"
#include "application.h"
#include "camera.h"

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

void ObjectPropertiesEditor::UpdateProperty(VariantValue* it)
{
	m_pPropertiesBinding->UpdateObjectProperties(it,1);
}

void ObjectPropertiesEditor::RenderGuizmo()
{
	if (!CheckObjectValidity()) return;

	if (!m_pGuizmoPropertyPosition) return;
		
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

	SetupGuizmo();
}

DockPanels ObjectPropertiesEditor::GetDockSide()
{
	return DockPanels::RightTop;
}

void ObjectPropertiesEditor::Render()
{
	RenderEditor();
}

int ObjectPropertiesEditor::CurrentSerialNumber()
{
	if (!m_pPropertiesBinding)
		return -1;

	return m_pPropertiesBinding->GetSerialNumber();
}

void ObjectPropertiesEditor::ReloadPropertyValue(int id)
{
	m_vPropsData.clear();	
	m_pPropertiesBinding->FillProperties(m_vPropsData);

	SetupGuizmo();
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
	m_pGuizmoPropertyPosition = FindFirstPropertyByType(PropertiesTypes::Position);

	m_GizmoMode = 0;

	if (!m_pGuizmoPropertyPosition)
	{
		ImGuizmo::Enable(false);
		return;
	}
	else
		m_GizmoMode = ImGuizmo::TRANSLATE;

	m_pGuizmoPropertyRotation = FindFirstPropertyByType(PropertiesTypes::Angles);

	if (m_pGuizmoPropertyRotation)	
		m_GizmoMode |= ImGuizmo::ROTATE;
	
	ImGuizmo::Enable(true);
	

	glm::vec3 pos = m_pGuizmoPropertyPosition->GetPosition();
	glm::vec3 ang = m_pGuizmoPropertyRotation->GetAngles();
	glm::vec3 scale = glm::vec3(1.f);

	ImGuizmo::RecomposeMatrixFromComponents(&pos.x, &ang.x, &scale.x, &m_matGuizmo[0][0]);

}

VariantValue* ObjectPropertiesEditor::FindFirstPropertyByType(PropertiesTypes type)
{
	for (auto& it : m_vPropsData)
	{
		if (it.GetType() == type)
			return &it;
	}

	return nullptr;
}

void ObjectPropertiesEditor::RenderFlagsEditor()
{
	if (ImGui::BeginPopupModal("Flags", 0, ImGuiWindowFlags_AlwaysAutoResize))
	{
		assert(m_pCurrentFlagProp);

		for (auto& it : m_pCurrentFlagProp->GetEnumValues())
		{
			if (ImGui::CheckboxFlags(it.first.c_str(), (int*)m_pCurrentFlagProp->Data(), it.second))
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

				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::SeparatorText(m_pPropertiesBinding->ObjectClassname());

				for (auto& it : m_vPropsData)
				{
					ImGui::PushID(i++); // Use field index as identifier.

					// Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::AlignTextToFramePadding();
					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
					ImGui::TreeNodeEx("Field", flags, "%s", it.DisplayName());

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

void ObjectPropertiesEditor::RenderPropertyControl(VariantValue& it)
{
	VariantValue oldValue = it;

	switch (it.GetType())
	{
	case PropertiesTypes::Enum:

		

		if (it.GetEnumValues().size() > 0)
		{
			int selectionIndex = it.GetInt();
			it.SetTempLabel((char*)it.GetEnumValues()[selectionIndex].first.c_str());

			if (ImGui::BeginCombo(it.DisplayName(), it.TempLabel()))
			{
				int i = 0;

				for (auto& enumValue : it.GetEnumValues())
				{
					if (ImGui::Selectable(enumValue.first.c_str(), i == selectionIndex))
					{
						it.SetInt(i);
						it.SetTempLabel((char*)enumValue.first.c_str());
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

		for (auto flagValue : it.GetEnumValues())
		{ 
			if (it.GetFlags() & flagValue.second)
				flagDescription += flagValue.first + ",";
		}

		if (flagDescription.length() > 1)
			flagDescription[flagDescription.length() - 1] = ']';
		else
			flagDescription += "<None set>]";

		ImGui::Text("%s", flagDescription.c_str());

	}
		break;
	case PropertiesTypes::Position:		
		if (ImGui::DragFloat3("Vector", (float*)it.Data()))
		{
			SetupGuizmo();
			UpdateProperty(&it);
		}
		break;
	case PropertiesTypes::ColorRGB:
		if (ImGui::ColorEdit3("##value", (float*)it.Data()))
			UpdateProperty(&it);
		break;
	case PropertiesTypes::ColorRGBA:
		if (ImGui::ColorEdit4("##value", (float*)it.Data()))
			UpdateProperty(&it);
		break;
	case PropertiesTypes::Float:
		if (ImGui::DragFloat("##value", (float*)it.Data(), 0.01f))
			UpdateProperty(&it);
		break;
	case PropertiesTypes::Angles:
		if (ImGui::DragFloat3("Vector", (float*)it.Data()))
			UpdateProperty(&it);
		break;
	case PropertiesTypes::Int:
		if (ImGui::DragInt("##value", (int*)it.Data()))
			UpdateProperty(&it);
		break;
	case PropertiesTypes::SizeX:
		if (ImGui::DragFloat("Vector", (float*)it.Data()))
			UpdateProperty(&it);
		break;
	default:
		break;

	}

	if (ImGui::IsItemActivated())
	{
		// Here we record old value when item is actived for the first time.
		m_OldPropertyValue = oldValue;
	}

	if (ImGui::IsItemDeactivatedAfterEdit())
	{
		// Here we can save undo (to undo stack or something). Triggered once just after user stops editing. 
		// RecordUndo(oldValue, value);

		auto history = Application::GetMainWindow()->GetSceneRenderer()->GetScene()->GetEditHistory();
		history->PushAction(new CPropertyChangeAction(m_pPropertiesBinding->GetSerialNumber(), m_OldPropertyValue, it));

		m_pPropertiesBinding->OnPropertyChangeSavedToHistory();
	}
}

void ObjectPropertiesEditor::EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition)
{
	ImGuizmo::OPERATION mCurrentGizmoOperation = m_pPropertiesBinding->GetMeaningfulGizmoOperationMode();

	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
	static bool useSnap = false;
	static float snap[3] = { 1.f, 1.f, 1.f };
	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	static bool boundSizing = false;
	static bool boundSizingSnap = false;

	if (editTransformDecomposition)
	{
		if (ImGui::IsKeyPressed(ImGuiKey_T))
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed(ImGuiKey_R))
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed(ImGuiKey_Y)) // r Key
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
		if (ImGui::IsKeyPressed(ImGuiKey_U))
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


	//ImGuizmo::SetGizmoSizeClipSpace(0.5f);
	ImGuizmo::AllowAxisFlip(false);

	ImGuizmo::SetRect(win[0], h - win[1] - win[3], win[2], win[3]);	
	bool bChanged = ImGuizmo::Manipulate(cameraView, cameraProjection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

	if (bChanged)
	{
		

		

		float matrixTranslation[3], matrixRotation[3], matrixScale[3];
		ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);


		m_pGuizmoPropertyPosition->SetPosition(*(glm::vec3*)matrixTranslation);

		if (m_pGuizmoPropertyRotation)
		{
			auto angleMod = [](float f) -> float { if (f < 0) return f + 360; else if (f > 360) return f - 360; return f; };


			for(int i = 0 ; i < 3; i++)
				matrixRotation[i] = angleMod(matrixRotation[i]);

			m_pGuizmoPropertyRotation->SetAngles(*(glm::vec3*)matrixRotation);

			UpdateProperty(m_pGuizmoPropertyRotation);
		}

		UpdateProperty(m_pGuizmoPropertyPosition);
	}

	if (!m_bGuizmoEdited && ImGuizmo::IsUsing())
	{
		// Here we record old value when item is actived for the first time.
		m_OldPropertyValue = *m_pGuizmoPropertyPosition;
		if (m_pGuizmoPropertyRotation)
			m_OldPropertyValue2 = *m_pGuizmoPropertyRotation;
		m_bGuizmoEdited = true;
	}

	if (!ImGuizmo::IsUsing() && m_bGuizmoEdited)
	{
		float delta1 = glm::length(m_OldPropertyValue.GetPosition() - m_pGuizmoPropertyPosition->GetPosition());
		float delta2 = glm::length(m_OldPropertyValue2.GetAngles() - m_pGuizmoPropertyRotation->GetAngles());

		if (delta1 < glm::epsilon<float>() && delta2 < glm::epsilon<float>())
			return;

		m_bGuizmoEdited = false;

		auto history = Application::GetMainWindow()->GetSceneRenderer()->GetScene()->GetEditHistory();
		history->PushAction(new CPropertyChangeAction(m_pPropertiesBinding->GetSerialNumber(), m_OldPropertyValue, *m_pGuizmoPropertyPosition));

		if (m_pGuizmoPropertyRotation)
			history->PushAction(new CPropertyChangeAction(m_pPropertiesBinding->GetSerialNumber(), m_OldPropertyValue2, *m_pGuizmoPropertyRotation));
		

		m_pPropertiesBinding->OnPropertyChangeSavedToHistory();



	}
	
}

