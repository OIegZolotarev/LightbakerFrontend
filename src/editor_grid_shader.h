/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "common.h"
#include "scene_shader.h"
#include "gl_shader.h"

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
    
    void SetGridStep(int step) const;
    void SetGridCustomHighlight(int step) const;

    void SetAxisColor(glm::vec4 color) const;
    void SetGridMainColor(glm::vec4 color) const;
    void SetGrid64thLineColor(glm::vec4 color) const;
    void SetGrid1024thLineColor(glm::vec4 color) const;
    void SetGridCustomColor(glm::vec4 color) const;
  
};
