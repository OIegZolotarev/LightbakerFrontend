/*
   LightBaker3000 Frontend project,
   (c) 2023 CrazyRussian
*/

#include "application.h"
#include "shader_program.h"
#include "gl_backend.h"
#include "text_utils.h"
#include <type_traits>
#include <boost/functional/hash.hpp>

static uniformDecl_t g_UniformDecl[]{

    // clang-format off
    {"u_ProjectionMatrix"    , UniformKind::ProjectionMatrix    , UniformDataType::FloatMat4 , 0} ,
    {"u_ModelViewMatrix"     , UniformKind::ModelViewMatrix     , UniformDataType::FloatMat4 , 0} ,
    {"u_TransformMatrix"     , UniformKind::TransformMatrix     , UniformDataType::FloatMat4 , 0} ,
    {"u_Scale"               , UniformKind::Scale               , UniformDataType::FloatVec3 , 0} ,
    {"u_GridStep"            , UniformKind::GridStep            , UniformDataType::Int       , 0} ,
    {"u_GridHighlightCustom" , UniformKind::GridHighlightCustom , UniformDataType::Int       , 0} ,
    {"u_GridAxisColor"       , UniformKind::GridAxisColor       , UniformDataType::FloatVec4 , 0} ,
    {"u_GridMainColor"       , UniformKind::GridMainColor       , UniformDataType::FloatVec4 , 0} ,
    {"u_Grid64thLineColor"   , UniformKind::Grid64thLineColor   , UniformDataType::FloatVec4 , 0} ,
    {"u_Grid1024thLineColor" , UniformKind::Grid1024thLineColor , UniformDataType::FloatVec4 , 0} ,
    {"u_GridCustomColor"     , UniformKind::GridCustomColor     , UniformDataType::FloatVec4 , 0} ,
    {"u_Color"               , UniformKind::Color               , UniformDataType::FloatVec4 , 0} ,
    {"u_RightVector"         , UniformKind::RightVector         , UniformDataType::FloatVec3 , 0} ,
    {"u_UpVector"            , UniformKind::UpVector            , UniformDataType::FloatVec3 , 0} ,
    {"u_Forward"             , UniformKind::ForwardVector       , UniformDataType::FloatVec3 , 0} ,
    {"u_Diffuse"             , UniformKind::Diffuse             , UniformDataType::FloatVec3 , FL_TEXTURE_UNIT0},
    {"u_Lightmap"            , UniformKind::Lightmap            , UniformDataType::FloatVec3 , FL_TEXTURE_UNIT1}
    // clang-format on
};

GLuint ShaderTypesToGL(ShaderTypes type)
{
#define GL_GEOMETRY_SHADER        0x8DD9
#define GL_TESS_EVALUATION_SHADER 0x8E87
#define GL_TESS_CONTROL_SHADER    0x8E88
#define GL_COMPUTE_SHADER         0x91B9

    switch (type)
    {
    case ShaderTypes::Vertex:
        return GL_VERTEX_SHADER;
    case ShaderTypes::Fragment:
        return GL_FRAGMENT_SHADER;
    case ShaderTypes::Geometry:
        return GL_GEOMETRY_SHADER;
    case ShaderTypes::Compute:
        return GL_COMPUTE_SHADER;
    case ShaderTypes::TessControl:
        return GL_TESS_CONTROL_SHADER;
    case ShaderTypes::TessEvaluation:
        return GL_TESS_EVALUATION_SHADER;
    default:
        break;
    }

    return 0;
}

uniformDecl_t *FindUniformDecl(const char *name)
{
    for (size_t i = 0; i < ARRAYSIZE(g_UniformDecl); i++)
    {
        if (!strcmp(name, g_UniformDecl[i].uniformName))
            return &g_UniformDecl[i];
    }

    return nullptr;
}

void ShaderProgram::ParseProgramUniforms()
{
    GLint count;

    glGetProgramiv(m_uiProgramId, GL_ACTIVE_UNIFORMS, &count);
    glUseProgram(m_uiProgramId);

    m_vecUniforms.reserve(count);

    for (size_t i = 0; i < count; i++)
    {
        int size;
        GLuint fmt;
        char name[MAX_UNIFORM_NAME_LEN];

        glGetActiveUniform(m_uiProgramId, i, MAX_UNIFORM_NAME_LEN, nullptr, &size, &fmt, name);

        GLint loc = glGetUniformLocation(m_uiProgramId, name);

        if (loc == -1)
            continue;

        uniformDecl_t *decl = FindUniformDecl(name);

        if (!decl)
            continue;

        // TODO: check actual uniform type and pass it to constructor
        m_vecUniforms.push_back(new ShaderUniform(decl, loc));

        
        if (decl->flags & FL_TEXTURE_UNIT0)
            glUniform1i(loc, 0);
        
        if (decl->flags & FL_TEXTURE_UNIT1)
            glUniform1i(loc, 1);

        if (decl->flags & FL_TEXTURE_UNIT2)
            glUniform1i(loc, 2);

        if (decl->flags & FL_TEXTURE_UNIT3)
            glUniform1i(loc, 3);


    }
}

std::string ShaderProgram::PreprocessIncludes(FileData *fd)
{
    auto lines = TextUtils::SplitTextSimple((const char *)fd->Data(), fd->Length(), '\n');

    std::string result;
    result.reserve(fd->Length());

    for (auto &line : lines)
    {
        if (line.starts_with("#include"))
        {
            auto parts    = TextUtils::SpliteWhitespaces(line);
            auto fileName = parts[1].substr(1, parts[1].length() - 2);

            // TODO: make proper relative paths
            auto path     = "res/glprogs/" + fileName;
            FileData *inc = FileSystem::Instance()->LoadFile(path.c_str());

            result += std::string((const char *)inc->Data());
            result += "\n";

            inc->UnRef();
        }
        else
            result += line + '\n';
    }

    return result;
}

GLuint ShaderProgram::MakeShader(const char *fileName, GLuint type)
{
    auto fd = Application::GetFileSystem()->LoadFile(fileName);

    if (!fd)
        return 0;

    auto src = std::string(std::string_view((char *)fd->Data(), fd->Length()));
    auto ptr = src.c_str();

    GLuint result = glCreateShader(type);
    glShaderSource(result, 1, &ptr, NULL);
    glCompileShader(result);

    int success;

    glGetShaderiv(result, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(result, 512, NULL, infoLog);
        Con_Printf("MakeShader(): failed on \"%s\":\n%s", fileName, infoLog);
    }

    delete fd;

    return result;
}

GLuint ShaderProgram::MakeShader(std::string &source, ShaderTypes type, std::list<const char *> &defines)
{
    size_t num_blocks = defines.size() + 3; // Shader type and shader source

    std::string *definesText = new std::string[defines.size()];

    const char **ptrs = new const char *[num_blocks];

    GLuint gl_type = 0;

    ptrs[0] = "#version 330 core\n";

    switch (type)
    {
    case ShaderTypes::Vertex:
        gl_type = GL_VERTEX_SHADER;
        ptrs[1] = "#define VERTEX_SHADER\n";
        break;
    case ShaderTypes::Fragment:
        gl_type = GL_FRAGMENT_SHADER;
        ptrs[1] = "#define FRAGMENT_SHADER\n";
        break;
    }

    size_t idx = 2;

    for (auto it : defines)
    {
        definesText[idx - 2] = std::format("#define {0}\n", it);
        ptrs[idx]            = definesText[idx - 2].c_str();
        idx++;
    }

    ptrs[idx] = source.c_str();

    GLuint result = glCreateShader(gl_type);
    glShaderSource(result, num_blocks, ptrs, NULL);
    glCompileShader(result);

    int success;
    char infoLog[512];
    glGetShaderiv(result, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(result, 512, NULL, infoLog);
    }

    delete[] ptrs;
    delete[] definesText;

    return result;
}

ShaderProgram::ShaderProgram()
{
    m_uiProgramId = glCreateProgram();
}

ShaderProgram::ShaderProgram(std::string fileName, std::list<const char *> defs)
{
    m_Defines  = defs;
    m_FileName = std::string(fileName);

    LoadAndParseShader();

    m_Hash = CalculateHash(m_FileName, m_Defines);
}

void ShaderProgram::LoadAndParseShader()
{
    m_uiProgramId = glCreateProgram();

    FileData *fd = FileSystem::Instance()->LoadFile(m_FileName);

    std::string src = PreprocessIncludes(fd);

    m_uiVertexShader   = MakeShader(src, ShaderTypes::Vertex, m_Defines);
    m_uiFragmentShader = MakeShader(src, ShaderTypes::Fragment, m_Defines);

    LinkProgram();
    ParseProgramUniforms();

    fd->UnRef();
}

ShaderProgram::~ShaderProgram()
{
    if (m_uiVertexShader)
        glDeleteShader(m_uiVertexShader);
    if (m_uiGeometryShader)
        glDeleteShader(m_uiGeometryShader);
    if (m_uiFragmentShader)
        glDeleteShader(m_uiFragmentShader);

    glDeleteProgram(m_uiProgramId);

    m_Defines.clear();
    ClearPointersVector(m_vecUniforms);
}

size_t ShaderProgram::Hash()
{
    return m_Hash;
}

bool ShaderProgram::AttachVertexShader(const char *fileName)
{
    m_uiVertexShader = MakeShader(fileName, GL_VERTEX_SHADER);
    return m_uiVertexShader != 0;
}

bool ShaderProgram::AttachFragmentShader(const char *fileName)
{
    m_uiFragmentShader = MakeShader(fileName, GL_FRAGMENT_SHADER);
    return m_uiFragmentShader != 0;
}

bool ShaderProgram::AttachGeometryShader(const char *fileName)
{
    // m_uiGeometryShader = MakeShader(fileName, GL_GEOMETRY_SHADER);
    return m_uiGeometryShader != 0;
}

void ShaderProgram::LinkProgram()
{
    if (m_uiVertexShader)
        glAttachShader(m_uiProgramId, m_uiVertexShader);
    if (m_uiFragmentShader)
        glAttachShader(m_uiProgramId, m_uiFragmentShader);
    if (m_uiGeometryShader)
        glAttachShader(m_uiProgramId, m_uiGeometryShader);

    glLinkProgram(m_uiProgramId);

    GLint linked;
    char infoLog[512];
    glGetProgramiv(m_uiProgramId, GL_LINK_STATUS, &linked);

    if (!linked)
    {
        glGetProgramInfoLog(m_uiProgramId, 512, NULL, infoLog);
    }
}

void ShaderProgram::Bind() const
{
    glUseProgram(m_uiProgramId);
}

void ShaderProgram::Unbind() const
{
    glUseProgram(0);
}

std::vector<ShaderUniform *> &ShaderProgram::Uniforms()
{
    return m_vecUniforms;
}

ShaderUniform *ShaderProgram::UniformByKind(UniformKind kind)
{
    for (auto it : m_vecUniforms)
    {
        if (it->Kind() == kind)
            return it;
    }

    return nullptr;
}

void ShaderProgram::Reload()
{
    if (m_uiProgramId)
        glDeleteProgram(m_uiProgramId);
    if (m_uiVertexShader)
        glDeleteProgram(m_uiVertexShader);
    if (m_uiFragmentShader)
        glDeleteProgram(m_uiFragmentShader);

    for (auto it : m_vecUniforms)
        delete it;

    m_vecUniforms.clear();

    LoadAndParseShader();
}

size_t ShaderProgram::CalculateHash(std::string &fileName, std::list<const char *> &defs)
{
    std::size_t nameHash = std::hash<std::string>{}(fileName);

    for (auto & it: defs)
    {
        std::string_view view = std::string_view(it);
        std::size_t defHash = std::hash<std::string_view>{}(view);

        boost::hash_combine(nameHash, defHash);
    }

    return nameHash;
}

ShaderUniform::ShaderUniform(uniformDecl_t *decl, GLuint location)
{
    m_pDecl    = decl;
    m_Location = location;    
}

ShaderUniform::~ShaderUniform()
{
}

void ShaderUniform::SetFloat(float newVal)
{
    assert(m_pDecl->datatype == UniformDataType::Float);

    // if (m_ValueCached.valFloat != newVal)
    {
        m_ValueCached.valFloat.x = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetFloat2(glm::vec2 newVal)
{
    assert(m_pDecl->datatype == UniformDataType::FloatVec2);

    // if (m_ValueCached.valFloat2 != newVal)
    {
        m_ValueCached.valFloat2 = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetFloat3(glm::vec3 newVal)
{
    assert(m_pDecl->datatype == UniformDataType::FloatVec3);

    // if (m_ValueCached.valFloat3 != newVal)
    {
        m_ValueCached.valFloat3 = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetFloat4(glm::vec4 newVal)
{
    assert(m_pDecl->datatype == UniformDataType::FloatVec4);

    // if (m_ValueCached.valFloat4 != newVal)
    {
        m_ValueCached.valFloat4 = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetInt(int newVal)
{
    assert(m_pDecl->datatype == UniformDataType::Int);

    // if (m_ValueCached.valInt != newVal)
    {
        m_ValueCached.valInt.x = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetInt2(glm::ivec2 newVal)
{
    assert(m_pDecl->datatype == UniformDataType::IntVec2);

    // if (m_ValueCached.valInt2 != newVal)
    {
        m_ValueCached.valInt2 = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetInt3(glm::ivec3 newVal)
{
    assert(m_pDecl->datatype == UniformDataType::IntVec3);

    // if (m_ValueCached.valInt3 != newVal)
    {
        m_ValueCached.valInt3 = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetInt4(glm::ivec4 newVal)
{
    assert(m_pDecl->datatype == UniformDataType::IntVec4);

    // if (m_ValueCached.valInt4 != newVal)
    {
        m_ValueCached.valInt4 = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetBool(glm::bvec1 newVal)
{
    assert(m_pDecl->datatype == UniformDataType::Bool);

    // if (m_ValueCached.valBool != newVal)
    {
        m_ValueCached.valInt = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetBool2(glm::bvec2 newVal)
{
    assert(m_pDecl->datatype == UniformDataType::Bool2);

    // if (m_ValueCached.valBoo2 != newVal)
    {
        m_ValueCached.valInt2 = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetBool3(glm::bvec3 newVal)
{
    assert(m_pDecl->datatype == UniformDataType::Bool3);

    // if (m_ValueCached.valBoo3 != newVal)
    {
        m_ValueCached.valInt3 = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetBool4(glm::bvec4 newVal)
{
    assert(m_pDecl->datatype == UniformDataType::Bool4);

    // if (m_ValueCached.valBoo4 != newVal)
    {
        m_ValueCached.valInt4 = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetMat2(glm::mat2 newVal)
{
    assert(m_pDecl->datatype == UniformDataType::FloatMat2);

    // if (m_ValueCached.valMat2 != newVal)
    {
        m_ValueCached.valMat2 = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetMat3(glm::mat3 newVal)
{
    assert(m_pDecl->datatype == UniformDataType::FloatMat3);

    // if (m_ValueCached.valMat3 != newVal)
    {
        m_ValueCached.valMat3 = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetMat4(glm::mat4 newVal)
{
    assert(m_pDecl->datatype == UniformDataType::FloatMat4);

    // if (m_ValueCached.valMat4 != newVal)
    {
        m_ValueCached.valMat4 = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetSampler1D(int newVal)
{
    assert(m_pDecl->datatype == UniformDataType::Sampler1D);

    // if (m_ValueCached.valSampler1D != newVal)
    {
        m_ValueCached.valSampler1D = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetSampler2D(int newVal)
{
    assert(m_pDecl->datatype == UniformDataType::Sampler2D);

    // if (m_ValueCached.valSampler2D != newVal)
    {
        m_ValueCached.valSampler2D = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetSampler3D(int newVal)
{
    assert(m_pDecl->datatype == UniformDataType::Sampler3D);

    // if (m_ValueCached.valSampler3D != newVal)
    {
        m_ValueCached.valSampler3D = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetSamplerCube(int newVal)
{
    assert(m_pDecl->datatype == UniformDataType::SamplerCube);

    // if (m_ValueCached.valSamplerCube != newVal)
    {
        m_ValueCached.valSamplerCube = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetSampler1DShadow(int newVal)
{
    assert(m_pDecl->datatype == UniformDataType::Sampler1DShadow);

    // if (m_ValueCached.valSampler1DShadow != newVal)
    {
        m_ValueCached.valSampler1DShadow = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetSampler2DShadow(int newVal)
{
    assert(m_pDecl->datatype == UniformDataType::Sampler2DShadow);

    // if (m_ValueCached.valSampler2DShadow != newVal)
    {
        m_ValueCached.valSampler2DShadow = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetSampler2DRect(int newVal)
{
    assert(m_pDecl->datatype == UniformDataType::Sampler2DRect);

    // if (m_ValueCached.valSampler2DRect != newVal)
    {
        m_ValueCached.valSampler2DRect = newVal;
        UpdateUniformValue();
    }
}

void ShaderUniform::SetSampler2DRectShadow(int newVal)
{
    assert(m_pDecl->datatype == UniformDataType::Sampler2DRectShadow);

    // if (m_ValueCached.valSampler2DRectShadow != newVal)
    {
        m_ValueCached.valSampler2DRectShadow = newVal;
        UpdateUniformValue();
    }
}

UniformKind ShaderUniform::Kind()
{
    return m_pDecl->kind;
}

void ShaderUniform::UpdateUniformValue()
{
    switch (m_pDecl->datatype)
    {
    case UniformDataType::Float:
        glUniform1f(m_Location, m_ValueCached.valFloat.x);
        break;
    case UniformDataType::FloatVec2:
        glUniform2fv(m_Location, 1, &m_ValueCached.valFloat2.x);
        break;
    case UniformDataType::FloatVec3:
        glUniform3fv(m_Location, 1, &m_ValueCached.valFloat3.x);
        break;
    case UniformDataType::FloatVec4:
        glUniform4fv(m_Location, 1, &m_ValueCached.valFloat4.x);
        break;
    case UniformDataType::Int:
    case UniformDataType::Bool:
        glUniform1i(m_Location, m_ValueCached.valInt.x);
        break;
    case UniformDataType::IntVec2:
    case UniformDataType::Bool2:
        glUniform2iv(m_Location, 1, &m_ValueCached.valInt2.x);
        break;
    case UniformDataType::IntVec3:
    case UniformDataType::Bool3:
        glUniform3iv(m_Location, 1, &m_ValueCached.valInt3.x);
        break;
    case UniformDataType::IntVec4:
    case UniformDataType::Bool4:
        glUniform4iv(m_Location, 1, &m_ValueCached.valInt4.x);
        break;
    case UniformDataType::FloatMat2:
        glUniformMatrix2fv(m_Location, 1, GL_FALSE, &m_ValueCached.valMat2[0][0]);
        break;
    case UniformDataType::FloatMat3:
        glUniformMatrix3fv(m_Location, 1, GL_FALSE, &m_ValueCached.valMat3[0][0]);
        break;
    case UniformDataType::FloatMat4:
        glUniformMatrix4fv(m_Location, 1, GL_FALSE, &m_ValueCached.valMat4[0][0]);
        break;
    case UniformDataType::Sampler1D:
        glUniform1i(m_Location, m_ValueCached.valSampler1D);
        break;
    case UniformDataType::Sampler2D:
        glUniform1i(m_Location, m_ValueCached.valSampler2D);
        break;
    case UniformDataType::Sampler3D:
        glUniform1i(m_Location, m_ValueCached.valSampler3D);
        break;
    case UniformDataType::SamplerCube:
        glUniform1i(m_Location, m_ValueCached.valSamplerCube);
        break;
    case UniformDataType::Sampler1DShadow:
        glUniform1i(m_Location, m_ValueCached.valSampler1DShadow);
        break;
    case UniformDataType::Sampler2DShadow:
        glUniform1i(m_Location, m_ValueCached.valSampler2DShadow);
        break;
    case UniformDataType::Sampler2DRect:
        glUniform1i(m_Location, m_ValueCached.valSampler2DRect);
        break;
    case UniformDataType::Sampler2DRectShadow:
        glUniform1i(m_Location, m_ValueCached.valSampler2DRectShadow);
        break;
    default:
        break;
    }
}
