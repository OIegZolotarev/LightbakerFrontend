/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "common.h"
#include "viewport_rendermodes.h"
#include <optional>

enum class ModelType
{
    StaticLightmapped = 0,
    Dynamic,
    Sprite,
    Unset
};

class Scene;
class SceneEntity;

class IModel
{
protected:
    void SetTransparent(bool flag);
    void SetType(ModelType type);

private:
    size_t      m_Hash;
    std::string m_Name;
    bool        m_bTransparent = false;

    ModelType m_Type = ModelType::Unset;
    // size_t    m_nRefs = 0;

public:
    IModel(const char *fileName);
    virtual ~IModel();
    ;

    bool         IsTransparent();
    virtual void Render(SceneEntity *pEntity, RenderMode mode) = 0;

    size_t Hash();
    void   SetHash(size_t hash);

    virtual const std::optional<BoundingBox> GetBoundingBox() const;
    virtual void                             OnSceneLoaded(Scene *pScene);
    ;

    const ModelType GetType() const;
    //
    //     void AddReference();
    //     void RemoveReference();
};

typedef std::shared_ptr<IModel> IModelSharedPtr;
typedef std::weak_ptr<IModel>   IModelWeakPtr;

class ModelsManager : public Singleton<ModelsManager>
{
    std::list<IModelSharedPtr> m_lstModels;

public:
    ~ModelsManager();
    IModelWeakPtr LookupModel(const char *fileName, bool canFallback = true);

    void OnSceneLoaded(Scene *pScene);
};