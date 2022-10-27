/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

#include "common.h"
#include "shader_program.h"


class ISceneShader: public ShaderProgram
{
private:
	GLuint m_unScale = 0xffffffff;
	GLuint m_unView = 0xffffffff;
	GLuint m_unProjection = 0xffffffff;
protected:
	void InitCommonSceneUniforms();
	void LinkSamplerToUnit(const char* sampleName, GLuint unitNumber);
public:
	ISceneShader();
	~ISceneShader();

	void SetView(float* matrix);
	void SetProjection(float* matrix);
	void SetScale(float scale);

	void SetDefaultCamera();
};

