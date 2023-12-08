/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "bsp_property.h"
#include "object_props.h"
#include <type_traits>
#include "text_utils.h"
#include "wad_textures.h"
#include "file_system.h"

using namespace GoldSource;

BSPProperty::BSPProperty(BSPEntity * pOwner, std::string &name, std::string &value, FGDPropertyDescriptor *pDescr)
{
    m_Name   = name;
    m_Hash   = CalcHash(name);

    m_pDescr = pDescr;
        
    PropertiesTypes ptType = AdaptPropertyType();
    m_pValueWrapper        = new VariantValue(0, ptType, m_Name);
   
    m_pOwner = pOwner;
    
    ParseValue(value);
}

BSPProperty::~BSPProperty()
{
    delete m_pValueWrapper;
}

PropertiesTypes BSPProperty::AdaptPropertyType()
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

    return PropertiesTypes::String;
}


std::string &BSPProperty::Name()
{
    return m_Name;
}

size_t BSPProperty::Hash()
{
    return m_Hash;
}

 BSPProperty::BSPProperty(BSPProperty *pOther)
{
    m_pDescr = pOther->m_pDescr;
    m_Name   = pOther->m_Name;

    PropertiesTypes ptType = AdaptPropertyType();
    m_pValueWrapper        = new VariantValue(0, ptType, m_Name);

    m_pOwner        = nullptr;
    m_Hash          = pOther->m_Hash;
}

size_t BSPProperty::CalcHash(std::string &val)
{
   
    return std::hash<std::string>{}(val);
}

void BSPProperty::ParseValue(std::string &value)
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
    case PropertyMetatype::None:
        break;
    case PropertyMetatype::Light:
        break;    
    default:
        break;
    }
}

void BSPProperty::ParseOrigin(std::string &value)
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

glm::vec3 BSPProperty::ConvertOriginToSceneSpace(glm::vec3 &bspSpaceOrigin)
{
    glm::vec3 newOrigin = bspSpaceOrigin;

    // јбсолютно ниху€ не пон€тно
    newOrigin.x = -bspSpaceOrigin.y;
    newOrigin.y = bspSpaceOrigin.z;
    newOrigin.z = -bspSpaceOrigin.x;

    return newOrigin;
}

glm::vec3 BSPProperty::ConvertOriginFromSceneSpace(glm::vec3 &pos)
{
    return glm::vec3{-pos.z, -pos.x, pos.y};
}

void GoldSource::BSPProperty::ParseAngles(std::string& value)
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

void BSPProperty::ParseWad(std::string &value)
{
    // E:\Games\Half-Life\valve\halflife.wad;
    // E:\Games\Half-Life\valve\liquids.wad;
    // E:\Games\Half-Life\valve\xeno.wad;
    // E:\Games\Half-Life\valve\decals.wad;
    // E:\Games\Hammer\ZHLT\zhlt.wad;

    auto wadFiles = TextUtils::SplitTextSimple(value.c_str(), value.length(), ';');

    for (auto &it : wadFiles)
    {
        auto baseName = FileSystem::Instance()->ExtractFileBase(it.c_str());

        // ƒебильный хак, надо сделать авто монтаж игропапки
        std::string fullPath = "D:/Games/Steam/steamapps/common/Half-Life/valve/" + baseName + ".wad";

        WADPool::Instance()->LoadWad(fullPath.c_str());
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
