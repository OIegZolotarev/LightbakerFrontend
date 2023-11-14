/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "common.h"
#include "scene_shader.h"
#include "shader_program.h"

class EditorGridShaderProgram : public ISceneShader
{
    GLuint m_unColorOverride;
    GLuint m_unTransform;
    GLuint m_unGridStep;
    GLuint m_unGridHiglightCustom;

    GLuint m_GridAxisColor;
    GLuint m_GridMainColor;
    GLuint m_Grid64thLineColor;
    GLuint m_Grid1024thLineColor;
    GLuint m_GridCustomColor;

  public:
    EditorGridShaderProgram();
    ~EditorGridShaderProgram();

    void SetTransform(glm::mat4x4 &mat);
    
    void SetGridStep(int step);
    void SetGridCustomHighlight(int step);

    void SetAxisColor(glm::vec4 color);
    void SetGridMainColor(glm::vec4 color);
    void SetGrid64thLineColor(glm::vec4 color);
    void SetGrid1024thLineColor(glm::vec4 color);
    void SetGridCustomColor(glm::vec4 color);
  
};
