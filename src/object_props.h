#pragma once

#include "common.h"

enum class PropertiesTypes
{
	Enum,
	Flags,
	Position,
	ColorRGB,
	ColorRGBA,
	Float,
	Angles,
	Int,
	SizeX,	
	Bool
};

typedef std::pair<std::string, int> enumValuePair_t;

typedef struct propsData_s
{
	int id;
	PropertiesTypes type;
	std::string display_name;
	std::string group_id;

	std::vector<enumValuePair_t> m_EnumOrFlagsValues;

	union data
	{
		int asEnum;
		int asFlags;
		glm::vec3 asPosition;
		glm::vec4 asColorRGBA;
		glm::vec3 asColorRGB;
		float asFloat;
		glm::vec3 asAngles;
		int asInt;
		bool asBool;
	}value;

	char* m_TempLabel = 0;

	propsData_s(int _id, PropertiesTypes _type, std::string _displayName, std::string _group_id = "")
	{
		id = _id;
		type = _type;
		display_name = _displayName;
		group_id = _group_id;

		memset(&value, 0, sizeof(value));
	}

	propsData_s()
	{
		memset(this, 0, sizeof(*this));
	}

	int FindEnumValueIndex(int enumValue)
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

	void SetEnumIndexFromValue(int value)
	{
		this->value.asEnum = FindEnumValueIndex(value);
	}

	int GetEnumValue()
	{
		if (!m_EnumOrFlagsValues.size())
			return 0;
#pragma warning(disable:4018)
		if (value.asEnum > (m_EnumOrFlagsValues.size() - 1))
			return 0;

		return m_EnumOrFlagsValues[value.asEnum].second;
	}

	int GetFlags()
	{
		return value.asFlags;
	}

	glm::vec3 GetPosition()
	{
		return value.asPosition;
	}

	glm::vec3 GetColorRGB()
	{
		return value.asColorRGB;
	}

	glm::vec4 GetColorRGBA()
	{
		return value.asColorRGBA;
	}

	float GetFloat()
	{
		return value.asFloat;
	}

	glm::vec3 GetAngles()
	{
		return value.asAngles;
	}

	int GetInt()
	{
		return value.asInt;
	}

	float GetSizeX()
	{
		return value.asFloat;
	}

	bool GetAsBool()
	{
		return value.asBool;
	}

}propsData_t;


class IObjectPropertiesBinding
{
public:
	virtual void FillProperties(std::vector<propsData_t>& collection) = 0;
	virtual void UpdateObjectProperties(propsData_t * props,size_t num) = 0;

	virtual bool IsObjectValid(){
		return true;
	}
};