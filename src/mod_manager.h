/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "common.h"
#include "viewport_rendermodes.h"



class SceneEntity;

class IModel
{
    size_t m_Hash;
public:
    IModel(const char* fileName);
    virtual ~IModel(){};
        
    virtual void Render(SceneEntity *pEntity, RenderMode mode) = 0;
    size_t       Hash();

};

typedef std::shared_ptr<IModel> IModelSharedPtr;
typedef std::weak_ptr<IModel>   IModelWeakPtr;

class ModelsManager: public Singleton<ModelsManager>
{
    std::list<IModelSharedPtr> m_lstModels;
public:
    
    ~ModelsManager();

    IModelWeakPtr LookupModel(const char *fileName);
};