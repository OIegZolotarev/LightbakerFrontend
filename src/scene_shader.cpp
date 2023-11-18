/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "scene_shader.h"
#include "application.h"
#include "camera.h"

void ISceneShader::InitCommonSceneUniforms()
{
	m_unProjection = glGetUniformLocation(m_uiProgramId, "projection");
	m_unView = glGetUniformLocation(m_uiProgramId, "view");
	m_unScale = glGetUniformLocation(m_uiProgramId, "scale");
    m_unTransform  = glGetUniformLocation(m_uiProgramId, "transform");
}

void ISceneShader::LinkSamplerToUnit(const char* samplerName, GLuint unitNumber)
{
	glUseProgram(m_uiProgramId);
	GLuint uniform = glGetUniformLocation(m_uiProgramId, samplerName);
	glUniform1i(uniform, unitNumber);
	glUseProgram(0);
}

ISceneShader::ISceneShader()
{

}

ISceneShader::~ISceneShader()
{
	// handle by ShaderProgram
}

void ISceneShader::SetView(float *matrix) const
{
	glUniformMatrix4fv(m_unView, 1, GL_FALSE, matrix);
}

void ISceneShader::SetProjection(float *matrix) const
{
	glUniformMatrix4fv(m_unProjection, 1, GL_FALSE, matrix);
}


void ISceneShader::SetScale(float param1) const
{
	glUniform3f(m_unScale, param1, param1, param1);
}

void ISceneShader::SetDefaultCamera() const
{
	auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();
	auto camera = sceneRenderer->GetCamera();

	auto matProj = camera->GetProjectionMatrix();
    auto matModelView = camera->GetViewMatrix();
	SetProjection((float*)&matProj);
	SetView((float*)&matModelView);
}

void ISceneShader::SetTransform(glm::mat4 transform) const
{
    glUniformMatrix4fv(m_unTransform, 1, GL_FALSE, &transform[0][0]);
}

void ISceneShader::SetTransformIdentity() const
{
    SetTransform(glm::mat4(1));
}
