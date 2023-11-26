/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "file_system.h"

namespace GoldSource
{
enum class FGDEntityClassType
{
    Solid,
    Point,
    BaseDef
};

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

    bool useFloat            = false;
    std::string defaultValue = "";
    float defaultValueFloat  = 0;
    std::string propertyHelp = "<No help provided>";
} OptionalDefaultValAndHelp_t;

typedef struct FGDFlagsValue_s
{
    std::string description = "";
    std::string help        = "";
    int value               = 0;
    bool enabled            = false;

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

class FGDPropertyDescriptor
{
  protected:
    std::string m_Name         = "";
    std::string m_Descr        = "";
    std::string m_DefaultValue = "";
    float m_DefaultValueFloat  = 0;
    std::string m_PropertyHelp = "";

  public:
    FGDPropertyDescriptor(FGDPropertyDescriptor *pOther)
    {
        m_Name  = pOther->m_Name;
        m_Descr = pOther->m_Descr;
    }

    FGDPropertyDescriptor(std::string name, std::string typeId, std::string descr,
                          OptionalDefaultValAndHelp_s defaultValueAndHelp)
    {
        m_Name  = name;
        m_Descr = descr;

        if (defaultValueAndHelp.useFloat)
            m_DefaultValueFloat = defaultValueAndHelp.defaultValueFloat;
        else
            m_DefaultValue = defaultValueAndHelp.defaultValue;

        m_PropertyHelp = defaultValueAndHelp.propertyHelp;
    }

    ~FGDPropertyDescriptor()
    {
    }
    std::string &GetName();
};

class FGDFlagsEnumProperty : public FGDPropertyDescriptor
{
  public:
    FGDFlagsEnumProperty(std::string name, std::string descr, FGDFlagsList &values,
                         OptionalDefaultValAndHelp_s defValueAndHelp);
    FGDFlagsEnumProperty(FGDFlagsEnumProperty *pOther);

    bool IsSpawnflagsProperty();

  private:
    FGDFlagsList m_Values;
    bool m_isFlagsProperty;
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

class FGDEntityClass
{
    friend class HammerFGDFile;

  public:
    FGDEntityClass(FGDEntityClassType type, std::string className, std::string description, FGDPropertiesList &props);
    ~FGDEntityClass();

    void SetColor(glm::vec3 color);
    void SetBBox(glm::vec3 size);
    void SetBBoxOffset(glm::vec3 offset);
    void SetBBox(glm::vec3 min, glm::vec3 max);

    void SetModel(std::string model);
    void SetSprite(std::string model);
    void SetDecalEntity(bool flag);
    void SetEditorSprite(std::string sprite);
    void SetPropertyExtra(std::string p, float value);
    ;

    void AddProperty(FGDPropertyDescriptor *p)
    {
        m_Properties.push_back(p);
    }

    void SetCtorFlags(int flags)
    {
        m_CtorDefinitionFlags = flags;
    }

    void SetBaseClasses(std::list<std::string> classes)
    {
        m_BaseClasses = classes;
    }

    const std::string &ClassName() const;

    glm::vec3 GetMins();
    glm::vec3 GetMaxs();

    glm::vec3 GetColor();

  private:
    FGDEntityClassType m_Type;

    int m_CtorDefinitionFlags;

    std::string m_ClassName;
    std::string m_Description;

    // Jackhammer extensions
    glm::vec3 m_BboxOffset;
    size_t m_EditorSequence;

    glm::vec3 m_Mins = {-8, -8, -8}, m_Maxs = {8, 8, 8};
    glm::vec3 m_Color = {1, 0, 1};

    std::string m_Model;
    std::string m_Sprite;
    std::string m_EditorSprite;
    bool m_bDecal;

    FGDPropertiesList m_Properties;

    std::list<std::string> m_BaseClasses;

    FGDPropertyDescriptor *FindProperty(std::string &propertyName);
    void RelinkInheritedProperties(class HammerFGDFile *pFile);
};

class HammerFGDFile
{
    FileData *m_pFileData = nullptr;

    typedef std::pair<std::string, FGDEntityClass *> classesMapping_t;
    std::unordered_map<std::string, FGDEntityClass *> m_Entities;

  public:
    HammerFGDFile(FileData *fd);
    ~HammerFGDFile();

    char *Data();

    std::string FileName()
    {
        return m_pFileData->Name();
    }

    void AddEntityClass(FGDEntityClass *entityDef);
    void RelinkInheritedProperties();

    FGDEntityClass *FindEntityClass(std::string &baseClassStr);
};

} // namespace GoldSource
