/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "common.h"
#include "lights.h"
#include "scene_renderer.h"
#include "application.h"
#include "object_props.h"
#include "properties_editor.h"
#include "common_resources.h"


LightPropertiesBinding::LightPropertiesBinding(lightDefWPtr_t ref)
{
	m_pwLightDef = ref;
}

void LightPropertiesBinding::FillProperties(std::vector<propsData_t>& collection)
{
	auto ptr = m_pwLightDef.lock();

	if (!ptr)
		return;

	propsData_t p;
	
	MakeLightTypeProperty(p, ptr);
	collection.push_back(p);

	MakeLightFlagsProperty(p, ptr);
	collection.push_back(p);

	MakeLightPosProperty(p, ptr);
	collection.push_back(p);

	MakeLightAnglesProperty(p, ptr);
	collection.push_back(p);

	MakeLightColorProperty(p, ptr);
	collection.push_back(p);

	MakeLightIntensityProperty(p, ptr);
	collection.push_back(p);

	MakeConeAProperty(p, ptr);
	collection.push_back(p);

	MakeConeBProperty(p, ptr);
	collection.push_back(p);

	MakeSizeProperty(p, ptr);
	collection.push_back(p);

	MakeLightStyleProperty(p, ptr);
	collection.push_back(p);

}

void LightPropertiesBinding::MakeLightStyleProperty(propsData_t& p, std::shared_ptr<lightDef_t> ptr)
{
	p = propsData_s(LightProperties::Style, PropertiesTypes::Int, "Style");
	p.value.asInt = ptr->style;
}

void LightPropertiesBinding::MakeSizeProperty(propsData_t& p, std::shared_ptr<lightDef_t> ptr)
{
	p = propsData_s(LightProperties::Size, PropertiesTypes::SizeX, "Size");
	p.value.asFloat = ptr->size.x;
}

void LightPropertiesBinding::MakeConeBProperty(propsData_t& p, std::shared_ptr<lightDef_t> ptr)
{
	p = propsData_s(LightProperties::ConeB, PropertiesTypes::Float, "ConeB");
	p.value.asFloat = ptr->cones[1];
}

void LightPropertiesBinding::MakeConeAProperty(propsData_t& p, std::shared_ptr<lightDef_t> ptr)
{
	p = propsData_s(LightProperties::ConeA, PropertiesTypes::Float, "ConeA");
	p.value.asFloat = ptr->cones[0];
}

void LightPropertiesBinding::MakeLightIntensityProperty(propsData_t& p, std::shared_ptr<lightDef_t> ptr)
{
	p = propsData_s(LightProperties::Intensity, PropertiesTypes::Float, "Intensity");
	p.value.asFloat = ptr->intensity;
}

void LightPropertiesBinding::MakeLightColorProperty(propsData_t& p, std::shared_ptr<lightDef_t> ptr)
{
	p = propsData_s(LightProperties::Color, PropertiesTypes::ColorRGB, "Color");
	p.value.asColorRGB = ptr->color;
}

void LightPropertiesBinding::MakeLightAnglesProperty(propsData_t& p, std::shared_ptr<lightDef_t> ptr)
{
	p = propsData_s(LightProperties::Angles, PropertiesTypes::Angles, "Angles");
	p.value.asAngles = ptr->anglesDirection;
}

void LightPropertiesBinding::MakeLightPosProperty(propsData_t& p, std::shared_ptr<lightDef_t> ptr)
{
	p = propsData_s(LightProperties::Pos, PropertiesTypes::Position, "Pos");
	p.value.asPosition = ptr->pos;
}

void LightPropertiesBinding::MakeLightFlagsProperty(propsData_t& p, std::shared_ptr<lightDef_t> ptr)
{
	p = propsData_s(LightProperties::Flags, PropertiesTypes::Flags, "Flags");
	p.m_EnumOrFlagsValues.push_back(enumValuePair_t("Euler", LF_EULER));
	p.m_EnumOrFlagsValues.push_back(enumValuePair_t("Dyn", LF_DYN));
	p.m_EnumOrFlagsValues.push_back(enumValuePair_t("XYZ", LF_XYZ));
	p.m_EnumOrFlagsValues.push_back(enumValuePair_t("Linear", LF_LINEAR));
	p.m_EnumOrFlagsValues.push_back(enumValuePair_t("Disk", LF_DISK));
	p.m_EnumOrFlagsValues.push_back(enumValuePair_t("Rect", LF_RECT));
	p.value.asFlags = ptr->flags;
}

void LightPropertiesBinding::MakeLightTypeProperty(propsData_t& p, std::shared_ptr<lightDef_t> ptr)
{
	p = propsData_s(LightProperties::Type, PropertiesTypes::Enum, "Type");

	p.m_EnumOrFlagsValues.push_back(enumValuePair_t("Omni", (int)LightTypes::Omni));
	p.m_EnumOrFlagsValues.push_back(enumValuePair_t("Spot", (int)LightTypes::Spot));
	p.m_EnumOrFlagsValues.push_back(enumValuePair_t("Direct", (int)LightTypes::Direct));
	p.SetEnumIndexFromValue((int)ptr->type);
}

void LightPropertiesBinding::UpdateObjectProperties(propsData_t* props, size_t num)
{
	auto ptr = m_pwLightDef.lock();
	if (!ptr)
		return;

	auto history = Application::GetMainWindow()->GetSceneRenderer()->GetEditHistory();

	for (size_t i = 0; i < num; i++)
	{
		auto it = props[i];

		propsData_t oldValue;

		switch (it.id)
		{
		case LightProperties::Type:
			MakeLightTypeProperty(oldValue, ptr);
			ptr->type = (LightTypes)it.GetEnumValue();
			ptr->UpdateEditorIcon();
			break;
		case LightProperties::Flags:
			MakeLightFlagsProperty(oldValue, ptr);
			ptr->flags = it.GetFlags();
			break;
		case LightProperties::Pos:
			MakeLightPosProperty(oldValue, ptr);
			ptr->pos = it.GetPosition();
			break;
		case LightProperties::Color:
			MakeLightColorProperty(oldValue, ptr);
			ptr->color = it.GetColorRGB();
			break;
		case LightProperties::Intensity:
			MakeLightIntensityProperty(oldValue, ptr);
			ptr->intensity = it.GetFloat();
			break;
		case LightProperties::Angles:
			MakeLightAnglesProperty(oldValue, ptr);
			ptr->anglesDirection = it.GetAngles();
			break;
		case LightProperties::ConeA:
			MakeConeAProperty(oldValue, ptr);
			ptr->cones[0] = it.GetFloat();
			break;
		case LightProperties::ConeB:
			MakeConeBProperty(oldValue, ptr);
			ptr->cones[1] = it.GetFloat();
			break;
		case LightProperties::Size:
			MakeSizeProperty(oldValue, ptr);
			ptr->size[0] = it.GetFloat();
			break;
		case LightProperties::Style:
			MakeLightStyleProperty(oldValue, ptr);
			ptr->style = it.GetInt();
			break;
		}
	}

}

bool LightPropertiesBinding::IsObjectValid()
{
	return m_pwLightDef.lock() != nullptr;
}

int LightPropertiesBinding::GetSerialNumber()
{
	auto ptr = m_pwLightDef.lock();

	if (!ptr) return 0;

	return ptr->serial_number;
}

void LightPropertiesBinding::OnPropertyChangeSavedToHistory()
{
	Application::ScheduleCompilationIfNecceseary();
}

ImGuizmo::OPERATION LightPropertiesBinding::GetMeaningfulGizmoOperationMode()
{
	auto ptr = m_pwLightDef.lock();

	if (!ptr)
		return (ImGuizmo::OPERATION)0;

	switch (ptr->type)
	{
	case LightTypes::Spot:		
		return ImGuizmo::TRANSLATE | ImGuizmo::ROTATE_X | ImGuizmo::ROTATE_Y;
	case LightTypes::Direct:
		return ImGuizmo::TRANSLATE | ImGuizmo::ROTATE_X | ImGuizmo::ROTATE_Y;
	case LightTypes::Omni:
		return ImGuizmo::TRANSLATE;
	}

	return (ImGuizmo::OPERATION)0;

}

void lightDef_s::UpdateEditorIcon()
{
	switch (type)
	{
	case LightTypes::Omni:
		editor_icon = GetCommonIcon(CommonIcons::OmniLight);
		break;
	case LightTypes::Spot:
		editor_icon = GetCommonIcon(CommonIcons::SpotLight);
		break;
	case LightTypes::Direct:
		editor_icon = GetCommonIcon(CommonIcons::DirectLight);
		break;
	default:
		break;

	}
}

void lightDef_s::OnHovered()
{

}

void lightDef_s::OnMouseMove(glm::vec2 delta)
{
	
}

void lightDef_s::OnUnhovered()
{
	
}

void lightDef_s::RenderForSelection(int objectId,SceneRenderer * pRenderer)
{	
	pRenderer->DrawBillboardSelection(pos, glm::vec2(4, 4), editor_icon, objectId);
}

void lightDef_s::OnSelect()
{
	auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();

	auto weakRef = sceneRenderer->GetLightWeakRef(this);

	sceneRenderer->HintSelected(weakRef);

	LightPropertiesBinding* pBinding = new LightPropertiesBinding(weakRef);
	ObjectPropertiesEditor::Instance()->LoadObject(pBinding);
}

void lightDef_s::OnUnSelect()
{
	ObjectPropertiesEditor::Instance()->UnloadObject();
}

const char* lightDef_s::Description()
{
	switch (type)
	{
	case LightTypes::Omni:
		return "Omni directional";
		break;
	case LightTypes::Spot:
		return "Spotlight";
		break;
	case LightTypes::Direct:
		return "Direct";
		break;
	default:
		break;
		
	}

	return "Bad";
}
