/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/

// clang-format off
#include "application.h"
#include "gl_backend.h"
#include "Camera.h"
#include <stddef.h>
// clang-format on

size_t GLBackend::m_CurrentTextureUnit;
textureUnitState_t GLBackend::m_TexturesUnitStates[16];

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
    GLBackend::Instance()->OnMeshDrawn(this, num > 0 ? num : m_NumElements);

    if (m_iFlags & HasIndices)
    {
        assert(first == 0);

        size_t nElements = num > 0 ? num : m_NumIndices;
        glDrawElements(m_drawMode, nElements, m_IndiciesType, 0);
    }
    else
    {
        size_t nElements = num > 0 ? num : m_NumElements;
        glDrawArrays(m_drawMode, first, nElements);
    }
}

void DrawMesh::Unbind()
{
    glBindVertexArray(0);
}

void DrawMesh::Begin(GLenum mode)
{
//     if (m_vboId != 0)
//         glDeleteBuffers(1, &m_vboId);
// 
//     if (m_vaoId != 0)
//         glDeleteVertexArrays(1, &m_vaoId);

    m_drawMode = mode;


    if (!m_vboId)
        glGenBuffers(1, &m_vboId);

    if (!m_vaoId)
        glGenVertexArrays(1, &m_vaoId);
}

void DrawMesh::End()
{
    glBindVertexArray(m_vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
    
    GLenum use = GL_STATIC_DRAW;

    if (m_iFlags & DrawMeshFlags::Dynamic)
        use = GL_DYNAMIC_DRAW;

    glBufferData(GL_ARRAY_BUFFER, sizeof(drawVert_t) * m_Data.size(), m_Data.data(), use);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(drawVert_t), (void *)offsetof(drawVert_t, xyz));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(drawVert_t), (void *)offsetof(drawVert_t, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(drawVert_t), (void *)offsetof(drawVert_t, tangent));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(drawVert_t), (void *)offsetof(drawVert_t, color));
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(drawVert_t), (void *)offsetof(drawVert_t, uv));
    glEnableVertexAttribArray(4);

    m_NumElements = m_Data.size();
    FreeVector(m_Data);

    if (m_Indices.size() > 0)
    {
        m_iFlags |= HasIndices;

        glGenBuffers(1, &m_indBuffId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indBuffId);

        if (m_NumElements < 0xFFFF)
        {
            std::vector<unsigned short> ushort_buffer;
            ushort_buffer.reserve(m_Indices.size());

            for (size_t i = 0; i < m_Indices.size(); i++)
                ushort_buffer.push_back((unsigned short)m_Indices[i]);

            m_IndiciesType = GL_UNSIGNED_SHORT;
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, ushort_buffer.size() * sizeof(unsigned short), ushort_buffer.data(),
                         GL_STATIC_DRAW);
        }
        else
        {
            m_IndiciesType = GL_UNSIGNED_INT;
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned short), m_Indices.data(),
                         GL_STATIC_DRAW);
        }

        m_NumIndices = m_Indices.size();
        m_Indices.clear();
    }

    glBindVertexArray(0);
}

void DrawMesh::Vertex2f(float x, float y)
{
    m_tempVert.xyz[0] = x;
    m_tempVert.xyz[1] = y;
    m_tempVert.xyz[2] = 0;

    m_Data.push_back(m_tempVert);
}

void DrawMesh::Vertex2fv(float *v)
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

void DrawMesh::Vertex3fv(float *v)
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

void DrawMesh::Normal3fv(float *v)
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

void DrawMesh::Tangent3fv(float *v)
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

void DrawMesh::Color4fv(float *v)
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

void DrawMesh::Color3fv(float *v)
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

void DrawMesh::TexCoord2fv(float *v)
{
    m_tempVert.uv[0] = v[0];
    m_tempVert.uv[1] = v[1];
}

void DrawMesh::Element1i(size_t idx)
{
    m_Indices.push_back(idx);
}

size_t DrawMesh::CurrentElement()
{
    return m_Data.size();
}

void DrawMesh::BindAndDraw()
{
    Bind();
    Draw();
    Unbind();
}

void DrawMesh::Element1iv(int *val, size_t count)
{
    for (size_t i = 0; i < count; i++)
        m_Indices.push_back(val[i]);
}

GLBackend::GLBackend()
{
    ReloadAllShaders();
}

void GLBackend::OnMeshDrawn(DrawMesh *pMesh, size_t numTriangles)
{
    m_RenderStats.nDrawCalls++;
    m_RenderStats.nTriangles += numTriangles;
}

GLBackend *GLBackend::Instance()
{
    static GLBackend *pInstance = new GLBackend;
    return pInstance;
}

GLBackend::~GLBackend()
{
    DeleteAllShaders();
    m_LoadedShaders.clear();
}

ShaderProgram *GLBackend::QueryShader(std::string fileName, std::list<const char *> defines)
{
    ShaderProgram *result = nullptr;
    size_t hashVal        = ShaderProgram::CalculateHash(fileName, defines);

    for (auto it : m_LoadedShaders)
    {
        if (it->Hash() == hashVal)
            return it;
    }

    result = new ShaderProgram(fileName, defines);
    m_LoadedShaders.push_back(result);

    return result;
}

void GLBackend::DeleteAllShaders()
{
    if (m_pHelperGeometryShader)
        delete m_pHelperGeometryShader;
    if (m_pLightmappedSceneShader)
        delete m_pLightmappedSceneShader;
    if (m_pGeometrySelectionShader)
        delete m_pGeometrySelectionShader;
    if (m_pSpotlightConeShader)
        delete m_pSpotlightConeShader;
    if (m_pDiffuseSceneShader)
        delete m_pDiffuseSceneShader;
}

const HelperGeometryShaderProgram *GLBackend::HelperGeometryShader() const
{
    return m_pHelperGeometryShader;
}

const GeometrySelectionShaderProgram *GLBackend::GeometrySelectionShader() const
{
    return m_pGeometrySelectionShader;
}

const LightMappedSceneShaderProgram *GLBackend::LightMappedSceneShader() const
{
    return m_pLightmappedSceneShader;
}

const DiffuseSceneShaderProgram *GLBackend::DiffuseSceneShader() const
{
    return m_pDiffuseSceneShader;
}

const GroupShadedSceneShaderProgram *GLBackend::GroupShadedSceneShader() const
{
    return m_pGroupShadedSceneShader;
}

const SpotlightConeShaderProgram *GLBackend::SpotlightConeShader() const
{
    return m_pSpotlightConeShader;
}

void GLBackend::ReloadAllShaders()
{
    DeleteAllShaders();

    m_pHelperGeometryShader    = new HelperGeometryShaderProgram;
    m_pLightmappedSceneShader  = new LightMappedSceneShaderProgram;
    m_pGeometrySelectionShader = new GeometrySelectionShaderProgram;
    m_pSpotlightConeShader     = new SpotlightConeShaderProgram;

    m_pGroupShadedSceneShader = new GroupShadedSceneShaderProgram;
    m_pDiffuseSceneShader     = new DiffuseSceneShaderProgram;

    for (auto it : m_LoadedShaders)
        it->Reload();
}

void GLBackend::NewFrame()
{
    m_RenderStats.nDrawCalls = 0;
    m_RenderStats.nTriangles = 0;
}

renderStats_t *GLBackend::RenderStats()
{
    return &m_RenderStats;
}

void GLBackend::BindTexture(size_t unit, const GLTexture *texture)
{
    auto state = &m_TexturesUnitStates[unit];

    if (!texture)
    {
        if (state->enabled)
        {
            state->enabled = false;

            if (m_CurrentTextureUnit != unit)
            {
                glActiveTexture(GL_TEXTURE0 + unit);
                m_CurrentTextureUnit = unit;
            }

            glDisable(GL_TEXTURE_2D);
        }

        return;
    }

    BindTexture(unit, texture->GLTextureNum());
}

void GLBackend::BindTexture(size_t unit, GLuint texture)
{
    auto state = &m_TexturesUnitStates[unit];

    if (m_CurrentTextureUnit != unit)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        m_CurrentTextureUnit = unit;
    }

    if (!state->enabled)
    {
        glEnable(GL_TEXTURE_2D);
        state->enabled = true;
    }

    if (state->texture != texture)
    {
        glBindTexture(GL_TEXTURE_2D, texture);
        state->texture = texture;
    }
}

void GLBackend::SetUniformValue(ShaderUniform *it)
{
    Camera *camera = Application::GetMainWindow()->GetSceneRenderer()->GetCamera();

    switch (it->Kind())
    {
    case UniformKind::ProjectionMatrix:
        it->SetMat4(camera->GetProjectionMatrix());
        break;
    case UniformKind::ModelViewMatrix:
        it->SetMat4(camera->GetViewMatrix());
        break;
    case UniformKind::Scale:
        it->SetFloat3({1.f, 1.f, 1.f});
        break;
    default:
        __debugbreak();
        break;
    }
}
