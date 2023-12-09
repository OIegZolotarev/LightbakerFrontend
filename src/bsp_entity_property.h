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


class SpecialKeys
{
public:
    static size_t KeyOrigin();
    static size_t Key_Light();
    static size_t Key_Wad();
    static size_t KeyWad();
    static size_t KeyAngles();
    static size_t KeyFlags();
    static size_t KeyClassname();
};

enum class PropertyMetatype
{
    Classname,
    Origin,
    Angles,
    Flags,
    Light,
    Wad,
    None
};

// BSPProperty - Incapsulates BSP property and it's metada in form of FGDProperty,
// Also handles converting data from various types to another
class BSPEntityProperty: public VariantValue
{
    FGDPropertyDescriptor *m_pDescriptor;
    std::string            m_Name;
    
    BSPEntity *     m_pOwner;
    PropertiesTypes AdaptPropertyType();

    size_t m_Hash;
    PropertyMetatype m_iSpecialId;

    static glm::vec3 ConvertOriginToSceneSpace(glm::vec3  &bspSpaceOrigin);
    static glm::vec3 ConvertOriginFromSceneSpace(glm::vec3 & pos);
    

public:
    BSPEntityProperty(BSPEntityProperty * pOther);
    BSPEntityProperty(BSPEntity *pOwner, std::string &name, std::string &value, FGDPropertyDescriptor *pDescr);
    ~BSPEntityProperty();

    std::string &   Name();
    VariantValue*   Value();

    size_t Hash();

    static size_t CalcHash(std::string &val);
    void SerializeAsKeyValue(FILE *fp);

    FGDPropertyDescriptor *PropertyDescriptor();

    void SetDescriptor(FGDPropertyDescriptor *descr);

private:
    void ParseValue(std::string &value);
    void ParseOrigin(std::string &value);
    void ParseAngles(std::string& value);
    void ParseWad(std::string &value);
    void ParseClassname(std::string &value);


};

} // namespace GoldSource