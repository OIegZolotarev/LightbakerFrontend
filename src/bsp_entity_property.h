/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "common.h"
#include "object_props.h"

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
    static size_t KeyModel();
};

enum class PropertyMetatype
{
    Classname,
    Origin,
    Angles,
    Flags,
    Light,
    Wad,
    Model,
    None
};

// BSPProperty - Incapsulates BSP property and it's metada in form of FGDProperty,
// Also handles converting data from various types to another
class BSPEntityProperty : public VariantValue
{
    FGDPropertyDescriptor *m_pDescriptor;
    std::string            m_Name;

    BSPEntity *     m_pOwner;
    PropertiesTypes AdaptPropertyType();

    size_t           m_Hash;
    PropertyMetatype m_iSpecialId;

    static glm::vec3 ConvertOriginToSceneSpace(glm::vec3 &bspSpaceOrigin);
    static glm::vec3 ConvertOriginFromSceneSpace(glm::vec3 &pos);

    void ParseOrigin(const std::string &value);
    void ParseLight(const std::string &value);
    void ParseWad(const std::string &value);
    void ParseClassname(const std::string &value);

    void ParseFlags(const std::string &value);
    void RebuildFlagsList();

    void        ParseAngles(const std::string &value);
    
    void        ParseModel(const std::string &value);

public:
    
    BSPEntityProperty(const BSPEntityProperty *pOther);
    BSPEntityProperty(const BSPEntityProperty *pOther, BSPEntity * pNewOwner);


    BSPEntityProperty(const BSPEntity *pOwner, const FGDPropertyDescriptor *pDescr);
    BSPEntityProperty(const BSPEntity *pOwner, const std::string &name, const std::string &value,
                      const FGDPropertyDescriptor *pDescr);

    virtual ~BSPEntityProperty();

    std::string &Name();
    size_t       Hash();

    static size_t CalcHash(const std::string &val);
    void          SerializeAsKeyValue(FILE *fp);
    std::string   SerializeValue();

    FGDPropertyDescriptor *PropertyDescriptor();

    void SetDescriptor(const FGDPropertyDescriptor *descr);

    void Update(BSPEntityProperty *pNewProperty);

    void ParseValue(const std::string &value);
};

} // namespace GoldSource