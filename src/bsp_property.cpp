/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "bsp_property.h"
#include "object_props.h"

using namespace GoldSource;

BSPProperty::BSPProperty(BSPEntity * pOwner, std::string &name, std::string &value, FGDPropertyDescriptor *pDescr)
{
    m_Name   = name;
    m_pDescr = pDescr;

    PropertiesTypes ptType = AdaptPropertyType();

    m_pValueWrapper = new VariantValue(0, ptType, name);

    m_pOwner = pOwner;
}

BSPProperty::~BSPProperty()
{
    delete m_pValueWrapper;
}

PropertiesTypes BSPProperty::AdaptPropertyType()
{
    return PropertiesTypes::String;
}
