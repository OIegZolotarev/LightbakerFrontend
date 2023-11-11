/*
	LightBaker3000 Frontend project,
	(c) 2023 CrazyRussian
*/


#include "application.h"
#include "common.h"
#include "hammer_fgd.h"

using namespace GoldSource;
extern void ParseFGD(GoldSource::HammerFGDFile* file);

char* HammerFGDFile::Data()
{
	return (char*)m_pFileData->Data();
}

HammerFGDFile::HammerFGDFile(FileData* fd)
{
	m_pFileData = fd;

	ParseFGD(this);
}

HammerFGDFile::~HammerFGDFile()
{

}



FGDEntityClass::~FGDEntityClass()
{

}

FGDEntityClass::FGDEntityClass(FGDEntityClassType type, std::string className, std::string description, FGDPropertiesList & props)
{
	m_Type = type;
	m_ClassName = className;
	m_Description = description;

	m_Properties = props;
}

void FGDEntityClass::SetColor(glm::vec3 color)
{
	m_Color = color;
}

void FGDEntityClass::SetBBox(glm::vec3 size)
{
	m_Mins = size * -0.5f;
	m_Maxs = size * -0.5f;
}

void FGDEntityClass::SetBBox(glm::vec3 min, glm::vec3 max)
{
	m_Mins = min;
	m_Maxs = max;
}

void FGDEntityClass::SetModel(std::string model)
{
	m_Model = model;
}

void FGDEntityClass::SetSprite(std::string model)
{
	m_Sprite = model;
}

void FGDEntityClass::SetDecalEntity(bool flag)
{
	m_bDecal = flag;
}

void FGDEntityClass::SetEditorSprite(std::string sprite)
{
	m_EditorSprite = sprite;
}

void FGDEntityClass::SetPropertyExtra(std::string p, float value)
{
	// Do nothing for now
}

FGDFlagsEnumProperty::FGDFlagsEnumProperty(std::string name, std::string desc, FGDFlagsList & values) : FGDPropertyDescriptor(name, "", desc)
{
	m_Values = values;
}
