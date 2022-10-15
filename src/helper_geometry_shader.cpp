/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "application.h"
#include "ui_common.h"
#include "helper_geometry_shader.h"



HelperGeometryShaderProgram::~HelperGeometryShaderProgram()
{
	
}

void HelperGeometryShaderProgram::SetView(float * matrix)
{
	glUniformMatrix4fv(m_unView, 1, GL_FALSE, matrix);
}

void HelperGeometryShaderProgram::SetOrigin(glm::vec3& pos)
{
	glUniform3fv(m_unOrigin, 1, &pos[0]);
}

void HelperGeometryShaderProgram::SetProjection(float* matrix)
{
	glUniformMatrix4fv(m_unProjection, 1, GL_FALSE, matrix);
}

void HelperGeometryShaderProgram::SetColor(glm::vec4 color)
{
	glUniform4fv(m_unColorOverride, 1, &color[0]);
}

void HelperGeometryShaderProgram::SetScale(float param1)
{
	glUniform3f(m_unScale, param1, param1, param1);
}

HelperGeometryShaderProgram::HelperGeometryShaderProgram()
{
	AttachVertexShader("res/glprogs/helper_geometry.vert");
	AttachFragmentShader("res/glprogs/helper_geometry.frag");	

	LinkProgram();

	m_unProjection			= glGetUniformLocation(m_uiProgramId, "projection");
	m_unView				= glGetUniformLocation(m_uiProgramId, "view");
	m_unScale				= glGetUniformLocation(m_uiProgramId, "scale");
	m_unOrigin				= glGetUniformLocation(m_uiProgramId, "origin");
	m_unColorOverride		= glGetUniformLocation(m_uiProgramId, "colorOverride");


}
