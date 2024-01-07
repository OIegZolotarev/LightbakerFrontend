/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once

#include "viewport.h"

enum class MouseButtons
{
    Left,
    Right,
    Middle,
    Aux1,
    Aux2
};

class IEditingTool
{
    Viewport *m_pActiveViewport;
    Scene *   m_pActiveDocument;
public:
    IEditingTool();
    ~IEditingTool();

    virtual void OnMouseClicked(Viewport * pActiveViewport, Scene* pActiveDocument);
    virtual void OnMouseReleased();
    virtual void Think();

    virtual void RenderUI();
};
