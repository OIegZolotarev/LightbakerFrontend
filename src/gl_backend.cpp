/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "gl_backend.h"
#include <stddef.h>
#include "application.h"

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
	glDrawArrays(m_drawMode, first, num > 0 ? num : m_NumElements);	
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(drawVert_t), (void*)offsetof(drawVert_t, xyz));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(drawVert_t), (void*)offsetof(drawVert_t, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(drawVert_t), (void*)offsetof(drawVert_t, tangent));
	glEnableVertexAttribArray(2);
	
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(drawVert_t), (void*)offsetof(drawVert_t, color));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(drawVert_t), (void*)offsetof(drawVert_t, uv));
	glEnableVertexAttribArray(4);


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

GLuint ShaderProgram::MakeShader(const char* fileName, GLuint type)
{	
	auto fd = Application::GetFileSystem()->LoadFile(fileName);

	if (!fd)
		return 0;

	
	auto src = std::string(std::string_view((char*)fd->Data(), fd->Length()));
	auto ptr = src.c_str();

	GLuint result = glCreateShader(type);
	glShaderSource(result, 1, &ptr, NULL);
	glCompileShader(result);

	int  success;
	char infoLog[512];
	glGetShaderiv(result, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(result, 512, NULL, infoLog);
		printf("MakeShader(): failed on \"%s\":\n%s", fileName, infoLog);
	}

	delete fd;

	return result;
}

ShaderProgram::ShaderProgram()
{
	m_uiProgramId = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
	if (m_uiVertexShader) glDeleteShader(m_uiVertexShader);
	if (m_uiGeometryShader) glDeleteShader(m_uiGeometryShader);
	if (m_uiFragmentShader) glDeleteShader(m_uiFragmentShader);

	glDeleteProgram(m_uiProgramId);
}

bool ShaderProgram::AttachVertexShader(const char* fileName)
{
	m_uiVertexShader = MakeShader(fileName,GL_VERTEX_SHADER);
	return m_uiVertexShader != 0;
}

bool ShaderProgram::AttachFragmentShader(const char* fileName)
{
	m_uiFragmentShader = MakeShader(fileName, GL_FRAGMENT_SHADER);
	return m_uiFragmentShader != 0;
}

bool ShaderProgram::AttachGeometryShader(const char* fileName)
{
	//m_uiGeometryShader = MakeShader(fileName, GL_GEOMETRY_SHADER);
	return m_uiGeometryShader != 0;
}

void ShaderProgram::LinkProgram()
{
	if (m_uiVertexShader)		glAttachShader(m_uiProgramId, m_uiVertexShader);
	if (m_uiFragmentShader)		glAttachShader(m_uiProgramId, m_uiFragmentShader);
	if (m_uiGeometryShader)		glAttachShader(m_uiProgramId, m_uiGeometryShader);
	
	glLinkProgram(m_uiProgramId);
}

void ShaderProgram::Bind()
{
	glUseProgram(m_uiProgramId);
}

void ShaderProgram::Unbind()
{
	glUseProgram(0);
}

GLBackend::GLBackend()
{
	ReloadAllShaders();
}

GLBackend* GLBackend::Instance()
{
	static GLBackend* pInstance = new GLBackend;
	return pInstance;
}

GLBackend::~GLBackend()
{
	if (m_pHelperGeometryShader) delete m_pHelperGeometryShader;
	if (m_pLightmappedSceneShader) delete m_pLightmappedSceneShader;
	if (m_pGeometrySelectionShader) delete m_pGeometrySelectionShader;
	if (m_pSpotlightConeShader) delete m_pSpotlightConeShader;
	if (m_pDiffuseSceneShader) delete m_pDiffuseSceneShader;
}

HelperGeometryShaderProgram* GLBackend::HelperGeometryShader()
{
	return m_pHelperGeometryShader;
}

GeometrySelectionShaderProgram* GLBackend::GeometrySelectionShader()
{
	return m_pGeometrySelectionShader;
}

LightMappedSceneShaderProgram* GLBackend::LightMappedSceneShader()
{
	return m_pLightmappedSceneShader;
}

DiffuseSceneShaderProgram* GLBackend::DiffuseSceneShader()
{
	return m_pDiffuseSceneShader;
}

GroupShadedSceneShaderProgram* GLBackend::GroupShadedSceneShader()
{
	return m_pGroupShadedSceneShader;
}

SpotlightConeShaderProgram* GLBackend::SpotlightConeShader()
{
	return m_pSpotlightConeShader;
}

void GLBackend::ReloadAllShaders()
{
	if (m_pHelperGeometryShader) delete m_pHelperGeometryShader;
	if (m_pLightmappedSceneShader) delete m_pLightmappedSceneShader;
	if (m_pGeometrySelectionShader) delete m_pGeometrySelectionShader;
	if (m_pSpotlightConeShader) delete m_pSpotlightConeShader;
	
	if (m_pLightmappedSceneShader) delete m_pLightmappedSceneShader;
	if (m_pGroupShadedSceneShader) delete m_pGroupShadedSceneShader;

	m_pHelperGeometryShader = new HelperGeometryShaderProgram;
	m_pLightmappedSceneShader = new LightMappedSceneShaderProgram;
	m_pGeometrySelectionShader = new GeometrySelectionShaderProgram;
	m_pSpotlightConeShader = new SpotlightConeShaderProgram;

	m_pGroupShadedSceneShader = new GroupShadedSceneShaderProgram;
	m_pDiffuseSceneShader = new DiffuseSceneShaderProgram;
}
