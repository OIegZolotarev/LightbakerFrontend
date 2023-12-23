/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"

#include "Scene.h"
#include "bsp_entity.h"
#include "bsp_entity_property.h"
#include "fs_core.h"
#include "game_configuration.h"
#include "goldsource_game_configuration.h"
#include "object_props.h"
#include "text_utils.h"
#include "wad_textures.h"

using namespace GoldSource;

BSPEntityProperty::BSPEntityProperty(const BSPEntity *pOwner, const std::string &name, const std::string &value,
                                     const FGDPropertyDescriptor *pDescr)
{
    m_Name = name;
    m_Hash = CalcHash(name);

    SetDescriptor(pDescr);

    m_Type   = AdaptPropertyType();
    m_pOwner = (BSPEntity *)pOwner;

    ParseValue(value);
}

BSPEntityProperty::~BSPEntityProperty()
{
}

PropertiesTypes BSPEntityProperty::AdaptPropertyType()
{
    if (m_Hash == SpecialKeys::KeyOrigin())
    {
        m_iSpecialId = PropertyMetatype::Origin;
        return PropertiesTypes::Position;
    }
    else if (m_Hash == SpecialKeys::KeyAngles())
    {
        m_iSpecialId = PropertyMetatype::Angles;
        return PropertiesTypes::Angles;
    }
    else if (m_Hash == SpecialKeys::KeyFlags())
    {
        m_iSpecialId = PropertyMetatype::Flags;
        return PropertiesTypes::Flags;
    }
    else if (m_Hash == SpecialKeys::KeyClassname())
    {
        m_iSpecialId = PropertyMetatype::Classname;
        return PropertiesTypes::String;
    }
    else if (m_Hash == SpecialKeys::Key_Light())
    {
        m_iSpecialId = PropertyMetatype::Light;
        return PropertiesTypes::ColorRGBA;
    }
    else
        m_iSpecialId = PropertyMetatype::None;

    return PropertiesTypes::String;
}

std::string &BSPEntityProperty::Name()
{
    return m_Name;
}

size_t BSPEntityProperty::Hash()
{
    return m_Hash;
}

BSPEntityProperty::BSPEntityProperty(const BSPEntityProperty *pOther) : VariantValue(*pOther)
{
    m_Name   = pOther->m_Name;
    m_pOwner = nullptr;
    m_Hash   = pOther->m_Hash;

    // TODO: check if this is enough
    m_iSpecialId = pOther->m_iSpecialId;

    SetDescriptor(pOther->m_pDescriptor);
}

BSPEntityProperty::BSPEntityProperty(const BSPEntity *pOwner, const FGDPropertyDescriptor *pDescr)
{
    m_pOwner = (BSPEntity *)pOwner;

    m_Name = pDescr->GetName();
    m_Hash = CalcHash(m_Name);

    m_bInitialized = false;
    SetDescriptor(pDescr);

    m_Type = AdaptPropertyType();
}

size_t BSPEntityProperty::CalcHash(const std::string &val)
{
    return std::hash<std::string>{}(val);
}

void GoldSource::BSPEntityProperty::ParseValue(const std::string &value)
{
    switch (m_iSpecialId)
    {
    case PropertyMetatype::Origin:
        ParseOrigin(value);
        break;
    case PropertyMetatype::Angles:
        ParseAngles(value);
        break;
    case PropertyMetatype::Wad:
        ParseWad(value);
        break;
    case PropertyMetatype::Flags:
        ParseFlags(value);
        break;
    case PropertyMetatype::Light:
        ParseLight(value);
        break;
    case PropertyMetatype::Classname:
        ParseClassname(value);
        break;
    default:
        break;
    }
}

std::string BSPEntityProperty::SerializeValue()
{
    switch (m_iSpecialId)
    {
    case PropertyMetatype::Classname:
        return GetStringStd();
    case PropertyMetatype::Origin: {
        glm::vec3 pos = GetPosition();
        return std::format("{0} {1} {2}", pos[0], pos[1], pos[2]);
    }
    case PropertyMetatype::Angles: {
        glm::vec3 ang = GetAngles();
        return std::format("{0} {1} {2}", ang[0], ang[1], ang[2]);
    }
    case PropertyMetatype::Flags:
        return std::format("{0}", GetFlags());
    case PropertyMetatype::Light: {
        glm::vec4 val = GetColorRGBA();
        return std::format("{0} {1} {2} {3}", val[0], val[1], val[2], val[3]);
    }
    default:
        return GetStringStd();
        break;
    }
}

void BSPEntityProperty::ParseAngles(const std::string &value)
{
    auto digits = TextUtils::SplitTextWhitespaces(value.c_str(), value.size());

    glm::vec3 angles = {0, 0, 0};

    if (digits.size() == 3)
    {
        int i = 0;
        for (auto it = digits.begin(); it != digits.end(); it++, i++)
        {
            angles[i] = std::stof(*it);

            if (isnan(angles[i]))
                angles[i] = 0;
        }

        SetAngles(angles);
        m_pOwner->SetAngles(GetAngles());
    }
}

void BSPEntityProperty::Update(BSPEntityProperty *pNewProperty)
{
    m_Value = pNewProperty->m_Value;

    switch (m_iSpecialId)
    {
    case PropertyMetatype::Classname:
        break;
    case PropertyMetatype::Origin:
        m_pOwner->SetPosition(GetPosition());
        break;
    case PropertyMetatype::Angles:
        m_pOwner->SetAngles(GetAngles());
        break;
    case PropertyMetatype::Flags:
        break;
    case PropertyMetatype::Light:
        break;
    case PropertyMetatype::Wad:
        break;
    case PropertyMetatype::None:
        break;
    default:
        break;
    }
}

void BSPEntityProperty::RebuildFlagsList()
{
    if (!m_pDescriptor)
        return;

    if (! instanceof <FGDFlagsEnumProperty>(m_pDescriptor))
        return;

    m_EnumOrFlagsValues.clear();

    FGDFlagsEnumProperty *pDescr = (FGDFlagsEnumProperty *)m_pDescriptor;

    for (auto &it : pDescr->GetValues())
    {
        AddEnumValue(it.description.c_str(), it.value);
    }
}

void BSPEntityProperty::ParseFlags(const std::string &value)
{
    SetDisplayName("Spawn flags");

    if (m_pDescriptor)
        RebuildFlagsList();
}

void BSPEntityProperty::ParseOrigin(const std::string &value)
{
    auto digits = TextUtils::SplitTextWhitespaces(value.c_str(), value.size());

    glm::vec3 origin = {0, 0, 0};

    if (digits.size() == 3)
    {
        int i = 0;
        for (auto it = digits.begin(); it != digits.end(); it++, i++)
        {
            origin[i] = std::stof(*it);

            if (isnan(origin[i]))
                origin[i] = 0;
        }

        SetPosition(origin);
        m_pOwner->SetPosition(GetPosition());

        SetDisplayName("Origin");
    }

    // SetPosition();
}

glm::vec3 BSPEntityProperty::ConvertOriginToSceneSpace(glm::vec3 &bspSpaceOrigin)
{
    glm::vec3 newOrigin = bspSpaceOrigin;

    // јбсолютно ниху€ не пон€тно
    newOrigin.x = -bspSpaceOrigin.y;
    newOrigin.y = bspSpaceOrigin.z;
    newOrigin.z = -bspSpaceOrigin.x;

    return newOrigin;
}

glm::vec3 BSPEntityProperty::ConvertOriginFromSceneSpace(glm::vec3 &pos)
{
    return glm::vec3{-pos.z, -pos.x, pos.y};
}

GoldSource::FGDPropertyDescriptor *BSPEntityProperty::PropertyDescriptor()
{
    return m_pDescriptor;
}

void BSPEntityProperty::SetDescriptor(const FGDPropertyDescriptor *descr)
{
    m_pDescriptor = (FGDPropertyDescriptor *)descr;

    if (m_pDescriptor)
    {
        SetHelp(m_pDescriptor->GetHelp());

        if (m_iSpecialId == PropertyMetatype::Flags)
        {
            SetDisplayName("Flags");
            RebuildFlagsList();
        }
        else
            SetDisplayName(m_pDescriptor->GetDescription());
    }
    else
        SetDisplayName(m_Name, false);
}

void BSPEntityProperty::ParseLight(const std::string &value)
{
    auto digits = TextUtils::SplitTextWhitespaces(value.c_str(), value.size());

    if (digits.size() == 4)
    {
        glm::vec4 color = {0, 0, 0, 0};

        int  i  = 0;
        auto it = digits.begin();

        for (; i < 3; it++, i++)
            color[i] = std::stof(*it) / 255.f;

        color[3] = std::stof(*it);

        SetColorRGBA(color);
    }
}

void BSPEntityProperty::SerializeAsKeyValue(FILE *fp)
{
    std::string stringValue = SerializeValue();

    fprintf(fp, "\"%s\" \"\%s\"\n", m_Name.c_str(), stringValue.c_str());
}

void BSPEntityProperty::ParseClassname(const std::string &value)
{
    m_pOwner->SetClassName(value.c_str());

    auto                     scene          = m_pOwner->m_pScene;
    GameConfigurationWeakPtr pConfigWeakPtr = scene->UsedGameConfiguration();

    GoldSource::HammerGameConfiguration *hammerConfig = GoldSource::HammerGameConfiguration::Get(pConfigWeakPtr);

    if (!hammerConfig)
    {
        return;
    }

    SetString(value);

    auto pFGDClass = hammerConfig->LookupFGDClass(value);
    m_pOwner->SetFGDClass(pFGDClass);
}

void BSPEntityProperty::ParseWad(const std::string &value)
{
    auto wadFiles = TextUtils::SplitTextSimple(value.c_str(), value.length(), ';');

    for (auto &it : wadFiles)
    {
        // FileSystem will handle (eventually) path resolving, so actually we can not bother with paths
        auto baseName = FileSystem::Instance()->ExtractFileBase(it.c_str()) + ".wad ";
        TextureManager::Instance()->RegisterWAD(baseName.c_str(), false);
    }
}

size_t SpecialKeys::KeyWad()
{
    static auto sKey = std::hash<std::string>{}("wad");
    return sKey;
}

size_t SpecialKeys::Key_Wad()
{
    static auto sKey = std::hash<std::string>{}("_wad");
    return sKey;
}

size_t SpecialKeys::Key_Light()
{
    static auto sKey = std::hash<std::string>{}("_light");
    return sKey;
}

size_t SpecialKeys::KeyOrigin()
{
    static auto sKey = std::hash<std::string>{}("origin");
    return sKey;
}

size_t SpecialKeys::KeyAngles()
{
    static auto sKey = std::hash<std::string>{}("angles");
    return sKey;
}

size_t SpecialKeys::KeyFlags()
{
    static auto sKey = std::hash<std::string>{}("spawnflags");
    return sKey;
}

size_t SpecialKeys::KeyClassname()
{
    static auto sKey = std::hash<std::string>{}("classname");
    return sKey;
}
