/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once
#include "gl_shader.h"
#include "common.h"
#include "scene_shader.h"

class HelperGeometryShaderProgram: public ISceneShader
{
	GLuint m_unColorOverride;	
	GLuint m_unTransform;
public:
	HelperGeometryShaderProgram();
	~HelperGeometryShaderProgram();
		
	void SetScale(glm::vec3 scale) const;

	void SetColor(glm::vec4 color) const;		
};

