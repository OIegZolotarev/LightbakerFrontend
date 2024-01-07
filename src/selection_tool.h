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

class SelectionTool: public IEditingTool
{
    SelectionModes m_SelectionMode = Picker;
public:
    SelectionTool();

    void OnMouseClicked(Viewport *pActiveViewport, Scene *pActiveDocument) override;
    void OnMouseReleased() override;

    void Render(float flFrameDelta) override;
    void RenderUI() override;

private:
    void RenderModeSelectorUI();
};
