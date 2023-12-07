/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "common.h"

class VariantValue;
enum class PropertiesTypes;

namespace GoldSource
{

class FGDPropertyDescriptor;
class BSPEntity;

class BSPProperty
{
    FGDPropertyDescriptor *m_pDescr;
    std::string            m_Name;
    std::string            m_Value;

    VariantValue *m_pValueWrapper;

    BSPEntity *m_pOwner;


    PropertiesTypes AdaptPropertyType();

public:
    BSPProperty(BSPEntity * pOwner,std::string &name, std::string &value, FGDPropertyDescriptor *pDescr);
    ~BSPProperty();


};

} // namespace GoldSource