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

void HelperGeometryShaderProgram::SetColor(glm::vec4 color) const
{
	glUniform4fv(m_unColorOverride, 1, &color[0]);
}



HelperGeometryShaderProgram::HelperGeometryShaderProgram()
{
	AttachVertexShader("res/glprogs/helper_geometry.vert");
	AttachFragmentShader("res/glprogs/helper_geometry.frag");	

	LinkProgram();
	InitCommonSceneUniforms();
	
	m_unTransform			= glGetUniformLocation(m_uiProgramId, "transform");	
	m_unColorOverride		= glGetUniformLocation(m_uiProgramId, "colorOverride");
}
