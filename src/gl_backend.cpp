#include "gl_backend.h"
#include <stddef.h>

DrawMesh::DrawMesh(int flags)
{
	m_iFlags = flags;
	memset(&m_tempVert, 0, sizeof(m_tempVert));
}

DrawMesh::~DrawMesh()
{
	if (m_vboId != 0)
		glDeleteBuffers(1, &m_vboId);

	FreeVector(m_Data);
}

void DrawMesh::Bind()
{
	glBindVertexArray(m_vaoId);
}

void DrawMesh::Draw(size_t first /*= 0*/, size_t num /*= 0*/)
{	
	glDrawArrays(m_drawMode, 0, num > 0 ? num : m_NumElements);	
}

void DrawMesh::Unbind()
{
	glBindVertexArray(0);
}

void DrawMesh::Begin(GLenum mode)
{
	if (m_vboId != 0)
		glDeleteBuffers(1, &m_vboId);

	if (m_vaoId != 0)
		glDeleteVertexArrays(1, &m_vaoId);

	m_drawMode = mode;
	glGenBuffers(1, &m_vboId);
	glGenVertexArrays(1, &m_vaoId);
}

void DrawMesh::End()
{
	glGenVertexArrays(1, &m_vaoId);
	glBindVertexArray(m_vaoId);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(drawVert_t) * m_Data.size(), m_Data.data(), GL_STATIC_DRAW);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, sizeof(drawVert_t), (void*)offsetof(drawVert_t, xyz));

	if (!(m_iFlags * DrawMeshFlags::NoUV))
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(drawVert_t), (void*)offsetof(drawVert_t, uv));
	}
	
	if (!(m_iFlags & DrawMeshFlags::NoColor))
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_FLOAT, sizeof(drawVert_t), (void*)offsetof(drawVert_t, color));
	}
	

	m_NumElements = m_Data.size();
	FreeVector(m_Data);

	glBindVertexArray(0);
}

void DrawMesh::Vertex2f(float x, float y)
{
	m_tempVert.xyz[0] = x;
	m_tempVert.xyz[1] = y;
	m_tempVert.xyz[2] = 0;

	m_Data.push_back(m_tempVert);
}

void DrawMesh::Vertex2fv(float* v)
{
	m_tempVert.xyz[0] = v[0];
	m_tempVert.xyz[1] = v[1];
	m_tempVert.xyz[2] = 0;

	m_Data.push_back(m_tempVert);
}

void DrawMesh::Vertex3f(float x, float y, float z)
{
	m_tempVert.xyz[0] = x;
	m_tempVert.xyz[1] = y;
	m_tempVert.xyz[2] = z;
	
	m_Data.push_back(m_tempVert);
}

void DrawMesh::Vertex3fv(float* v)
{
	m_tempVert.xyz[0] = v[0];
	m_tempVert.xyz[1] = v[1];
	m_tempVert.xyz[2] = v[2];

	m_Data.push_back(m_tempVert);
}

void DrawMesh::Normal3f(float nx, float ny, float nz)
{
	m_tempVert.normal[0] = nx;
	m_tempVert.normal[1] = ny;
	m_tempVert.normal[2] = nz;
}

void DrawMesh::Normal3fv(float* v)
{
	m_tempVert.normal[0] = v[0];
	m_tempVert.normal[1] = v[1];
	m_tempVert.normal[2] = v[2];
}

void DrawMesh::Tangent3f(float tx, float ty, float tz)
{
	m_tempVert.tangent[0] = tx;
	m_tempVert.tangent[1] = ty;
	m_tempVert.tangent[2] = tz;
}

void DrawMesh::Tangent3fv(float* v)
{
	m_tempVert.tangent[0] = v[0];
	m_tempVert.tangent[1] = v[1];
	m_tempVert.tangent[2] = v[2];
}

void DrawMesh::Color4f(float r, float g, float b, float a)
{
	m_tempVert.color[0] = r;
	m_tempVert.color[1] = g;
	m_tempVert.color[2] = b;
	m_tempVert.color[3] = a;
}

void DrawMesh::Color4fv(float* v)
{
	m_tempVert.color[0] = v[0];
	m_tempVert.color[1] = v[1];
	m_tempVert.color[2] = v[2];
	m_tempVert.color[3] = v[3];
}

void DrawMesh::Color3f(float r, float g, float b)
{
	m_tempVert.color[0] = r;
	m_tempVert.color[1] = g;
	m_tempVert.color[2] = b;
	m_tempVert.color[3] = 1;
}

void DrawMesh::Color3fv(float* v)
{
	m_tempVert.color[0] = v[0];
	m_tempVert.color[1] = v[1];
	m_tempVert.color[2] = v[2];
	m_tempVert.color[3] = 1;
}

void DrawMesh::TexCoord2f(float u, float v)
{
	m_tempVert.uv[0] = u;
	m_tempVert.uv[1] = v;
}

void DrawMesh::TexCoord2fv(float* v)
{
	m_tempVert.uv[0] = v[0];
	m_tempVert.uv[1] = v[1];
}

void DrawMesh::BindAndDraw()
{
	Bind();
	Draw();
	Unbind();
}
