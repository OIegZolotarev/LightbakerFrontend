/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once
#include "shader_program.h"
#include "common.h"

class HelperGeometryShaderProgram: public ShaderProgram
{
	GLuint m_unColorOverride;
	GLuint m_unScale;
	GLuint m_unOrigin;
	GLuint m_unView;
	GLuint m_unProjection;

public:
	HelperGeometryShaderProgram();
	~HelperGeometryShaderProgram();

	void SetView(float* matrix);

	void SetOrigin(glm::vec3& pos);
	void SetProjection(float* matrix);
	void SetColor(glm::vec4 color);
	void SetScale(float param1);
};

