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

	propsData_t p = propsData_s(LightProperties::Type, PropertiesTypes::Enum, "Type");

	p.m_EnumOrFlagsValues.push_back(enumValuePair_t("Omni", (int)LightTypes::Omni));
	p.m_EnumOrFlagsValues.push_back(enumValuePair_t("Spot", (int)LightTypes::Spot));
	p.m_EnumOrFlagsValues.push_back(enumValuePair_t("Direct", (int)LightTypes::Direct));	
	p.SetEnumIndexFromValue((int)ptr->type);
	
	collection.push_back(p);

	p = propsData_s(LightProperties::Flags, PropertiesTypes::Flags, "Flags");
	p.m_EnumOrFlagsValues.push_back(enumValuePair_t("Euler",	LF_EULER));
	p.m_EnumOrFlagsValues.push_back(enumValuePair_t("Dyn",		LF_DYN));
	p.m_EnumOrFlagsValues.push_back(enumValuePair_t("XYZ",		LF_XYZ));
	p.m_EnumOrFlagsValues.push_back(enumValuePair_t("Linear",	LF_LINEAR));
	p.m_EnumOrFlagsValues.push_back(enumValuePair_t("Disk",		LF_DISK));
	p.m_EnumOrFlagsValues.push_back(enumValuePair_t("Rect",		LF_RECT));
	p.value.asFlags = ptr->flags;
	collection.push_back(p);

	p = propsData_s(LightProperties::Pos, PropertiesTypes::Position, "Pos");
	p.value.asPosition = ptr->pos;
	collection.push_back(p);

	p = propsData_s(LightProperties::Angles, PropertiesTypes::Angles, "Angles");
	p.value.asAngles = ptr->anglesDirection;
	collection.push_back(p);

	p = propsData_s(LightProperties::Color, PropertiesTypes::ColorRGB, "Color");
	p.value.asColorRGB = ptr->color;
	collection.push_back(p);

	p = propsData_s(LightProperties::Intensity, PropertiesTypes::Float, "Intensity");
	p.value.asFloat = ptr->intensity;
	collection.push_back(p);



	p = propsData_s(LightProperties::ConeA, PropertiesTypes::Float, "ConeA");
	p.value.asFloat = ptr->cones[0];
	collection.push_back(p);

	p = propsData_s(LightProperties::ConeB, PropertiesTypes::Float, "ConeB");
	p.value.asFloat = ptr->cones[1];
	collection.push_back(p);

	p = propsData_s(LightProperties::Size, PropertiesTypes::SizeX, "Size");
	p.value.asFloat = ptr->size.x;
	collection.push_back(p);

}

void LightPropertiesBinding::UpdateObjectProperties(propsData_t * props,size_t num)
{
	auto ptr = m_pwLightDef.lock();
	if (!ptr)
		return;

	for (size_t i = 0; i < num; i++)
	{
		auto it = props[i];

		switch (it.id)
		{
		case LightProperties::Type:
			ptr->type = (LightTypes)it.GetEnumValue();
			ptr->UpdateEditorIcon();
			break;
		case LightProperties::Flags:
			ptr->flags = it.GetFlags();
			break;
		case LightProperties::Pos:
			ptr->pos = it.GetPosition();
			break;
		case LightProperties::Color:
			ptr->color = it.GetColorRGB();
			break;
		case LightProperties::Intensity:
			ptr->intensity = it.GetFloat();
			break;
		case LightProperties::Angles:
			ptr->anglesDirection = it.GetAngles();
			break;
		case LightProperties::ConeA:
			ptr->cones[0] = it.GetFloat();
			break;
		case LightProperties::ConeB:
			ptr->cones[1] = it.GetFloat();
			break;
		case LightProperties::Size:
			ptr->size[0] = it.GetFloat();
			break;
		case LightProperties::Style:
			ptr->style = it.GetInt();
			break;
		}
	}
}

bool LightPropertiesBinding::IsObjectValid()
{
	return m_pwLightDef.lock() != nullptr;
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
		return "Omnidirectional";
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
