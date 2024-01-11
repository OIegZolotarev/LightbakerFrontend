/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "hammer_fgd.h"
#include "object_props.h"
#include "scene_entity.h"

class SceneEntity;

namespace GoldSource
{
class BSPEntityProperty;

typedef std::pair<uint32_t, glm::vec3> relativePosPair;

class BSPEntitiesPropertiesBinder
{
    std::string m_strObjectsClassname;

    std::list<SceneEntityWeakPtr>              m_lstSelectedObjects;
    std::list<GoldSource::BSPEntityProperty *> m_lstCommonProperties;

    void RebuildPropertiesList();
    void CleanupDeadObjects();
    void CleanupPropertiesList();

    FGDEntityClassWeakPtr m_pSelectedClass;

public:

    std::list<SceneEntityWeakPtr> &GetSelectedObjects()
    {
        return m_lstSelectedObjects;
    }


    void        SelectEntity(SceneEntityWeakPtr ptr);

    void BuildSelectionRepresentation();

    void        FillProperties(std::list<VariantValue *> &collection);
    const char *ObjectClassname() ;
    void        OnPropertyChangeSavedToHistory() ;

    void RenderFooter() ;

    ImGuizmo::OPERATION GetMeaningfulGizmoOperationMode() ;

    void UpdateProperty(VariantValue *prop) ;

    SceneEntity *GetEntity(int param1) ;

    void AddObject(SceneEntityWeakPtr weakRef);
    void ClearObjects();
    void UpdatePropertyPositionDelta(VariantValue *propertyPosition, glm::vec3 delta);
    
    void OnSelectionResized(std::unordered_map<uint32_t, glm::vec3> &relativePositions, glm::vec3 scale, glm::vec3 centerPos);
    
    void AddObjectBoxSelection(SceneEntityWeakPtr &pObject);
    void FinishBoxSelection();
};

} // namespace GoldSource