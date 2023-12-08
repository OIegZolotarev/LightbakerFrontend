/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "gl_texture.h"
#include "object_props.h"
#include "selection_3d.h"

enum class EntityClasses
{
    None = 0,
    World,
    GenericEntity,
    Light,
};

#define DECLARE_PROPERTY(Type, Name)                                                                                   \
protected:                                                                                                             \
    Type m_##Name;                                                                                                     \
    bool m_bIsSet##Name = false;                                                                                       \
                                                                                                                       \
public:                                                                                                                \
    void Set##Name(Type val)                                                                                           \
    {                                                                                                                  \
        m_##Name       = val;                                                                                          \
        m_bIsSet##Name = true;                                                                                         \
    }                                                                                                                  \
    const Type Get##Name()                                                                                             \
    {                                                                                                                  \
        return m_##Name;                                                                                               \
    }

// Not used 
// typedef std::pair<std::string, std::string>          kvData;
// typedef std::unordered_map<std::string, std::string> TPropertiesMap;

class Scene;

class SceneEntity : public ISelectableObject
{
    bool m_bDataLoaded = true;

    std::size_t m_ClassNameHash;
    std::string m_ClassName;

    EntityClasses m_EntityClass;

protected:
    
    void SetClassName(const char *name);
    void LoadPropertiesToPropsEditor(IObjectPropertiesBinding *binder);

    // TODO: copy-constructor
    // Обобщенные пары ключ-значение
    // TPropertiesMap m_vProperties;

    Scene *m_pScene;

public:
    SceneEntity(Scene * pScene);
    SceneEntity(SceneEntity &other);

    virtual void RenderLightshaded(); // С лайтмапой
    virtual void RenderUnshaded();    // Без лайтмапы
    virtual void RenderBoundingBox(); //
    virtual void RenderDebug();       // Отладочная отрисовка
    virtual void RenderGroupShaded();

    virtual bool IsDataLoaded();

    DECLARE_PROPERTY(uint32_t, SerialNumber);
    DECLARE_PROPERTY(glm::vec3, Position);

    DECLARE_PROPERTY(glm::vec3, Mins);
    DECLARE_PROPERTY(glm::vec3, Maxs);

    DECLARE_PROPERTY(glm::vec3, Color);
    DECLARE_PROPERTY(GLTexture *, EditorIcon);

    void OnHovered() override;
    void OnMouseMove(glm::vec2 delta) override;
    void OnSelect() override;
    void OnUnSelect() override;
    void OnUnhovered() override;

    virtual const char *Description();
    virtual bool        IsLightEntity();
    virtual void        OnAdditionToScene(class Scene *pScene){};

    virtual EntityClasses EntityClass();
    void                  FlagDataLoaded();

};

typedef std::shared_ptr<SceneEntity> SceneEntityPtr;
typedef std::weak_ptr<SceneEntity>   SceneEntityWeakPtr;
