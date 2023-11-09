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

typedef std::pair<FGDTokenTypes, std::string> TokenTypeAndValue;

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

class FGDEntityClass
{
public:
	
	FGDEntityClass(FGDEntityClassType type, std::string className, std::string description);
	~FGDEntityClass();

	void SetColor255(float r, float g, float b);
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

private:

	FGDEntityClassType m_Type;
	std::string m_ClassName;
	std::string m_Description;

	glm::vec3 m_Mins = { -8,-8,-8 }, m_Maxs = {8,8,8};
	glm::vec3 m_Color = {1,0,1};

	std::string m_Model;
	std::string m_Sprite;
	std::string m_EditorSprite;
	bool m_bDecal;

	std::vector< FGDPropertyDescriptor*> m_Properties;
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



