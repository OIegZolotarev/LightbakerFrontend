/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "common.h"
#include "geometry_selection_shader.h"

GeometrySelectionShaderProgram::GeometrySelectionShaderProgram()
{
	AttachVertexShader("res/glprogs/object_selection.vert");
	AttachFragmentShader("res/glprogs/object_selection.frag");

	LinkProgram();
	InitCommonSceneUniforms();

	m_unTransform = glGetUniformLocation(m_uiProgramId, "transform");
	m_unColor = glGetUniformLocation(m_uiProgramId, "colorOverride");
}

GeometrySelectionShaderProgram::~GeometrySelectionShaderProgram()
{

}

void GeometrySelectionShaderProgram::SetTransform(glm::mat4x4& mat)
{
	glUniformMatrix4fv(m_unTransform, 1, GL_FALSE, &mat[0][0]);
}

void GeometrySelectionShaderProgram::SetObjectId(int id)
{
	union
	{
		int id;
		byte col[4];
	}data;

	data.id = id;

	glUniform4f(m_unColor,data.col[0] / 255.f, data.col[1] / 255.f, data.col[2] / 255.f, data.col[3] / 255.f);
}
