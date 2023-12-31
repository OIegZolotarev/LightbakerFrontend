/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "fs_core.h"
#include "mod_manager.h"

class GLTexture;

namespace GoldSource
{
enum class FGDEntityClassType
{
    Solid,
    Point,
    BaseDef
};

BETTER_ENUM(FGDPropertyTypes, int, Integer, Color255, String, Sprite, Studio, Sky, Target_Destination, Target_Source,
            Sound, Flags, Choices, Decal)

// IntegerType | Color255 | String | Sprite | Studio | Sky | TargetDestination | TargetSource | Sound;

// Вспомогательная структура для значений по умолчанию и подсказки Jackhammer
//
typedef struct OptionalDefaultValAndHelp_s
{
    OptionalDefaultValAndHelp_s(std::string _defVal, std::string _help)
    {
        defaultValue = _defVal;
        propertyHelp = _help;
    }

    OptionalDefaultValAndHelp_s(float _defVal, std::string _help)
    {
        defaultValueFloat = _defVal;
        propertyHelp      = _help;
        useFloat          = true;
    }

    OptionalDefaultValAndHelp_s()
    {
    }

    bool        useFloat          = false;
    std::string defaultValue      = "";
    float       defaultValueFloat = 0;
    std::string propertyHelp      = "<No help provided>";
} OptionalDefaultValAndHelp_t;

typedef struct FGDFlagsValue_s
{
    std::string description = "";
    std::string help        = "";
    int         value       = 0;
    bool        enabled     = false;

    FGDFlagsValue_s(std::string &_descr, int _val, bool _enDefault, std::string _help)
    {
        description = _descr;
        value       = _val;
        enabled     = _enDefault;
        help        = _help;
    }

    FGDFlagsValue_s()
    {
    }

} FGDFlagsValue_t;

typedef std::list<FGDFlagsValue_t> FGDFlagsList;

class FGDEntityClass;

class FGDPropertyDescriptor
{


protected:
    std::string m_Name              = "";
    std::string m_Descr             = "";
    std::string m_DefaultValue      = "";
    float       m_DefaultValueFloat = 0;
    std::string m_PropertyHelp      = "";

    int m_Type;

    const FGDEntityClass *m_pBaseClass;

public:
    FGDPropertyDescriptor(FGDPropertyDescriptor *pOther);

    FGDPropertyDescriptor(std::string name, std::string typeId, std::string descr, OptionalDefaultValAndHelp_s defaultValueAndHelp);

    ~FGDPropertyDescriptor();
    const std::string &GetName() const;
    const std::string &GetDescription() const;
    const std::string &GetHelp() const;

    virtual FGDPropertyDescriptor *Clone()
    {
        return new FGDPropertyDescriptor(this);
    }

    void SetBaseClass(const FGDEntityClass * pClass)
    {
        m_pBaseClass = pClass;
    }

    const FGDEntityClass *GetBaseClass() const
    {
        return m_pBaseClass;
    }
};

class FGDFlagsEnumProperty : public FGDPropertyDescriptor
{
public:
    FGDFlagsEnumProperty(std::string name, std::string descr, FGDFlagsList &values,
                         OptionalDefaultValAndHelp_s defValueAndHelp);
    FGDFlagsEnumProperty(FGDFlagsEnumProperty *pOther);

    bool IsSpawnflagsProperty();

    const FGDFlagsList &GetValues() const
    {
        return m_Values;
    }

    virtual FGDPropertyDescriptor *Clone()
    {
        return new FGDFlagsEnumProperty(this);
    }

private:
    FGDFlagsList m_Values;
    bool         m_isFlagsProperty;
};

typedef std::list<FGDPropertyDescriptor *> FGDPropertiesList;

#define FL_SET_COLOR         (1 << 0)
#define FL_SET_SIZE          (1 << 1)
#define FL_SET_MODEL         (1 << 2)
#define FL_SET_SPRITE        (1 << 3)
#define FL_SET_DECAL         (1 << 4)
#define FL_SET_EDITOR_SPRITE (1 << 5)
#define FL_SET_BASE_CLASSES  (1 << 6)
#define FL_SET_BBOX_OFFSET   (1 << 7)

class HammerFGDFile;

class FGDEntityClass
{
    friend class HammerFGDFile;

public:
    FGDEntityClass(HammerFGDFile *pOwner, FGDEntityClassType type, std::string className, std::string description,
                   FGDPropertiesList &props);
    ~FGDEntityClass();

    void SetColor(glm::vec3 color);
    void SetBBox(const glm::vec3 size);
    void SetBBoxOffset(const glm::vec3 offset);
    void SetBBox(const glm::vec3 min, const glm::vec3 max);

    void SetBBox(const BoundingBox & other);

    void SetModel(std::string model);
    void SetSprite(std::string model);
    void SetDecalEntity(bool flag);
    void SetEditorSprite(std::string sprite);
    void SetPropertyExtra(std::string p, float value);

    void AddProperty(FGDPropertyDescriptor *p);

    void SetCtorFlags(int flags);

    void SetBaseClasses(std::list<std::string> classes);
    
    const std::string &GetModelName() const;
    const std::string &ClassName() const;
    const std::string &Description() const;

    const BoundingBox &GetBoundingBox() const;

    glm::vec4 GetColor();

    IModelWeakPtr GetEditorSpite();

    FGDPropertyDescriptor *  FindProperty(const std::string &propertyName) const;
    const FGDPropertiesList &GetProperties() const;

private:
    HammerFGDFile *m_pOwner;

    FGDEntityClassType m_PrimitiveKind;

    int m_CtorDefinitionFlags;

    std::string m_ClassName;
    std::string m_Description;

    // Jackhammer extensions
    glm::vec3 m_BboxOffset;
    size_t    m_EditorSequence;

    BoundingBox m_BoundingBox;
    glm::vec3 m_Color = {1, 0, 1};

    std::string m_Model;
    std::string m_Sprite;
    std::string m_EditorSprite;
    bool        m_bDecal;

    FGDPropertiesList m_Properties;

    std::list<std::string> m_BaseClasses;

    void RelinkInheritedProperties(class HammerFGDFile *pFile);

    IModelWeakPtr m_pEditorSprite;

};

typedef std::shared_ptr<FGDEntityClass> FGDEntityClassSharedPtr;
typedef std::weak_ptr<FGDEntityClass> FGDEntityClassWeakPtr;

class HammerFGDFile
{
    FileData *m_pFileData = nullptr;

    typedef std::pair<std::string, FGDEntityClassSharedPtr> classesMapping_t;
    std::unordered_map<std::string, FGDEntityClassSharedPtr> m_Entities;

public:
    HammerFGDFile(FileData *fd);
    ~HammerFGDFile();

    char *Data();

    std::string &FileName();

    void AddEntityClass(FGDEntityClass *entityDef);
    void RelinkInheritedProperties();

    FGDEntityClassWeakPtr FindEntityClass(const std::string &baseClassStr);
    std::string     AbsoluteResourcePath(std::string &m_EditorSprite);

private:
    std::string m_strFileName;
};

} // namespace GoldSource
