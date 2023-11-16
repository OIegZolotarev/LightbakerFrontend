/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

#include "shader_program.h"
#include "scene_shader.h"

class GeometrySelectionShaderProgram : public ISceneShader
{
	GLuint m_unTransform;
	GLuint m_unColor;
public:
	GeometrySelectionShaderProgram();
	~GeometrySelectionShaderProgram();
	
    void SetObjectId(int id) const;
};


