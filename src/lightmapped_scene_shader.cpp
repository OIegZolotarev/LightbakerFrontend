/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "lightmapped_scene_shader.h"

LightMappedSceneShaderProgram::LightMappedSceneShaderProgram()
{
	AttachVertexShader("res/glprogs/scene.vert");
	AttachFragmentShader("res/glprogs/scene_ligtmapped.frag");

	LinkProgram();
	InitCommonSceneUniforms();

	LinkSamplerToUnit("diffuse", 0);
	LinkSamplerToUnit("lightmap", 1);
}

LightMappedSceneShaderProgram::~LightMappedSceneShaderProgram()
{
	
}



DiffuseSceneShaderProgram::DiffuseSceneShaderProgram()
{
	AttachVertexShader("res/glprogs/scene.vert");
	AttachFragmentShader("res/glprogs/scene_diffuse.frag");

	LinkProgram();
	InitCommonSceneUniforms();

	LinkSamplerToUnit("diffuse", 0);
	LinkSamplerToUnit("lightmap", 1);
}

DiffuseSceneShaderProgram::~DiffuseSceneShaderProgram()
{

}

GroupShadedSceneShaderProgram::GroupShadedSceneShaderProgram()
{
	AttachVertexShader("res/glprogs/scene.vert");
	AttachFragmentShader("res/glprogs/scene_groupshaded.frag");

	LinkProgram();
	InitCommonSceneUniforms();

	m_unColor = glGetUniformLocation(m_uiProgramId, "groupColor");
}

GroupShadedSceneShaderProgram::~GroupShadedSceneShaderProgram()
{

}

void GroupShadedSceneShaderProgram::SetObjectColor(glm::vec4 color) const
{
	glUniform4f(m_unColor, color[0], color[1], color[2], color[3]);
}
