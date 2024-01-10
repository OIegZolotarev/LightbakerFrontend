/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once
#include "editing_tool.h"

enum SelectionModes
{
    Picker,
    BoxSelection,
    Paint
};

class SelectionTool : public IEditingTool
{
    SelectionModes m_SelectionMode = BoxSelection;

    bool m_bMouseDragValid = false;
    bool m_bMouseDragged   = false;

    Viewport *m_pDragViewport;
    glm::vec2 m_vecMouseDragStart;
    glm::vec2 m_vecMouseDragEnd;

    void RenderModeSelectorUI();

    int  HandleKeydownEvent(const SDL_Event &e, const float flFrameDelta);
    int  HandleKeyupEvent(const SDL_Event &e, const float flFrameDelta);
    bool m_bCtrlHeld = false;

    int HandleLeftRelease();
    int HandleLeftClick();

    void SelectHoveredObject();

    VariantValue *m_pSelectionBoxColor;

    Frustum m_BoxSelectionFrustum;

    void UpdateBoxSelectionFrustum();
    
    void SelectItemsInSelectionFrustum();
    void TraverseSelectionBoxFrustum(BVHTree * pTree, BVHNode * pNode, FrustumVisiblity parentVisibility);

    void ValidateMinsMaxs();

public:
    SelectionTool();

    // Rendering and UI
    void Render(float flFrameDelta) override;
    void RenderUI() override;

    // Events handling
    int HandleKeyboardEvent(bool bWasHandled, const SDL_Event &e, const float flFrameDelta) override;
    int HandleMouseEvent(bool bWasHandled, const SDL_Event &e, const float flFrameDelta) override;

    void RenderViewportUI(Viewport *pViewport) override;
};
