/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "object_props.h"
#include "scene_entity.h"
#include "hammer_fgd.h"

class SceneEntity;

namespace GoldSource
{

class BSPEntityProperty;

class BSPEntitiesPropertiesBinder : public IObjectPropertiesBinding
{
    std::string m_strObjectsClassname;

    std::list<SceneEntityWeakPtr> m_lstSelectedObjects;    
    std::list<GoldSource::BSPEntityProperty*> m_lstCommonProperties;

    void RebuildPropertiesList();
    void CleanupDeadObjects();
    void CleanupPropertiesList();

    FGDEntityClassWeakPtr m_pSelectedClass;

public:

    void        SelectEntity(SceneEntityWeakPtr ptr);
    void FillProperties(std::list<VariantValue*>& collection) override;
    const char *ObjectClassname() override;
    void        OnPropertyChangeSavedToHistory() override;
    
    

    void RenderFooter() override;

    ImGuizmo::OPERATION GetMeaningfulGizmoOperationMode() override;

    void UpdateProperty(VariantValue *prop) override;

    SceneEntity *GetEntity(int param1) override;

    void AddObject(SceneEntityWeakPtr weakRef) override;
};

} // namespace GoldSource