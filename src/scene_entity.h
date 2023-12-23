/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "gl_texture.h"
#include "object_props.h"
#include "selection_3d.h"
#include "mod_manager.h"

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

typedef struct sentvars_s
{
    // Transformation
    glm::vec3 origin;
    glm::vec3 angles;

    glm::vec3 scale;

    // Bounding boxes
    BoundingBox bboxRelative;
    BoundingBox bboxAbsolute;
    
    // Displaying
    ColorRGBA     rendercolor;
    ColorRGBA     wireframecolor;    

    // Models
    IModelWeakPtr model;
    GLTexture*     editor_icon; // TODO: make sprite-models later
    
    // Studio models
    int   bodynum;
    int   skin;
    int   sequence;
    float fps;
    float frame;
    float blending[2];
    float controller[4];
    float mouth;

    // Editor entity
    uint32_t serialNumber;    
    std::size_t classname_hash;
    std::string classname;

    sentvars_s()
    {
        origin = {0, 0, 0};
        angles = {0, 0, 0};
        scale  = {1, 1, 1};

        bboxRelative = BoundingBox(8);
        bboxAbsolute = BoundingBox(8);

        rendercolor = {1, 1, 1, 1};
        wireframecolor = {1, 1, 1, 1};

        model = IModelWeakPtr();
        editor_icon = nullptr;

        bodynum = 0;
        skin    = 0;
        sequence = 0;
        frame    = 0;
        
        for (auto &it : blending)
            it = 0;

        for (auto &it : controller)
            it = 0;

        mouth = 0;
        serialNumber = 0;

        classname_hash = 0;
        classname      = "";
    }

}sentvars_t;


class Scene;

class SceneEntity : public ISelectableObject
{
    bool m_bDataLoaded = true;    
    EntityClasses m_EntityClass;    
    sentvars_t m_EntVars;

    void RecalcAbsBBox()
    {
        m_EntVars.bboxAbsolute = BoundingBox(m_EntVars.origin, m_EntVars.bboxRelative);
    }

protected:
    
    const BoundingBox &GetRelativeBoundingBox() const;
    const BoundingBox &GetAbsoulteBoundingBox() const;

    void SetClassName(const char *name);
    void LoadPropertiesToPropsEditor(IObjectPropertiesBinding *binder);

    // For transparent sorting
    std::weak_ptr<SceneEntity> m_pNext;

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

    // DECLARE_PROPERTY(uint32_t      , SerialNumber);

    void SetSerialNumber(const uint32_t newNum)
    {
        m_EntVars.serialNumber = newNum;
    }

    const uint32_t GetSerialNumber() const
    {
        return m_EntVars.serialNumber;
    }

    void SetPosition(const glm::vec3 & pos)
    {
        m_EntVars.origin = pos;
        RecalcAbsBBox();
    }

    const glm::vec3 GetPosition() const
    {
        return m_EntVars.origin;
    }
    
    void SetBoundingBox(const BoundingBox & bbox)
    {
        m_EntVars.bboxRelative = bbox;        
    }

    DECLARE_PROPERTY(glm::vec3     , Color);
    DECLARE_PROPERTY(GLTexture *   , EditorIcon);
    DECLARE_PROPERTY(IModelWeakPtr , Model);

    void OnHovered() override;
    void OnMouseMove(glm::vec2 delta) override;
    void OnSelect(ISelectableObjectWeakRef myWeakRef) override;
    void OnUnSelect() override;
    void OnUnhovered() override;

    virtual const char *Description();
    virtual bool        IsLightEntity();
    virtual void        OnAdditionToScene(class Scene *pScene){};

    virtual EntityClasses EntityClass();
    void                  FlagDataLoaded();

    void InvokeSelect();
    const std::string &GetClassName() const;

    template<class T> 
    static T* GetRawSafest(std::weak_ptr<SceneEntity> & weakRef)
    {
        auto ptr = weakRef.lock();

        if (!ptr)
            return nullptr;

        SceneEntity * rawPtr = ptr.get();

        if (! instanceof <T>(rawPtr))
            return nullptr;

        return static_cast<T*>(rawPtr);
    }

    virtual bool IsTransparent();

    std::weak_ptr<SceneEntity> Next();
    void SetNext(std::weak_ptr<SceneEntity> & pOther);
    const BoundingBox &        AbsoulteBoundingBox() const;
};

typedef std::shared_ptr<SceneEntity> SceneEntityPtr;
typedef std::weak_ptr<SceneEntity>   SceneEntityWeakPtr;
