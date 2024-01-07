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
    SelectionModes m_SelectionMode = Picker;

    bool m_bMouseDragValid = false;
    glm::vec2 m_vecMouseDragStart;
    glm::vec2 m_vecMouseDragEnd;

public:

    SelectionTool();

    // Rendering and UI
    void Render(float flFrameDelta) override;
    void RenderUI() override;

    // Events handling
    int HandleKeyboardEvent(bool bWasHandled, const SDL_Event &e, const float flFrameDelta) override;
    int HandleMouseEvent(bool bWasHandled, const SDL_Event &e, const float flFrameDelta) override;

private:
    void RenderModeSelectorUI();
};
