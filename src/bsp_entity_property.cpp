/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"

#include "bsp_entity.h"
#include "bsp_entity_property.h"
#include "object_props.h"
#include "text_utils.h"
#include "wad_textures.h"
#include "file_system.h"
#include "game_configuration.h"
#include "goldsource_game_configuration.h"
#include "Scene.h"


using namespace GoldSource;

BSPEntityProperty::BSPEntityProperty(BSPEntity * pOwner, std::string &name, std::string &value, FGDPropertyDescriptor *pDescr)
{
    m_Name   = name;
    m_Hash   = CalcHash(name);

    m_pDescr = pDescr;
        
    PropertiesTypes ptType = AdaptPropertyType();
    m_pValueWrapper        = new VariantValue(0, ptType, m_Name);
   
    m_pOwner = pOwner;
    
    ParseValue(value);
}

BSPEntityProperty::~BSPEntityProperty()
{
    delete m_pValueWrapper;
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

 BSPEntityProperty::BSPEntityProperty(BSPEntityProperty *pOther)
{
    m_pDescr = pOther->m_pDescr;
    m_Name   = pOther->m_Name;

    PropertiesTypes ptType = AdaptPropertyType();
    m_pValueWrapper        = new VariantValue(0, ptType, m_Name);

    m_pOwner        = nullptr;
    m_Hash          = pOther->m_Hash;
}

size_t BSPEntityProperty::CalcHash(std::string &val)
{
   
    return std::hash<std::string>{}(val);
}

void BSPEntityProperty::ParseValue(std::string &value)
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
        break;
    case PropertyMetatype::Light:
        break;    
    case PropertyMetatype::Classname:
        ParseClassname(value);
        break;
    default:
        break;
    
    }
}

void BSPEntityProperty::ParseOrigin(std::string &value)
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
                
        m_pValueWrapper->SetPosition(ConvertOriginToSceneSpace(origin));
    }

    //SetPosition();
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

void GoldSource::BSPEntityProperty::ParseAngles(std::string& value)
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

        // TODO: Fixme
        m_pValueWrapper->SetColorRGBA(color);
    }
}

void BSPEntityProperty::ParseClassname(std::string &value)
{
    m_pOwner->SetClassName(value.c_str());

    auto                     scene          = m_pOwner->m_pScene;
    GameConfigurationWeakPtr pConfigWeakPtr = scene->UsedGameConfiguration();
    
    GoldSource::HammerGameConfiguration *hammerConfig = GoldSource::HammerGameConfiguration::Get(pConfigWeakPtr);
        
    if (!hammerConfig)
    {
        return;
    }
    
    auto pFGDClass = hammerConfig->LookupFGDClass(value);
    m_pOwner->SetFGDClass(pFGDClass);
}

void BSPEntityProperty::ParseWad(std::string &value)
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
    static auto sKey = std::hash<std::string>{}("flags");
    return sKey;
}

size_t SpecialKeys::KeyClassname()
{
    static auto sKey = std::hash<std::string>{}("classname");
    return sKey;
}
