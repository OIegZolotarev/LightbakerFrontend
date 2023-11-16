/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

#include "scene_shader.h"

class SpotlightConeShaderProgram : public ISceneShader
{
	GLuint m_unColorOverride;	
	GLuint m_unHeight;
	GLuint m_unConeAngle;

public:
	SpotlightConeShaderProgram();
	~SpotlightConeShaderProgram();

	void SetColor(glm::vec4 color) const;   
	void SetConeHeight(float height) const;
    void SetConeAngleDegrees(float angle) const;
};