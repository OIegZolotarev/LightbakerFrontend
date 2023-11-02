#include "object_props.h"



VariantValue::VariantValue()
{
	//memset(this, 0, sizeof(*this));
	memset(&value, 0, sizeof(value));
}

VariantValue::VariantValue(int _id, PropertiesTypes _type, std::string _displayName, std::string _group_id /*= ""*/)
{
	id = _id;
	type = _type;
	display_name = _displayName;
	group_id = _group_id;

	memset(&value, 0, sizeof(value));
}

VariantValue::~VariantValue()
{

}

int VariantValue::FindEnumValueIndex(int enumValue)
{
	int i = 0;

	for (auto& it : m_EnumOrFlagsValues)
	{
		if (it.second == enumValue)
			return i;

		i++;
	}

	return -1;
}

void VariantValue::SetEnumIndexFromValue(int value)
{
	this->value.asEnum = FindEnumValueIndex(value);
}

const int VariantValue::GetEnumValue() const
{
	if (!m_EnumOrFlagsValues.size())
		return 0;
#pragma warning(disable:4018)
	if (value.asEnum > (m_EnumOrFlagsValues.size() - 1))
		return 0;

	return m_EnumOrFlagsValues[value.asEnum].second;
}

const int VariantValue::GetFlags() const
{
	return value.asFlags;
}

const glm::vec3 VariantValue::GetPosition() const
{
	return value.asPosition;
}

const glm::vec3 VariantValue::GetColorRGB() const
{
	return value.asColorRGB;
}

const glm::vec4 VariantValue::GetColorRGBA() const
{
	return value.asColorRGBA;
}

const float VariantValue::GetFloat() const
{
	return value.asFloat;
}

const glm::vec3 VariantValue::GetAngles() const
{
	return value.asAngles;
}

const int VariantValue::GetInt() const
{
	return value.asInt;
}

const float VariantValue::GetSizeX() const
{
	return value.asFloat;
}

const bool VariantValue::GetAsBool() const
{
	return value.asBool;
}

void VariantValue::SetEnumValue(int val)
{
	initialized = true;
	value.asEnum = val;
}

void VariantValue::SetFlags(int val)
{
	initialized = true;
	value.asFlags = val;
}

void VariantValue::SetPosition(glm::vec3 val)
{
	initialized = true;
	value.asPosition = val;
}

void VariantValue::SetColorRGB(glm::vec3 val)
{
	initialized = true;
	value.asColorRGB = val;
}

void VariantValue::SetColorRGBA(glm::vec4 val)
{
	initialized = true;
	value.asColorRGBA = val;
}

void VariantValue::SetFloat(float val)
{
	initialized = true;
	value.asFloat = val;
}

void VariantValue::SetAngles(glm::vec3 val)
{
	initialized = true;
	value.asAngles = val;
}

void VariantValue::SetInt(int val)
{
	initialized = true;
	value.asInt = val;
}

void VariantValue::SetSizeX(float val)
{
	initialized = true;
	value.asFloat = val;
}

void VariantValue::SetBool(bool val)
{
	initialized = true;
	value.asBool = val;
}

void VariantValue::AddEnumValue(const char* descr, int val)
{
	m_EnumOrFlagsValues.push_back(enumValuePair_t(descr, val));
}

const int VariantValue::GetId() const
{
	return id;
}

const const char* VariantValue::DisplayName() const
{
	return display_name.c_str();
}

const PropertiesTypes VariantValue::GetType() const
{
	return type;
}

void* VariantValue::Data()
{
	return (void*)&value.asBool;
}

const std::vector<enumValuePair_t>& VariantValue::GetEnumValues() const
{
	return m_EnumOrFlagsValues;
}

void VariantValue::SetTempLabel(char* param1)
{
	m_TempLabel = param1;
}

const char* VariantValue::TempLabel()
{
	return m_TempLabel;
}

void VariantValue::SetNumericalLimits(float min, float max)
{
	m_Limits[0] = min;
	m_Limits[1] = max;
}

void VariantValue::ValidateValue()
{
	switch (type)
	{		
	case PropertiesTypes::Float:	
		value.asFloat = std::clamp(value.asFloat, m_Limits[0], m_Limits[1]);
		break;
	case PropertiesTypes::SizeX:
	case PropertiesTypes::Int:	
		value.asInt = std::clamp(value.asInt, (int)m_Limits[0], (int)m_Limits[1]);		
		break;	
	default:
		break;

	}
}

float* VariantValue::GetNumericalLimits()
{
	return m_Limits;
}
