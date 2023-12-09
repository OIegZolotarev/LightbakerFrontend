/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "hammer_fgd.h"
#include "common.h"

using namespace GoldSource;
extern void ParseFGD(GoldSource::HammerFGDFile *file);

char *HammerFGDFile::Data()
{
    return (char *)m_pFileData->Data();
}

HammerFGDFile::HammerFGDFile(FileData *fd)
{
    m_pFileData = fd;

    m_strFileName = fd->Name();

    ParseFGD(this);
    RelinkInheritedProperties();
}

HammerFGDFile::~HammerFGDFile()
{
    for (auto &kv : m_Entities)
    {
        delete kv.second;
    }
}

void HammerFGDFile::AddEntityClass(FGDEntityClass *entityDef)
{
    classesMapping_t ttt = classesMapping_t(entityDef->ClassName(), entityDef);
    m_Entities.insert(ttt);
}

void HammerFGDFile::RelinkInheritedProperties()
{
    for (auto &kv : m_Entities)
    {
        FGDEntityClass *classDef = kv.second;
        classDef->RelinkInheritedProperties(this);
    }
}

GoldSource::FGDEntityClass *HammerFGDFile::FindEntityClass(std::string &baseClassStr)
{
    if (!m_Entities.contains(baseClassStr))
        return nullptr;

    return m_Entities[baseClassStr];
}

std::string HammerFGDFile::AbsoluteResourcePath(std::string &relPath)
{
    auto fs      = FileSystem::Instance();
    auto baseDir = fs->BaseDirectoryFromPath(FileName());

    return baseDir + "/" + relPath;
}

FGDEntityClass::~FGDEntityClass()
{
}

FGDEntityClass::FGDEntityClass(HammerFGDFile *pOwner, FGDEntityClassType type, std::string className,
                               std::string description, FGDPropertiesList &props)
{
    m_Type        = type;
    m_ClassName   = className;
    m_Description = description;

    m_Properties = props;
    m_pOwner     = pOwner;
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

const std::string &FGDEntityClass::ClassName() const
{
    return m_ClassName;
}

void FGDEntityClass::RelinkInheritedProperties(HammerFGDFile *pOwner)
{
    for (auto &baseClassStr : m_BaseClasses)
    {
        FGDEntityClass *baseClass = pOwner->FindEntityClass(baseClassStr);

        if (!baseClass)
            continue;

        baseClass->RelinkInheritedProperties(pOwner);

        if (!(m_CtorDefinitionFlags & FL_SET_COLOR) && baseClass->m_CtorDefinitionFlags & FL_SET_COLOR)
            SetColor(baseClass->m_Color);
        if (!(m_CtorDefinitionFlags & FL_SET_SIZE) && baseClass->m_CtorDefinitionFlags & FL_SET_SIZE)
            SetBBox(baseClass->m_Mins, baseClass->m_Maxs);
        if (!(m_CtorDefinitionFlags & FL_SET_MODEL) && baseClass->m_CtorDefinitionFlags & FL_SET_MODEL)
            SetModel(baseClass->m_Model);
        if (!(m_CtorDefinitionFlags & FL_SET_SPRITE) && baseClass->m_CtorDefinitionFlags & FL_SET_SPRITE)
            SetSprite(baseClass->m_Sprite);
        if (!(m_CtorDefinitionFlags & FL_SET_DECAL) && baseClass->m_CtorDefinitionFlags & FL_SET_DECAL)
            SetDecalEntity(baseClass->m_bDecal);
        if (!(m_CtorDefinitionFlags & FL_SET_EDITOR_SPRITE) && baseClass->m_CtorDefinitionFlags & FL_SET_EDITOR_SPRITE)
            SetEditorSprite(baseClass->m_EditorSprite);

        // TODO: скопировать свойства базовых классов? —делать два списка свойств - наследованные и объ€вленные
        // ƒобавить дл€ скорости третий список? »ли же проще их копировать?
    }

    for (auto it = m_BaseClasses.rbegin(); it != m_BaseClasses.rend(); it++)
    {
        FGDEntityClass *baseClass = pOwner->FindEntityClass(*it);

        if (!baseClass)
            continue;

        // Ѕазовые классы идут в пор€дке наследовани€ (надеюсь), поэтому копировать свойства достаточно на один уровень
        // назад
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

            if (typeid(prop) == typeid((FGDPropertyDescriptor *)0))
            {
                m_Properties.push_front(new FGDPropertyDescriptor(prop));
            }
            else if (typeid(prop) == typeid((FGDFlagsEnumProperty *)0))
            {
                auto castedProp = static_cast<FGDFlagsEnumProperty *>(prop);
                m_Properties.push_front(new FGDFlagsEnumProperty(castedProp));
            }
        }
    }
}

FGDPropertyDescriptor *FGDEntityClass::FindProperty(const std::string &propertyName) const
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

GLTexture *FGDEntityClass::GetEditorSpite()
{
    if (m_EditorSprite.empty())
        return nullptr;

    if (!m_pEditorSprite)
    {
        std::string path = m_pOwner->AbsoluteResourcePath(m_EditorSprite);
        m_pEditorSprite  = TextureManager::LoadTextureSynch(path.c_str());
    }

    return m_pEditorSprite;
}

void FGDEntityClass::SetBaseClasses(std::list<std::string> classes)
{
    m_BaseClasses = classes;
}

void FGDEntityClass::SetCtorFlags(int flags)
{
    m_CtorDefinitionFlags = flags;
}

void FGDEntityClass::AddProperty(FGDPropertyDescriptor *p)
{
    m_Properties.push_back(p);
}

const std::string &FGDEntityClass::Description() const
{
    return m_Description;
}

FGDFlagsEnumProperty::FGDFlagsEnumProperty(std::string name, std::string desc, FGDFlagsList &values,
                                           OptionalDefaultValAndHelp_t defValueAndHelp)
    : FGDPropertyDescriptor(name, "flags", desc, defValueAndHelp)
{
    m_Values = values;
}

FGDFlagsEnumProperty::FGDFlagsEnumProperty(FGDFlagsEnumProperty *pOther) : FGDPropertyDescriptor(pOther)
{
    m_Values          = pOther->m_Values;
    m_isFlagsProperty = (m_Name == "spawnflags");
}

bool FGDFlagsEnumProperty::IsSpawnflagsProperty()
{
    return m_isFlagsProperty;
}

const std::string &FGDPropertyDescriptor::GetName() const
{
    return m_Name;
}

FGDPropertyDescriptor::~FGDPropertyDescriptor()
{
}

FGDPropertyDescriptor::FGDPropertyDescriptor(std::string name, std::string typeId, std::string descr,
                                             OptionalDefaultValAndHelp_s defaultValueAndHelp)
{
    m_Name  = name;
    m_Descr = descr;

    try
    {
        m_Type = FGDPropertyTypes::_from_string_nocase(typeId.c_str());
    }
    catch (std::exception &e)
    {
        Con_Printf("Error while parsing type: %s, exception: %s", typeId.c_str(), e.what());
    }

    if (defaultValueAndHelp.useFloat)
        m_DefaultValueFloat = defaultValueAndHelp.defaultValueFloat;
    else
        m_DefaultValue = defaultValueAndHelp.defaultValue;

    m_PropertyHelp = defaultValueAndHelp.propertyHelp;
}

FGDPropertyDescriptor::FGDPropertyDescriptor(FGDPropertyDescriptor *pOther)
{
    m_Name  = pOther->m_Name;
    m_Descr = pOther->m_Descr;

    m_DefaultValue      = pOther->m_DefaultValue;
    m_DefaultValueFloat = pOther->m_DefaultValueFloat;
    m_PropertyHelp      = pOther->m_PropertyHelp;
    m_Type              = pOther->m_Type;
}

const std::string &FGDPropertyDescriptor::GetDescription() const
{
    return m_Descr;
}

const std::string &FGDPropertyDescriptor::GetHelp() const
{
    return m_PropertyHelp;
}
