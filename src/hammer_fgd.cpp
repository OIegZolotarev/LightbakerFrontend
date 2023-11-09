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

FGDEntityClass::FGDEntityClass(FGDEntityClassType type, std::string className, std::string description)
{
	m_Type = type;
	m_ClassName = className;
	m_Description = description;
}

void FGDEntityClass::SetColor255(float r, float g, float b)
{
	m_Color.r = r / 255.f;
	m_Color.g = g / 255.f;
	m_Color.b = b / 255.f;
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
