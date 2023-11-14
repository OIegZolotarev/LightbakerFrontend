/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "helpers.h"

class GridRenderer : public Singleton<GridRenderer>
{
    DrawMesh *m_pGridMesh = nullptr;
    
    int m_iGridResolutions = 32;
    int m_iGridPower       = 5;
    
    VariantValue* m_GridAxisColor;
    VariantValue* m_GridMainColor;
    VariantValue* m_Grid64thLineColor;
    VariantValue* m_Grid1024thLineColor;
    VariantValue* m_GridCustomColor;
    VariantValue* m_GridCustomStep;


    

  public:
    ~GridRenderer();

    void Init();

    void CreateMesh();

    void Render();
    void SetGridResolution(int steps);

    void StepDownGrid();    
    void StepUpGrid();
};
