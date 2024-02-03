/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "bsp_entity_property.h"
#include "gl_texture.h"
#include "mod_manager.h"
#include "object_props.h"
#include "selection_3d.h"
#include "viewport_rendermodes.h"

#include "mathlib.h"
#include "r_bvh_boundingbox.h"
#include "user_presentable_object.h"

// Forward declarations

class Scene;
class ShaderProgram;
class OctreeNode;
namespace GoldSource
{
class BSPEntityProperty;
}
// Forward declarations end
 
enum class EntityClasses
{
    None = 0,
    World,
    GenericEntity,
    Light,
};

typedef struct sentvars_s
{
    // Transformation
    glm::vec3 origin;
    glm::vec3 angles;
    glm::vec3 scale;

    glm::mat4 transform;

    // Bounding boxes
    BoundingBox bboxRelative;
    BVHBoundingBox bboxAbsolute;

    // Displaying
    ColorRGBA rendercolor;
    ColorRGBA wireframecolor;

    // Models
    IModelWeakPtr model;

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
    uint32_t    serialNumber;
    std::size_t classname_hash;
    std::string classname;

    sentvars_s()
    {
        origin = {0, 0, 0};
        angles = {0, 0, 0};
        scale  = {1, 1, 1};

        bboxRelative = BoundingBox(8);
        bboxAbsolute = BVHBoundingBox(8);

        rendercolor    = {1, 1, 1, 1};
        wireframecolor = {1, 1, 1, 1};

        model = IModelWeakPtr();

        bodynum  = 0;
        skin     = 0;
        sequence = 0;
        frame    = 0;

        for (auto &it : blending)
            it = 0;

        for (auto &it : controller)
            it = 0;

        mouth        = 0;
        serialNumber = 0;

        classname_hash = 0;
        classname      = "";

        transform = glm::mat4(1);
    }

} sentvars_t;



class SceneEntity : public ISelectableObject, public IUserPresentableObject
{
    bool          m_bDataLoaded = true;
    EntityClasses m_EntityClass;
    sentvars_t    m_EntVars;

    void RecalcAbsBBox();

protected:
    void SetClassName(const char *name);
    // void LoadPropertiesToPropsEditor(class IObjectPropertiesBinding *binder);

    std::list<GoldSource::BSPEntityProperty *> m_lstProperties;

    Scene *m_pScene             = nullptr;
    bool   m_bRegisteredInScene = false;


public:
    SceneEntity(Scene *pScene);
    
    SceneEntity(SceneEntity &other);
    SceneEntity(SceneEntity *other);

    virtual ~SceneEntity();

    virtual SceneEntity* Clone() = 0;
    

    virtual void Render(RenderMode mode, const SceneRenderer * sr, ShaderProgram *shader){};

    virtual bool IsDataLoaded();

    // Common properties
    void           SetSerialNumber(const uint32_t newNum);
    const uint32_t GetSerialNumber() const;

    const glm::vec3 GetAngles() const;
    void            SetAngles(const glm::vec3 &angles);

    void            SetPosition(const glm::vec3 &pos);
    const glm::vec3 GetPosition() const;

    const glm::mat4 GetTransform();

    void SetBoundingBox(const BoundingBox &bbox);

    void            SetRenderColor(const ColorRGBA &color);
    const ColorRGBA GetRenderColor() const;

    IModelWeakPtr GetModel() const;
    void          SetModel(IModelWeakPtr &model);

    void        SetFrame(const float newVal);
    const float GetFrame() const;

    const BoundingBox &GetRelativeBoundingBox() const;
    const BVHBoundingBox &GetAbsoulteBoundingBox() const;

    void SetTransform(glm::mat4 m_matGuizmo);

    // Selectable object
    void OnHovered() override;
    void OnMouseMove(glm::vec2 delta) override;
    void OnSelect(ISelectableObjectWeakRef myWeakRef) override;
    void OnUnSelect() override;
    void OnUnhovered() override;
    void InvokeSelect();


    virtual const char *Description() const override;


    virtual bool        IsLightEntity();
    virtual void        OnAdditionToScene(class Scene *pScene);

    const std::string &   GetClassName() const;
    virtual EntityClasses EntityClass();
    void                  FlagDataLoaded();

    template <class T> static T *GetRawSafest(std::weak_ptr<SceneEntity> &weakRef);

    void Debug_RenderTransform();

    void FlagRegisteredInScene(bool state);
    const std::list<GoldSource::BSPEntityProperty *> &GetProperties() const;
    Scene *                                           GetScene();
};

template <class T> T *SceneEntity::GetRawSafest(std::weak_ptr<SceneEntity> &weakRef)
{
    auto ptr = weakRef.lock();

    if (!ptr)
        return nullptr;

    SceneEntity *rawPtr = ptr.get();

    if (! instanceof <T>(rawPtr))
        return nullptr;

    return static_cast<T *>(rawPtr);
}

typedef std::shared_ptr<SceneEntity> SceneEntityPtr;
typedef std::weak_ptr<SceneEntity>   SceneEntityWeakPtr;
