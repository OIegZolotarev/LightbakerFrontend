/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "bsp_property.h"
#include "object_props.h"
#include <type_traits>

using namespace GoldSource;

BSPProperty::BSPProperty(BSPEntity * pOwner, std::string &name, std::string &value, FGDPropertyDescriptor *pDescr)
{
    m_Name   = name;
    m_pDescr = pDescr;
        
    // not required for storing property instance
    m_pValueWrapper        = nullptr;
    m_pOwner = pOwner;

    m_Hash = CalcHash(name);
}

BSPProperty::~BSPProperty()
{
    delete m_pValueWrapper;
}

PropertiesTypes BSPProperty::AdaptPropertyType()
{
    return PropertiesTypes::String;
}

std::string &BSPProperty::Value()
{
    return m_Value;
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

size_t GoldSource::BSPProperty::CalcHash(std::string & val)
{
    return std::hash<std::string>{}(val);
}
