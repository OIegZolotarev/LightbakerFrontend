/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "lightmapped_scene_shader.h"

LightMappedSceneShaderProgram::LightMappedSceneShaderProgram()
{
	AttachVertexShader("res/glprogs/scene_ligtmapped.vert");
	AttachFragmentShader("res/glprogs/scene_ligtmapped.frag");

	LinkProgram();
	InitCommonSceneUniforms();

	LinkSamplerToUnit("diffuse", 0);
	LinkSamplerToUnit("lightmap", 1);
}

LightMappedSceneShaderProgram::~LightMappedSceneShaderProgram()
{

}
