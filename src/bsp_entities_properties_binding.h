/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "object_props.h"
#include "scene_entity.h"

namespace GoldSource
{

class BSPProperty;

class BSPEntitiesPropertiesBinder : public IObjectPropertiesBinding
{
    std::list<SceneEntityWeakPtr> m_lstSelectedObjects;    
    std::list<GoldSource::BSPProperty*> m_lstCommonProperties;

    void RebuildPropertiesList();
    void CleanupDeadObjects();
    void CleanupPropertiesList();

public:

    void        SelectEntity(SceneEntityWeakPtr ptr);
    void        FillProperties(std::vector<VariantValue> &collection) override;
    const char *ObjectClassname() override;
    void        OnPropertyChangeSavedToHistory() override;
    void        UpdateObjectProperties(VariantValue *props, size_t num) override;
};

} // namespace GoldSource