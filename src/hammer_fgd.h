/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/

#pragma once

#include "file_system.h"

namespace GoldSource
{


enum class FGDTokenTypes
{
	SolidClass = 0,
	BaseClass,
	PointClass,
	EqualsSign,
	Colon,
	StringLiteral,
	Identifier,
	Number,
	BaseDef,
	OpeningParenthesis,
	ClosingParenthesis,
	OpeningBracket,
	ClosingBracket,
	Comment,
	SizeBoundingBox,
	Iconsprite,
	Color,
	IntegerType,
	Color255,
	String,
	Sprite,
	Studio,
	Flags,
	Choices,
	
	EndOfFile
};

enum class FGDEntityClassType
{
	Solid,
	Point,
	BaseDef
};

typedef struct  FGDFlagsValue_s
{
	std::string description = "";
	int value = 0;
	bool enabled = false;
	 
	FGDFlagsValue_s(std::string& _descr, int _val, bool _enDefault)
	{
		description = _descr;
		value = _val;
		enabled = _enDefault;
	}

	FGDFlagsValue_s()
	{		
	}

}FGDFlagsValue_t;

typedef std::list<FGDFlagsValue_t>  FGDFlagsList;

class FGDPropertyDescriptor
{
	std::string m_Name;
	std::string m_Descr;

public:
	FGDPropertyDescriptor(std::string name, std::string typeId, std::string descr)
	{
		m_Name = name;
		m_Descr = descr;
	}
	
	~FGDPropertyDescriptor()
	{

	}
};

class FGDFlagsEnumProperty: public FGDPropertyDescriptor
{
public:
 	 FGDFlagsEnumProperty(std::string name, std::string descr, FGDFlagsList & values);
private:
	FGDFlagsList m_Values;
};

typedef std::list< FGDPropertyDescriptor*> FGDPropertiesList;


#define FL_SET_COLOR (1<<0)
#define FL_SET_SIZE (1<<1)
#define FL_SET_MODEL (1<<2)
#define FL_SET_SPRITE (1<<3)
#define FL_SET_DECAL (1<<4)
#define FL_SET_EDITOR_SPRITE (1<<5)
#define FL_SET_BASE_CLASSES (1<<6)

class FGDEntityClass
{
public:
	
	FGDEntityClass(FGDEntityClassType type, std::string className, std::string description, FGDPropertiesList & props);
	~FGDEntityClass();

	void SetColor(glm::vec3 color);
	void SetBBox(glm::vec3 size);
	void SetBBox(glm::vec3 min, glm::vec3 max);

	void SetModel(std::string model);
	void SetSprite(std::string model);
	void SetDecalEntity(bool flag);
	void SetEditorSprite(std::string sprite);
	void SetPropertyExtra(std::string p, float value);;

	void AddProperty(FGDPropertyDescriptor* p)
	{
		m_Properties.push_back(p);
	}

	void SetCtorFlags(int flags)
	{
		m_CtorDefinitionFlags = flags;
	}

	void SetBaseClasses(std::list<std::string> classes)
	{
		m_BaseClasses = classes;
	}

private:

	FGDEntityClassType m_Type;
	
	int m_CtorDefinitionFlags;

	std::string m_ClassName;
	std::string m_Description;

	glm::vec3 m_Mins = { -8,-8,-8 }, m_Maxs = {8,8,8};
	glm::vec3 m_Color = {1,0,1};

	std::string m_Model;
	std::string m_Sprite;
	std::string m_EditorSprite;
	bool m_bDecal;

	FGDPropertiesList m_Properties;
	
	std::list<std::string> m_BaseClasses;
};

class HammerFGDFile
{	
	FileData* m_pFileData = nullptr;

	std::vector<FGDEntityClass*> m_Entities;

public:
	HammerFGDFile(FileData* fd);
	~HammerFGDFile();

	char* Data();

	std::string FileName()
	{
		return m_pFileData->Name();
	}


	void AddEntityClass(FGDEntityClass* entityDef)
	{
		m_Entities.push_back(entityDef);
	}
	
};

}



