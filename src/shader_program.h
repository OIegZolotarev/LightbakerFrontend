/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#pragma once

class ShaderProgram
{
protected:
	GLuint m_uiProgramId = 0;

	GLuint m_uiVertexShader = 0;
	GLuint m_uiGeometryShader = 0;
	GLuint m_uiFragmentShader = 0;

	GLuint MakeShader(const char* fileName, GLuint type);

public:
	ShaderProgram();
	~ShaderProgram();

	bool AttachVertexShader(const char* fileName);
	bool AttachFragmentShader(const char* fileName);
	bool AttachGeometryShader(const char* fileName);

	void LinkProgram();
	
	void Bind();
	void Unbind();
};

