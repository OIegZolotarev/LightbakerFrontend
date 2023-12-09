/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "object_props.h"
#include "scene_entity.h"

namespace GoldSource
{

class FGDEntityClass;

class BSPEntityProperty;

class BSPEntitiesPropertiesBinder : public IObjectPropertiesBinding
{
    std::string m_strObjectsClassname;

    std::list<SceneEntityWeakPtr> m_lstSelectedObjects;    
    std::list<GoldSource::BSPEntityProperty*> m_lstCommonProperties;

    void RebuildPropertiesList();
    void CleanupDeadObjects();
    void CleanupPropertiesList();

    FGDEntityClass *m_pSelectedClass = nullptr;

public:

    void        SelectEntity(SceneEntityWeakPtr ptr);
    void FillProperties(std::list<VariantValue*>& collection) override;
    const char *ObjectClassname() override;
    void        OnPropertyChangeSavedToHistory() override;
    void        UpdateObjectProperties(VariantValue *props, size_t num) override;

    void RenderFooter() override;
};

} // namespace GoldSource