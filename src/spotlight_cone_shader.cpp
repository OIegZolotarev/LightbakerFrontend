#include "common.h"
#include "spotlight_cone_shader.h"



SpotlightConeShaderProgram::SpotlightConeShaderProgram()
{
	AttachVertexShader("res/glprogs/spotlight_cone.vert");
	AttachFragmentShader("res/glprogs/spotlight_cone.frag");

	LinkProgram();
	InitCommonSceneUniforms();

	m_unColorOverride	= glGetUniformLocation(m_uiProgramId, "colorOverride");
	m_unHeight			= glGetUniformLocation(m_uiProgramId, "coneHeight");
	m_unConeAngle		= glGetUniformLocation(m_uiProgramId, "coneAngle");
}

SpotlightConeShaderProgram::~SpotlightConeShaderProgram()
{

}

void SpotlightConeShaderProgram::SetColor(glm::vec4 color) const
{
	glUniform4fv(m_unColorOverride, 1, &color[0]);
}


void SpotlightConeShaderProgram::SetConeHeight(float height) const
{
	glUniform1f(m_unHeight, height);
}

void SpotlightConeShaderProgram::SetConeAngleDegrees(float angle) const
{
	glUniform1f(m_unConeAngle, glm::radians(angle));
}
