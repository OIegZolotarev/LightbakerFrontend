/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/

#pragma once
#include "bsp_entities_properties_binding.h"
#include "object_props.h"
#include "toolui_panel.h"

class Viewport;

class ObjectPropertiesEditor : public ToolUIPanel
{
    ImGuiID m_FlagsEditorID = 0;

    ObjectPropertiesEditor();

    std::unordered_map<uint32_t, glm::vec3>  m_mapRelativeObjectsPos;
    GoldSource::BSPEntitiesPropertiesBinder *m_pPropertiesBinding = nullptr;

    // Properties
    std::list<VariantValue *> m_lstProperties;
    void                      UpdateProperty(VariantValue *it);

    VariantValue *m_pCurrentFlagProp = nullptr;

    // Properties UI
    void RenderFlagsEditor();
    void RenderPropetiesPane();
    void RenderPropertyControl(VariantValue *it);

    // Gizmo
    int           m_GizmoMode;
    bool          m_bGuizmoEdited  = false;
    glm::mat4     m_matGuizmo      = glm::mat4(1);
    glm::mat4     m_matDeltaGuizmo = glm::mat4(1);
    BoundingBox   m_AllObjectsBounds;
    VariantValue *m_pGuizmoPropertyPosition = nullptr;
    VariantValue *m_pGuizmoPropertyRotation = nullptr;

    void ResetGuizmoScaling();
    void UpdateAllObjectsBoundingBox();
    void UpdateRelativePositions();

    void SetupGuizmo();
    void EditTransform(Viewport *pViewport, bool editTransformDecomposition);
    void HandleGizmoChange();

    bool CheckObjectValidity();

    VariantValue *FindFirstPropertyByType(PropertiesTypes type);

    VariantValue m_OldPropertyValue;
    VariantValue m_OldPropertyValue2;

    void RenderEditor();

    void UpdatePositionDelta(VariantValue *propertyPosition, glm::vec3 delta);

public:
    static ObjectPropertiesEditor *Instance();
    ~ObjectPropertiesEditor();

    // Object manipulation

    GoldSource::BSPEntitiesPropertiesBinder *GetBindings();
    void LoadObject(SceneEntityWeakPtr &pObject, bool addToSelection);

    void UnloadObject();

    void ReloadPropertyValue(int id);

    DockPanels GetDockSide() override;

    const BoundingBox GetAllObjectsBoundingBox() const;

    void RenderGuizmo(Viewport *pViewport);
    void Render() override;
    void RenderDebugOctree();

    void ReloadPropertiesFromBinder();
    
};