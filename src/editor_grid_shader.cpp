/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "editor_grid_shader.h"
#include "ui_common.h"

EditorGridShaderProgram::~EditorGridShaderProgram()
{
}

void EditorGridShaderProgram::SetGridStep(int step) const
{
    glUniform1i(m_unGridStep, step);
}

void EditorGridShaderProgram::SetGridCustomHighlight(int step) const
{
    glUniform1i(m_unGridHiglightCustom, step);
}

void EditorGridShaderProgram::SetAxisColor(glm::vec4 color) const
{
    glUniform4fv(m_GridAxisColor, 1, &color[0]);
}

void EditorGridShaderProgram::SetGridMainColor(glm::vec4 color) const
{
    glUniform4fv(m_GridMainColor, 1, &color[0]);
}

void EditorGridShaderProgram::SetGrid64thLineColor(glm::vec4 color) const
{
    glUniform4fv(m_Grid64thLineColor, 1, &color[0]);
}

void EditorGridShaderProgram::SetGrid1024thLineColor(glm::vec4 color) const
{
    glUniform4fv(m_Grid1024thLineColor, 1, &color[0]);
}

void EditorGridShaderProgram::SetGridCustomColor(glm::vec4 color) const
{
    glUniform4fv(m_GridCustomColor, 1, &color[0]);
}

EditorGridShaderProgram::EditorGridShaderProgram()
{
    AttachVertexShader("res/glprogs/editor_grid.vert");
    AttachFragmentShader("res/glprogs/editor_grid.frag");

    LinkProgram();
    InitCommonSceneUniforms();

    m_unTransform          = glGetUniformLocation(m_uiProgramId, "transform");
    m_unGridStep           = glGetUniformLocation(m_uiProgramId, "gridStep");
    m_unGridHiglightCustom = glGetUniformLocation(m_uiProgramId, "gridHiglightCustom");

    m_GridAxisColor       = glGetUniformLocation(m_uiProgramId, "m_GridAxisColor");
    m_GridMainColor       = glGetUniformLocation(m_uiProgramId, "m_GridMainColor");
    m_Grid64thLineColor   = glGetUniformLocation(m_uiProgramId, "m_Grid64thLineColor");
    m_Grid1024thLineColor = glGetUniformLocation(m_uiProgramId, "m_Grid1024thLineColor");
    m_GridCustomColor     = glGetUniformLocation(m_uiProgramId, "m_GridCustomColor");
}
