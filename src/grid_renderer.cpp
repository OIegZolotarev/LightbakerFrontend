/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/


#include "application.h"
#include "grid_renderer.h"

GridRenderer::~GridRenderer()
{
    if (m_pGridMesh)
        delete m_pGridMesh;
}

void GridRenderer::Init()
{
    auto persistent = Application::GetPersistentStorage();

    m_GridAxisColor       = persistent->GetSetting(ApplicationSettings::GridAxisColor);
    m_GridMainColor       = persistent->GetSetting(ApplicationSettings::GridMainColor);
    m_Grid64thLineColor   = persistent->GetSetting(ApplicationSettings::Grid64thLineColor);
    m_Grid1024thLineColor = persistent->GetSetting(ApplicationSettings::Grid1024thLineColor);
    m_GridCustomColor     = persistent->GetSetting(ApplicationSettings::GridCustomColor);
    m_GridCustomStep      = persistent->GetSetting(ApplicationSettings::GridCustomStep);

    CreateMesh();

    // TODO: крашится на русской раскладке

    Application::CommandsRegistry()->RegisterCommand(new CCommand(
        GlobalCommands::IncreaseGridStep, "Increase grid step", "]", 0, CMD_ONLY_TEXT, [&]() { StepUpGrid(); }));

    Application::CommandsRegistry()->RegisterCommand(new CCommand(
        GlobalCommands::DecreaseGridStep, "Decrease grid step", "[", 0, CMD_ONLY_TEXT, [&]() { StepDownGrid(); }));
}

void GridRenderer::CreateMesh()
{
    m_pGridMesh = new DrawMesh;

    int spacing     = 1;
    float grid_size = 8192;

    float min_coord = -(grid_size) / 2;
    float max_coord = -min_coord;


    m_pGridMesh->Begin(GL_TRIANGLES);

    // Quad Scheme
    // 0--1
    // | /|
    // |/ |
    // 2--3

    m_pGridMesh->Vertex3f(min_coord, min_coord, 0);
    m_pGridMesh->Vertex3f(max_coord, min_coord, 0);
    m_pGridMesh->Vertex3f(min_coord, max_coord, 0);

    m_pGridMesh->Vertex3f(max_coord, min_coord, 0);
    m_pGridMesh->Vertex3f(max_coord, max_coord, 0);
    m_pGridMesh->Vertex3f(min_coord, max_coord, 0);

    m_pGridMesh->End();
}

void GridRenderer::Render()
{
    auto shader = GLBackend::Instance()->EditorGridShader();
    shader->Bind();
    
    shader->SetTransformIdentity();
    shader->SetDefaultCamera();
    
    shader->SetGridStep(m_iGridResolutions);    
    shader->SetGridCustomHighlight(m_GridCustomStep->GetInt());
    
    shader->SetAxisColor({m_GridAxisColor->GetColorRGB().rgb, 1});
    shader->SetGridMainColor({m_GridMainColor->GetColorRGB().rgb, 1});
    shader->SetGrid64thLineColor({m_Grid64thLineColor->GetColorRGB().rgb, 1});
    shader->SetGrid1024thLineColor({m_Grid1024thLineColor->GetColorRGB().rgb, 1});
    shader->SetGridCustomColor({m_GridCustomColor->GetColorRGB().rgb, 1});


    glm::mat4 identity = glm::mat4(1);

    shader->SetTransform(identity);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_LINE_SMOOTH);
    
    glLineWidth(1);
    m_pGridMesh->BindAndDraw();
    glLineWidth(1);

    glDisable(GL_LINE_SMOOTH);

    glDisable(GL_BLEND);

    shader->Unbind();
}

void GridRenderer::SetGridResolution(int steps)
{
    m_iGridResolutions = steps;
}

void GridRenderer::StepDownGrid()
{
    m_iGridPower       = std::max(0, --m_iGridPower);
    m_iGridResolutions = 1 << m_iGridPower;
    Con_Printf("Grid step: %d\n", m_iGridResolutions);
}

void GridRenderer::StepUpGrid()
{
    m_iGridPower       = std::min(13, ++m_iGridPower);
    m_iGridResolutions = 1 << m_iGridPower;

    Con_Printf("Grid step: %d\n", m_iGridResolutions);
}
