/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/

#pragma once

#include "common.h"
#include "editor_grid_shader.h"
#include "geometry_selection_shader.h"
#include "helper_geometry_shader.h"
#include "lightmapped_scene_shader.h"
#include "gl_shader.h"
#include "spotlight_cone_shader.h"
#include "gl_texture.h"

typedef struct drawVert_s
{
    glm::vec3 xyz;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec4 color;
    glm::vec3 uv;
} drawVert_t;

enum DrawMeshFlags
{
    None       = 0,
    NoColor    = (1 << 0),
    NoUV       = (1 << 1),
    HasIndices = (1 << 2),
    Dynamic    = (1 << 3)
};

class DrawMesh
{
    GLuint m_vboId    = 0;
    GLuint m_vaoId    = 0;
    GLuint m_indBuffId = 0;
    GLenum m_drawMode = 0;

    std::vector<drawVert_t> m_Data;

    GLenum m_IndiciesType;
    std::vector<unsigned int> m_Indices;
    size_t m_NumIndices;

    drawVert_t m_tempVert;
    size_t m_NumElements;

    int m_iFlags = 0;

  public:
    DrawMesh(int flags = DrawMeshFlags::None);
    ~DrawMesh();

    void Bind();
    void Draw(uint32_t first = 0, uint32_t num = 0);
    void Unbind();

    void Begin(GLenum mode);
    void End();

    void Vertex2f(float x, float y);
    void Vertex2fv(float *v);

    void Vertex3f(float x, float y, float z);
    void Vertex3fv(float *v);

    void Normal3f(float nx, float ny, float nz);
    void Normal3fv(float *v);

    void Tangent3f(float tx, float ty, float tz);
    void Tangent3fv(float *v);

    void Color4f(float r, float g, float b, float a);
    void Color4fv(float *v);

    void Color3f(float r, float g, float b);
    void Color3fv(float *v);

    void TexCoord2f(float u, float v);
    void TexCoord2fv(float *v);

    void Element1i(size_t idx);

    size_t CurrentElement();

    void BindAndDraw();
    void Element1iv(int *vals, size_t count);

};

typedef struct renderStats_s
{
    size_t nDrawCalls = 0;
    size_t nTriangles = 0;
} renderStats_t;

typedef struct textureUnitState_s
{
    bool enabled = true;
    GLuint texture = 0;
} textureUnitState_t;

class GLBackend
{
    GLBackend();

    // Scene render modes
    LightMappedSceneShaderProgram *m_pLightmappedSceneShader   = nullptr;
    DiffuseSceneShaderProgram *m_pDiffuseSceneShader           = nullptr;
    GroupShadedSceneShaderProgram *m_pGroupShadedSceneShader   = nullptr;
    GeometrySelectionShaderProgram *m_pGeometrySelectionShader = nullptr;
    SpotlightConeShaderProgram *m_pSpotlightConeShader         = nullptr;    

    friend DrawMesh;
    
    renderStats_t m_RenderStats;
    void OnMeshDrawn(DrawMesh *pMesh, size_t numTriangles);
    
    static size_t m_CurrentTextureUnit;
    static textureUnitState_t m_TexturesUnitStates[16];    
    static ShaderProgram *s_BoundShaderProgram;

    std::list<ShaderProgram *> m_LoadedShaders;


    ShaderProgram *m_pSolidGeometryShader;
  public:

    static GLBackend *Instance();
    ~GLBackend();

    ShaderProgram *QueryShader(std::string fileName, std::list<const char*> defines);

    void DeleteAllShaders();

    const GeometrySelectionShaderProgram *GeometrySelectionShader() const;
    const LightMappedSceneShaderProgram *LightMappedSceneShader() const;
    const DiffuseSceneShaderProgram *DiffuseSceneShader() const;
    const GroupShadedSceneShaderProgram *GroupShadedSceneShader() const;
    const SpotlightConeShaderProgram *SpotlightConeShader() const;
    
    ShaderProgram *                 SolidColorGeometryShader() const;

    void ReloadAllShaders();

    void NewFrame();
    renderStats_t *RenderStats();

    static void BindTexture(int unit, const GLTexture *texture);
    static void BindTexture(int unit, GLuint texture);
    static void SetUniformValue(ShaderUniform *it);


};
