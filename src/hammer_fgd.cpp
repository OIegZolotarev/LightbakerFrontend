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
	RelinkInheritedProperties();
}

HammerFGDFile::~HammerFGDFile()
{
	for (auto kv : m_Entities)
	{
		delete kv.second;
	}
}

void HammerFGDFile::AddEntityClass(FGDEntityClass* entityDef)
{	
	classesMapping_t ttt = classesMapping_t(entityDef->ClassName(), entityDef);
	m_Entities.insert(ttt);
}

void HammerFGDFile::RelinkInheritedProperties()
{
	for (auto & kv : m_Entities)
	{
		FGDEntityClass* classDef = kv.second;
		classDef->RelinkInheritedProperties(this);
	}
}

GoldSource::FGDEntityClass* HammerFGDFile::FindEntityClass(std::string& baseClassStr)
{
	if (!m_Entities.contains(baseClassStr))
		return nullptr;

	return m_Entities[baseClassStr];
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

const std::string & FGDEntityClass::ClassName() const
{
	return m_ClassName;
}

void FGDEntityClass::RelinkInheritedProperties(HammerFGDFile*  pOwner)
{
	for (auto & baseClassStr : m_BaseClasses)
	{
		FGDEntityClass* baseClass = pOwner->FindEntityClass(baseClassStr);

		if (!baseClass)
			continue;

		baseClass->RelinkInheritedProperties(pOwner);


		if (!(m_CtorDefinitionFlags & FL_SET_COLOR) && baseClass->m_CtorDefinitionFlags & FL_SET_COLOR)			SetColor(baseClass->m_Color);
		if (!(m_CtorDefinitionFlags & FL_SET_SIZE) && baseClass->m_CtorDefinitionFlags & FL_SET_SIZE)				SetBBox(baseClass->m_Mins, baseClass->m_Maxs);
		if (!(m_CtorDefinitionFlags & FL_SET_MODEL) && baseClass->m_CtorDefinitionFlags & FL_SET_MODEL)			SetModel(baseClass->m_Model);
		if (!(m_CtorDefinitionFlags & FL_SET_SPRITE) && baseClass->m_CtorDefinitionFlags & FL_SET_SPRITE)			SetSprite(baseClass->m_Sprite);
		if (!(m_CtorDefinitionFlags & FL_SET_DECAL) && baseClass->m_CtorDefinitionFlags & FL_SET_DECAL)			SetDecalEntity(baseClass->m_bDecal);
		if (!(m_CtorDefinitionFlags & FL_SET_EDITOR_SPRITE) && baseClass->m_CtorDefinitionFlags & FL_SET_EDITOR_SPRITE)	SetEditorSprite(baseClass->m_EditorSprite);

		// TODO: скопировать свойства базовых классов? —делать два списка свойств - наследованные и объ€вленные
		// ƒобавить дл€ скорости третий список? »ли же проще их копировать?
	}

	for (auto it = m_BaseClasses.rbegin(); it != m_BaseClasses.rend(); it++)
	{
		FGDEntityClass* baseClass = pOwner->FindEntityClass(*it);

		if (!baseClass)
			continue;

		// Ѕазовые классы идут в пор€дке наследовани€ (надеюсь), поэтому копировать свойства достаточно на один уровень назад
		// TODO: добавить проверку что базовый класс определен на момент упоминани€?
		for (auto prop : baseClass->m_Properties)
		{
			auto existingProperty = FindProperty(prop->GetName());
			
			// Ќекоторые классы друг-друга перекрывают. Ќадо отсе€ть такие пол€
			// TODO: проверить что не нужно соедин€ть флаги.
			if (existingProperty)
			{
				continue;
			}

			if (typeid(prop) == typeid((FGDPropertyDescriptor*)0))
			{
				m_Properties.push_front(new FGDPropertyDescriptor(prop));
			}
			else if (typeid(prop) == typeid((FGDFlagsEnumProperty*)0))
			{
				auto castedProp = static_cast<FGDFlagsEnumProperty* > (prop);
				m_Properties.push_front(new FGDFlagsEnumProperty(castedProp));
			}
			
		}
			
	}
}

GoldSource::FGDPropertyDescriptor* FGDEntityClass::FindProperty(std::string& propertyName)
{
	for (auto it : m_Properties)
	{
		if (it->GetName() == propertyName)
		{
			return it;
		}
	}

	return nullptr;
}

glm::vec3 FGDEntityClass::GetMins()
{
    return m_Mins;
}

glm::vec3 FGDEntityClass::GetMaxs()
{
    return m_Maxs;
}

glm::vec3 FGDEntityClass::GetColor()
{
    return m_Color;
}

void FGDEntityClass::SetBBoxOffset(glm::vec3 offset)
{
    m_BboxOffset = offset;
}

FGDFlagsEnumProperty::FGDFlagsEnumProperty(std::string name, std::string desc, FGDFlagsList &values,
                                           OptionalDefaultValAndHelp_t defValueAndHelp)
    : FGDPropertyDescriptor(name, "", desc, defValueAndHelp)
{
	m_Values = values;
}

FGDFlagsEnumProperty::FGDFlagsEnumProperty(FGDFlagsEnumProperty* pOther) : FGDPropertyDescriptor(pOther)
{
	m_Values = pOther->m_Values;
	m_isFlagsProperty = (m_Name == "spawnflags");
}

bool FGDFlagsEnumProperty::IsSpawnflagsProperty()
{
	return m_isFlagsProperty;
}

std::string& FGDPropertyDescriptor::GetName()
{
	return m_Name;
}
