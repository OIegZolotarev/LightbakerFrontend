/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

#include "scene_shader.h"

class SpotlightConeShaderProgram : public ISceneShader
{
	GLuint m_unColorOverride;
	GLuint m_unTransform;
	GLuint m_unHeight;
	GLuint m_unConeAngle;

public:
	SpotlightConeShaderProgram();
	~SpotlightConeShaderProgram();

	void SetColor(glm::vec4 color);
	void SetTransform(glm::mat4x4& mat);

	void SetConeHeight(float height);
	void SetConeAngleDegrees(float angle);
};