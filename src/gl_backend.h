#pragma once

#include "common.h"

typedef struct drawVert_s 
{
	glm::vec3 xyz;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec4 color;
	glm::vec3 uv;
}drawVert_t;

enum DrawMeshFlags
{
	None = 0,
	NoColor = (1<<0),
	NoUV = (1<<1)
};

class DrawMesh
{
	GLuint m_vboId = 0;
	GLuint m_vaoId = 0;
	GLenum m_drawMode = 0;

	std::vector<drawVert_t> m_Data;

	drawVert_t m_tempVert;
	size_t m_NumElements;

	int m_iFlags;
public:
	DrawMesh(int flags = DrawMeshFlags::None);
	~DrawMesh();

	void Bind();
	void Draw(size_t first = 0,size_t num = 0);
	void Unbind();

	void Begin(GLenum mode);
	void End();

	void Vertex2f(float x, float y);
	void Vertex2fv(float* v);

	void Vertex3f(float x, float y, float z);
	void Vertex3fv(float * v);

	void Normal3f(float nx, float ny, float nz);
	void Normal3fv(float* v);

	void Tangent3f(float tx, float ty, float tz);
	void Tangent3fv(float* v);

	void Color4f(float r, float g, float b, float a);
	void Color4fv(float * v);

	void Color3f(float r, float g, float b);
	void Color3fv(float* v);

	void TexCoord2f(float u, float v);
	void TexCoord2fv(float* v);

	size_t CurrentElement()
	{
		return m_Data.size();
	}
	void BindAndDraw();
};
