/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

#include "common.h"
#include "shader_program.h"


class ISceneShader: public ShaderProgram
{
  protected
    :
	GLuint m_unScale = 0xffffffff;
	GLuint m_unView = 0xffffffff;
	GLuint m_unProjection = 0xffffffff;
    GLuint m_unTransform  = 0xffffffff;
  protected:
	void InitCommonSceneUniforms();
	void LinkSamplerToUnit(const char* sampleName, GLuint unitNumber);
public:
	ISceneShader();
	virtual ~ISceneShader();

	void SetView(float *matrix) const;
    void SetProjection(float *matrix) const;
    void SetScale(float scale) const;

	void SetDefaultCamera() const;
    
	void SetTransform(glm::mat4 transform) const;    
	void SetTransformIdentity() const;
};

