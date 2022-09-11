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
	
};

typedef std::pair<std::string, int> enumValuePair_t;

typedef struct propsData_s
{
	int id;
	PropertiesTypes type;
	std::string display_name;

	std::vector<enumValuePair_t> m_EnumValues;

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
	}value;

	char* m_TempLabel = 0;

	propsData_s(int _id, PropertiesTypes _type, std::string _displayName)
	{
		id = _id;
		type = _type;
		display_name = _displayName;

		memset(&value, 0, sizeof(value));
	}

}propsData_t;


class IObjectPropertiesBinding
{
public:
	virtual void FillProperties(std::vector<propsData_t>& collection) = 0;
	virtual void UpdateObjectProperties(std::vector<propsData_t>& collection) = 0;
};