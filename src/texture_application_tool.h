/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/


#pragma once
#include "editing_tool.h"

class TextureApplicationTool : public IEditingTool
{
public:
    TextureApplicationTool();

    void Render(float flFrameDelta) override;
    void RenderUI() override;

private:
    MaterialAssetsGroup *m_pSelectedAssetsGroup = nullptr;
};
