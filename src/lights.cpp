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

void LightPropertiesBinding::FillProperties(std::vector<VariantValue>& collection)
{
	auto ptr = m_pwLightDef.lock();

	if (!ptr)
		return;

	VariantValue p;
	
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

void LightPropertiesBinding::MakeLightStyleProperty(VariantValue& p, std::shared_ptr<Lb3kLightEntity> ptr)
{
	p = VariantValue(LightProperties::Style, PropertiesTypes::Int, "Style");
	p.SetInt(ptr->style);
}

void LightPropertiesBinding::MakeSizeProperty(VariantValue& p, std::shared_ptr<Lb3kLightEntity> ptr)
{
	p = VariantValue(LightProperties::Size, PropertiesTypes::SizeX, "Size");
	p.SetFloat(ptr->size.x);
}

void LightPropertiesBinding::MakeConeBProperty(VariantValue& p, std::shared_ptr<Lb3kLightEntity> ptr)
{
	p = VariantValue(LightProperties::ConeB, PropertiesTypes::Float, "ConeB");
	p.SetFloat(ptr->cones[1]);
}

void LightPropertiesBinding::MakeConeAProperty(VariantValue& p, std::shared_ptr<Lb3kLightEntity> ptr)
{
	p = VariantValue(LightProperties::ConeA, PropertiesTypes::Float, "ConeA");
	p.SetFloat(ptr->cones[0]);
}

void LightPropertiesBinding::MakeLightIntensityProperty(VariantValue& p, std::shared_ptr<Lb3kLightEntity> ptr)
{
	p = VariantValue(LightProperties::Intensity, PropertiesTypes::Float, "Intensity");
	p.SetFloat(ptr->intensity);
}

void LightPropertiesBinding::MakeLightColorProperty(VariantValue& p, std::shared_ptr<Lb3kLightEntity> ptr)
{
	p = VariantValue(LightProperties::Color, PropertiesTypes::ColorRGB, "Color");
	p.SetColorRGB(ptr->GetColor());
}

void LightPropertiesBinding::MakeLightAnglesProperty(VariantValue& p, std::shared_ptr<Lb3kLightEntity> ptr)
{
	p = VariantValue(LightProperties::Angles, PropertiesTypes::Angles, "Angles");
	p.SetAngles(ptr->anglesDirection);
}

void LightPropertiesBinding::MakeLightPosProperty(VariantValue& p, std::shared_ptr<Lb3kLightEntity> ptr)
{
	p = VariantValue(LightProperties::Pos, PropertiesTypes::Position, "Pos");
	p.SetPosition(ptr->GetPosition());
}

void LightPropertiesBinding::MakeLightFlagsProperty(VariantValue& p, std::shared_ptr<Lb3kLightEntity> ptr)
{
	p = VariantValue(LightProperties::Flags, PropertiesTypes::Flags, "Flags");
	p.AddEnumValue("Euler", LF_EULER);
	p.AddEnumValue("Dyn", LF_DYN);
	p.AddEnumValue("XYZ", LF_XYZ);
	p.AddEnumValue("Linear", LF_LINEAR);
	p.AddEnumValue("Disk", LF_DISK);
	p.AddEnumValue("Rect", LF_RECT);
	p.SetFlags(ptr->flags);
}

void LightPropertiesBinding::MakeLightTypeProperty(VariantValue& p, std::shared_ptr<Lb3kLightEntity> ptr)
{
	p = VariantValue(LightProperties::Type, PropertiesTypes::Enum, "Type");

	p.AddEnumValue("Omni", (int)LightTypes::Omni);
	p.AddEnumValue("Spot", (int)LightTypes::Spot);
	p.AddEnumValue("Direct", (int)LightTypes::Direct);
	p.SetEnumIndexFromValue((int)ptr->type);
}

void LightPropertiesBinding::UpdateObjectProperties(VariantValue* props, size_t num)
{
	auto ptr = m_pwLightDef.lock();
	if (!ptr)
		return;

	auto history = Application::GetMainWindow()->GetSceneRenderer()->GetScene()->GetEditHistory();

	for (size_t i = 0; i < num; i++)
	{
		auto it = props[i];

		VariantValue oldValue;

		switch (it.GetId())
		{
		case LightProperties::Type:
			MakeLightTypeProperty(oldValue, ptr);
			ptr->SetType((LightTypes)it.GetEnumValue());			
			break;
		case LightProperties::Flags:
			MakeLightFlagsProperty(oldValue, ptr);
			ptr->flags = it.GetFlags();
			break;
		case LightProperties::Pos:
			MakeLightPosProperty(oldValue, ptr);
			ptr->SetPosition(it.GetPosition());
			break;
		case LightProperties::Color:
			MakeLightColorProperty(oldValue, ptr);
			ptr->SetColor(it.GetColorRGB());
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

size_t LightPropertiesBinding::GetSerialNumber()
{
	auto ptr = m_pwLightDef.lock();

	if (!ptr) return 0;

	return ptr->GetSerialNumber();
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

const char* LightPropertiesBinding::ObjectClassname()
{
	auto ptr = m_pwLightDef.lock();

	if (!ptr)
		return "";

	return "Light source (LB3k)";
}

Lb3kLightEntity::Lb3kLightEntity()
{
	SetClassName("light");
}

Lb3kLightEntity::Lb3kLightEntity(Lb3kLightEntity& other): SceneEntity(other)
{
	LightTypes	type = LightTypes::Omni;
	flags = other.flags;
	intensity = other.intensity;
	anglesDirection = other.anglesDirection;
	cones[0] = other.cones[0];
	cones[1] = other.cones[1];
	size = other.size;

	type = other.type;

	style = other.style;
}

void Lb3kLightEntity::SetType(LightTypes newType)
{
	type = newType;

	switch (type)
	{
	case LightTypes::Omni:
        SetEditorIcon(GetCommonIcon(CommonTextures::OmniLight));
		break;
	case LightTypes::Spot:
        SetEditorIcon(GetCommonIcon(CommonTextures::SpotLight));
		break;
	case LightTypes::Direct:
        SetEditorIcon(GetCommonIcon(CommonTextures::DirectLight));
		break;
	default:
		break;

	}
}

void Lb3kLightEntity::OnHovered()
{

}

void Lb3kLightEntity::OnMouseMove(glm::vec2 delta)
{
	
}

void Lb3kLightEntity::OnUnhovered()
{
	
}

void Lb3kLightEntity::RenderForSelection(int objectId,SceneRenderer * pRenderer)
{	
	pRenderer->DrawBillboardSelection(GetPosition(), glm::vec2(8, 8), objectId);
}

void Lb3kLightEntity::OnSelect()
{
	auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();
	auto scene = sceneRenderer->GetScene();

	auto weakRef = scene->GetEntityWeakRef(this);
	scene->HintSelected(weakRef);

	auto ptr = weakRef.lock();
	auto lightWeakRef = std::dynamic_pointer_cast<Lb3kLightEntity>(ptr);

	LightPropertiesBinding* pBinding = new LightPropertiesBinding(lightWeakRef);
	ObjectPropertiesEditor::Instance()->LoadObject(pBinding);

	
}

void Lb3kLightEntity::OnUnSelect()
{
	ObjectPropertiesEditor::Instance()->UnloadObject();
}

const char* Lb3kLightEntity::Description()
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

bool Lb3kLightEntity::IsLightEntity()
{
	return true;
}

void Lb3kLightEntity::SetIntensity(float f)
{
    intensity = f;
}

void Lb3kLightEntity::SetFlags(int _flags)
{
    flags = _flags;
}

void Lb3kLightEntity::SetAngles(float pitch, float yaw, float roll)
{
    anglesDirection = glm::vec3(pitch, yaw, roll);
}

void Lb3kLightEntity::SetCones(float a, float b)
{
    cones[0] = a;
    cones[1] = b;
}

void Lb3kLightEntity::SetSize(float sizeX, float sizeY)
{
    size = glm::vec2(sizeX, sizeY);
}


EntityClasses Lb3kLightEntity::EntityClass()
{
	return EntityClasses::Light;
}

float Lb3kLightEntity::GetIntensity()
{
	return intensity;
}
