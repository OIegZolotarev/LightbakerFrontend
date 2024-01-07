/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once
#include "editing_tool.h"

class CameraTool: public IEditingTool
{
public:

    CameraTool();

    void OnMouseClicked(Viewport *pActiveViewport, Scene *pActiveDocument) override;
    void OnMouseReleased() override;

    void Render(float flFrameDelta) override;
    void RenderUI() override;
};
