/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/

#pragma once
#include "igui_panel.h"
#include "object_props.h"

class ObjectPropertiesEditor : public IGUIPanel
{
    ImGuiID m_FlagsEditorID = 0;

    ObjectPropertiesEditor();

    IObjectPropertiesBinding *m_pPropertiesBinding = nullptr;

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
    bool          m_bGuizmoEdited           = false;
    glm::mat4     m_matGuizmo               = glm::mat4(1);
    VariantValue *m_pGuizmoPropertyPosition = nullptr;
    VariantValue *m_pGuizmoPropertyRotation = nullptr;
    void          SetupGuizmo();
    void EditTransform(float *cameraView, float *cameraProjection, float *matrix, bool editTransformDecomposition);

    bool CheckObjectValidity();

    VariantValue *FindFirstPropertyByType(PropertiesTypes type);

    VariantValue m_OldPropertyValue;
    VariantValue m_OldPropertyValue2;

    void RenderEditor();

public:
    static ObjectPropertiesEditor *Instance();
    ~ObjectPropertiesEditor();

    // Object manipulation
    void LoadObject(IObjectPropertiesBinding *pBindings);
    void UnloadObject();

    int  CurrentSerialNumber(); // TODO: refactor this
    void ReloadPropertyValue(int id);

    DockPanels GetDockSide() override;

    void RenderGuizmo();
    void Render() override;
};