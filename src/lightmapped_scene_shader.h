/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

#include "common.h"
#include "shader_program.h"
#include "scene_shader.h"


class LightMappedSceneShaderProgram: public ISceneShader
{
public:
	LightMappedSceneShaderProgram();
	~LightMappedSceneShaderProgram();
};

class DiffuseSceneShaderProgram : public ISceneShader
{
public:
	DiffuseSceneShaderProgram();
	~DiffuseSceneShaderProgram();
};

class GroupShadedSceneShaderProgram : public ISceneShader
{
public:
	GroupShadedSceneShaderProgram();
	~GroupShadedSceneShaderProgram();

	void SetObjectColor(glm::vec4 color) const;

private:
	GLuint m_unColor;
};

