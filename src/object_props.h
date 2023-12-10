/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

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
	Bool,
	String,
	
};

typedef std::pair<std::string, int> enumValuePair_t;

class VariantValue
{
private:
    std::string m_strHelp = "";
protected:
	int id;
	PropertiesTypes type;
	
	std::string display_name;	
	

	std::vector<enumValuePair_t> m_EnumOrFlagsValues;

	bool initialized = false;
	float m_Limits[2] = { -FLT_MAX, FLT_MAX };

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

	std::string m_StringValue;

	char* m_TempLabel = 0;

public:
	VariantValue(int _id, PropertiesTypes _type, std::string _displayName);

	VariantValue(const VariantValue &pOther)
    {
        id = pOther.id;
        type = pOther.type;
        display_name = pOther.display_name;
        m_EnumOrFlagsValues = pOther.m_EnumOrFlagsValues;
        initialized         = pOther.initialized;
        
		m_Limits[0]         = pOther.m_Limits[0];
		m_Limits[1]         = pOther.m_Limits[1];

		m_StringValue = pOther.m_StringValue;

		value = pOther.value;

		SetHelp(pOther.m_strHelp);
	}

	VariantValue();
	~VariantValue();

	int FindEnumValueIndex(int enumValue);
	void SetEnumIndexFromValue(int value);

	const int		GetEnumValue() const;
	const int		GetFlags() const;
	const glm::vec3	GetPosition() const;
	const glm::vec3	GetColorRGB() const;
	const glm::vec4	GetColorRGBA() const;
	const float		GetFloat() const;
	const glm::vec3	GetAngles() const;
	const int		GetInt() const;
	const float		GetSizeX() const;
	const bool		GetAsBool() const;
	const char*		GetString() const;

	void SetEnumValue(int val);
	void SetFlags(int val);
	void SetPosition(glm::vec3 val);
	void SetColorRGB(glm::vec3 val);
	void SetColorRGBA(glm::vec4 val);
	void SetFloat(float val);
	void SetAngles(glm::vec3 val);
	void SetInt(int val);
	void SetSizeX(float val);
	void SetBool(bool val);
	void SetString(const char* value);
	void SetString(std::string val);

	const int GetId() const;
	const PropertiesTypes GetType() const;

	void AddEnumValue(const char* descr, int val);
	
	const char* DisplayName() const;
	

	void* Data();

	const std::vector<enumValuePair_t>& GetEnumValues() const; 
	void SetTempLabel(char* param1);
	const char* TempLabel();

	bool IsInitialized()
	{
		return initialized;
	}

	void SetNumericalLimits(float min, float max);
	void ValidateValue();
	float* GetNumericalLimits();
    std::string *GetStdStringPtr();

	const std::string & Help() const
    {
        return m_strHelp;
	}

	void SetHelp(const std::string & str)
    {
        m_strHelp = str;
	}

	void SetDisplayName(const std::string dispName)
    {
        display_name = dispName;
	}
};


class IObjectPropertiesBinding
{
public:
	virtual void FillProperties(std::list<VariantValue*>& collection) = 0;
	virtual void UpdateObjectProperties(VariantValue * props,size_t num) = 0;

	virtual bool IsObjectValid(){
		return true;
	}

	virtual uint32_t GetSerialNumber()
	{
		return 0;
	}

	virtual void OnPropertyChangeSavedToHistory() {};

	virtual ImGuizmo::OPERATION GetMeaningfulGizmoOperationMode()
	{
		return (ImGuizmo::OPERATION)0;
	}

	virtual const char* ObjectClassname()
	{
		return "<no classname>";
	}
    
	virtual void RenderFooter();
};