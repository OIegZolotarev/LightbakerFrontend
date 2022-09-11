#include "common.h"
#include "lights.h"


void LightPropertiesBinding::FillProperties(std::vector<propsData_t>& collection)
{
	propsData_t p = propsData_s(LightProperties::Type, PropertiesTypes::Enum, "Type");

	p.m_EnumValues.push_back(enumValuePair_t("Omni", (int)LightTypes::Omni));
	p.m_EnumValues.push_back(enumValuePair_t("Spot", (int)LightTypes::Spot));
	p.m_EnumValues.push_back(enumValuePair_t("Direct", (int)LightTypes::Direct));

	collection.push_back(p);

	p = propsData_s(LightProperties::Flags, PropertiesTypes::Flags, "Flags");
	collection.push_back(p);

	p = propsData_s(LightProperties::Pos, PropertiesTypes::Position, "Pos");
	collection.push_back(p);

	p = propsData_s(LightProperties::Color, PropertiesTypes::ColorRGB, "Color");
	collection.push_back(p);

	p = propsData_s(LightProperties::Intensity, PropertiesTypes::Float, "Intensity");
	collection.push_back(p);

	p = propsData_s(LightProperties::Angles, PropertiesTypes::Angles, "Angles");
	collection.push_back(p);

	p = propsData_s(LightProperties::ConeA, PropertiesTypes::Float, "ConeA");
	collection.push_back(p);

	p = propsData_s(LightProperties::ConeB, PropertiesTypes::Float, "ConeB");
	collection.push_back(p);

	p = propsData_s(LightProperties::Size, PropertiesTypes::SizeX, "Size");
	collection.push_back(p);

}

void LightPropertiesBinding::UpdateObjectProperties(std::vector<propsData_t>& collection)
{
	
		
}


