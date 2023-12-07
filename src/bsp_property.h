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



// BSPProperty - Incapsulates BSP property and it's metada in form of FGDProperty,
// Also handles converting data from various types to another
class BSPProperty
{
    FGDPropertyDescriptor *m_pDescr;
    std::string            m_Name;
    std::string            m_Value;

    VariantValue *m_pValueWrapper;

    BSPEntity *     m_pOwner;
    PropertiesTypes AdaptPropertyType();

    size_t m_Hash;

public:
    BSPProperty(BSPProperty * pOther);


    BSPProperty(BSPEntity *pOwner, std::string &name, std::string &value, FGDPropertyDescriptor *pDescr);
    ~BSPProperty();

    std::string &Name();
    std::string &Value();

    size_t Hash();

    static size_t CalcHash(std::string & val);
};

} // namespace GoldSource