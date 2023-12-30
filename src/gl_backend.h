/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/

#pragma once

#include "common.h"
#include "editor_grid_shader.h"
#include "helper_geometry_shader.h"

#include "gl_shader.h"
#include "spotlight_cone_shader.h"
#include "gl_texture.h"
#include "gl_draw_mesh.h"



typedef struct renderStats_s
{
    size_t nDrawCalls = 0;
    size_t nTriangles = 0;

    size_t nEntitiesRendered = 0;
    size_t nEntitiesTotal = 0;

    size_t nShaderBinds = 0;
    size_t nUnnecessaryShaderBinds = 0;
    size_t idLastShader = 0;

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
    SpotlightConeShaderProgram *m_pSpotlightConeShader         = nullptr;    

    friend DrawMesh;
    
    renderStats_t m_RenderStats;
    void OnMeshDrawn(DrawMesh *pMesh, size_t numTriangles);
    
    static size_t m_CurrentTextureUnit;
    static textureUnitState_t m_TexturesUnitStates[16];    
    static ShaderProgram *s_BoundShaderProgram;

    static bool m_bBlendingEnabled;
    static GLenum m_eBlendingSFactor;
    static GLenum m_eBlendingDFactor;

    std::list<ShaderProgram *> m_LoadedShaders;


    ShaderProgram *m_pSolidGeometryShader;
  public:

    static GLBackend *Instance();
    ~GLBackend();

    ShaderProgram *QueryShader(std::string fileName, std::list<const char*> defines);

    void DeleteAllShaders();

    
    const SpotlightConeShaderProgram *SpotlightConeShader() const;
    
    ShaderProgram *                 SolidColorGeometryShader() const;

    void ReloadAllShaders();

    void NewFrame();
    renderStats_t *RenderStats();

    static void BindTexture(int unit, const GLTexture *texture);
    static void BindTexture(int unit, GLuint texture);
    static void SetUniformValue(ShaderUniform *it);


    static void SetBlending(bool enable, GLenum sfactor, GLenum dfactor);
    static void SetBlending(bool enable);
};



void _GL_CheckForErrors(const char *filename, int line);

#ifdef GL_DEBUG
#define GL_CheckForErrors() _GL_CheckForErrors(__FILE__, __LINE__)
#else
#define GL_CheckForErrors() (void)0;
#endif