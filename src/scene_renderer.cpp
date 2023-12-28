/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/

#include "scene_renderer.h"
#include "..\include\ImGuizmo\ImGuizmo.h"
#include "application.h"
#include "common.h"
#include "common_resources.h"
#include "draw_utils.h"
#include "properties_editor.h"
#include "r_camera.h"
#include "r_editor_grid.h"
#include "scene.h"
#include "selection_3d.h"
#include <unordered_set>

#define FAST_BB

SceneRenderer::SceneRenderer(MainWindow *pTargetWindow)
{
    m_pTargetWindow = pTargetWindow;
    m_pScene        = nullptr;

    m_pUnitBoundingBox = DrawUtils::MakeWireframeBox(glm::vec3(1, 1, 1));
    m_pIntensitySphere = DrawUtils::MakeIcosphere(2);
    m_pSpotlightCone   = DrawUtils::MakeWireframeCone();
    m_pSolidCube       = DrawUtils::MakeCube(1);

    SetupBuildboardsRenderer();
    RegisterRendermodesCommands();

    GridRenderer::Instance()->Init();

    auto pers     = Application::Instance()->GetPersistentStorage();
    m_pShowGround = pers->GetSetting(ApplicationSettings::ShowGround);

    m_pSceneShader = GLBackend::Instance()->QueryShader("res/glprogs/scene_uber.glsl", {});
}

void SceneRenderer::SetupBuildboardsRenderer()
{
    float s = 1;

    m_pBillBoard = new DrawMesh();

    // TODO: try triangle strip

    m_pBillBoard->Begin(GL_TRIANGLES);

    m_pBillBoard->TexCoord2f(0, 0);
    m_pBillBoard->Normal3f(-1, -1, 0);
    m_pBillBoard->Vertex2f(0, 0); // 0 Left Up

    m_pBillBoard->TexCoord2f(1, 0);
    m_pBillBoard->Normal3f(1, -1, 0);
    m_pBillBoard->Vertex2f(0, 0); // 1 Right Up

    m_pBillBoard->TexCoord2f(0, 1);
    m_pBillBoard->Normal3f(-1, 1, 0);
    m_pBillBoard->Vertex2f(0, 0); // 2 Left Down

    m_pBillBoard->TexCoord2f(1, 1);
    m_pBillBoard->Normal3f(1, 1, 0);
    m_pBillBoard->Vertex2f(0, 0); // 3 Right Down

    int indicies[] = {0, 1, 2, 1, 3, 2};
    m_pBillBoard->Element1iv(indicies, ARRAYSIZE(indicies));

    // 012 - 132

    m_pBillBoard->End();

    m_pBillBoardsShader = GLBackend::Instance()->QueryShader("res/glprogs/billboard.glsl", {});
}

void SceneRenderer::RegisterRendermodesCommands()
{
    Application::CommandsRegistry()->RegisterCommand(
        new CCommand(GlobalCommands::DumpLightmapMesh, "Dump lightmap mesh", 0, 0, 0, [&]() { DumpLightmapMesh(); }));

    Application::CommandsRegistry()->RegisterCommand(
        new CCommand(GlobalCommands::DumpLightmapUV, "Dump lightmap uv", 0, 0, 0, [&]() { DumpLightmapUV(); }));

    Application::CommandsRegistry()->RegisterCommand(
        new CCommand(GlobalCommands::ReloadAllShaders, "Reload all shaders", 0, 0, 0,
                     [&]() { GLBackend::Instance()->ReloadAllShaders(); }));
}

SceneRenderer::~SceneRenderer()
{
    // delete m_pCamera;
    delete m_pScene;
    delete m_pUnitBoundingBox;

    delete SelectionManager::Instance();
    delete GridRenderer::Instance();
    ;
    delete m_pBillBoard;
    delete m_pSolidCube;
}

void SceneRenderer::RenderScene(Viewport *pViewport)
{
    GLBackend::SetBlending(false);

    m_pCamera    = pViewport->GetCamera();
    m_RenderMode = pViewport->GetRenderMode();

    ResetTransparentChain();

    Application::GetMainWindow()->ClearBackground();

    // Render visible stuff

    if (m_pScene)
    {
        m_pSceneShader->Bind();

        for (auto &it : m_pSceneShader->Uniforms())
        {
            switch (it->Kind())
            {
            case UniformKind::ProjectionMatrix:
                it->SetMat4(m_pCamera->GetProjectionMatrix());
                break;
            case UniformKind::ModelViewMatrix:
                it->SetMat4(m_pCamera->GetViewMatrix());
                break;
            case UniformKind::Scale:
                it->SetFloat3({1.0f, 1.0f, 1.0f});
                break;
            case UniformKind::RenderMode:
                it->SetInt((int)m_RenderMode);
                break;
            default:
                break;
            }
        }

        m_pScene->RenderEntities(m_RenderMode, this);
    }

    RenderTransparentChain();

    if (m_pShowGround->GetAsBool())
    {
        GridRenderer::Instance()->Render();
    }
}

void SceneRenderer::ResetTransparentChain()
{
    m_TransparentEntitiesChain.Reset(m_pCamera->GetOrigin());
}

void SceneRenderer::RenderHelperGeometry()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    SceneEntityWeakPtr selection;

#ifdef FAST_BB
    m_pBillBoardsShader->Bind();
    m_pBillBoard->Bind();

    for (auto &it : m_pBillBoardsShader->Uniforms())
    {
        switch (it->Kind())
        {
        case UniformKind::Color:
        case UniformKind::TransformMatrix:
        case UniformKind::Scale:
        case UniformKind::Diffuse:
            break;
        default:
            GLBackend::SetUniformValue(it);
            break;
        }
    }

#else
    m_pBillBoard->Unbind();

    glDisable(GL_CULL_FACE);
    glUseProgram(0);
#endif

    for (auto &it : m_pScene->GetLightDefs())
    {
        if (!it)
            return;

        if (!it->IsLightEntity())
            continue;

        DrawBillboard(it->GetPosition(), glm::vec2(8, 8), it->GetEditorIcon(), it->GetRenderColor(),
                      it->GetSerialNumber());

        if (it->IsSelected())
        {
            selection = it;
        }
    }

    if (selection.lock() && false)
    {
        DrawLightHelperGeometry(selection);
    }

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

int SceneRenderer::HandleEvent(bool bWasHandled, SDL_Event &e, float flFrameDelta)
{
    return 0;
}

float SceneRenderer::FrameDelta()
{
    return m_pTargetWindow->FrameDelta();
}

void SceneRenderer::LoadModel(const char *dropped_filedir, int loadFlags)
{
    if (!m_pScene)
        m_pScene = new Scene;

    m_pScene->LoadLevel(dropped_filedir, loadFlags);

    if (dropped_filedir)
        Application::GetPersistentStorage()->PushMRUFile(dropped_filedir);

    auto fileName = FileSystem::Instance()->ExtractFileName(dropped_filedir);

    Application::GetMainWindow()->SetTitle(fileName);
}

void SceneRenderer::DrawBillboard(const glm::vec3 pos, const glm::vec2 size, const GLTexture *texture,
                                  const glm::vec3 tint, const uint32_t objectSerialNumber)
{
    m_pBillBoardsShader->Bind();
    m_pBillBoard->Bind();

    // TODO: cache this and make meaningfull api
    GLBackend::SetBlending(true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (auto &it : m_pBillBoardsShader->Uniforms())
    {
        switch (it->Kind())
        {
        case UniformKind::Color:
        case UniformKind::TransformMatrix:
        case UniformKind::Scale:
        case UniformKind::Diffuse:
        case UniformKind::ObjectSerialNumber:
            break;
        default:
            GLBackend::SetUniformValue(it);
            break;
        }
    }

    glm::mat4 matTransform = glm::translate(glm::mat4(1), pos);

    for (auto &it : m_pBillBoardsShader->Uniforms())
    {
        switch (it->Kind())
        {
        case UniformKind::Color:
            it->SetFloat4({tint.xyz, 1});
            break;
        case UniformKind::TransformMatrix:
            it->SetMat4(matTransform);
            break;
        case UniformKind::Scale:
            it->SetFloat3({size.xyy});
            break;
        case UniformKind::ObjectSerialNumber:
            it->SetInt(objectSerialNumber);
            break;
        }
    }

    GLBackend::BindTexture(0, texture);
    GLBackend::BindTexture(1, nullptr);
    m_pBillBoard->Draw();
}

Camera *SceneRenderer::GetCamera()
{
    return m_pCamera;
}

void SceneRenderer::ReloadScene(int loadFlags)
{
    assert(m_pScene);
    m_pScene->Reload(loadFlags);
}

void SceneRenderer::Debug_DrawGround()
{
}

void SceneRenderer::DrawLightHelperGeometry(SceneEntityWeakPtr pObject)
{
    auto _ptr = pObject.lock();

    if (!_ptr)
        return;

    lightDefPtr_t ptr = std::dynamic_pointer_cast<Lb3kLightEntity>(_ptr);

    switch (ptr->type)
    {
    case LightTypes::Omni: {
        auto shader = GLBackend::Instance()->SolidColorGeometryShader();

        shader->Bind();

        for (auto &it : shader->Uniforms())
        {
            switch (it->Kind())
            {
            case UniformKind::Color:
                it->SetFloat4(glm::vec4(1, 1, 1, 1) - ptr->GetRenderColor());
                break;
            case UniformKind::TransformMatrix: {
                glm::mat4x4 mat = glm::translate(glm::mat4x4(1.f), ptr->GetPosition());
                it->SetMat4(mat);
            }
            break;
            default:
                GLBackend::SetUniformValue(it);
                break;
            }
        }

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
        // m_pIntensitySphere->BindAndDraw();

        m_pSolidCube->BindAndDraw();

        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        shader->Unbind();
    }
    break;
    case LightTypes::Spot: {
        auto shader = GLBackend::Instance()->SpotlightConeShader();

        shader->Bind();
        shader->SetDefaultCamera();
        shader->SetColor(glm::vec4(1, 1, 1, 1) - ptr->GetRenderColor());

        glm::mat4x4 mat = glm::translate(glm::mat4x4(1.f), ptr->GetPosition());

        shader->SetScale(1);
        shader->SetConeHeight(ptr->intensity);
        shader->SetConeAngleDegrees(ptr->cones[1]);
        shader->SetTransform(mat);

        auto mat1 = glm::rotate(glm::mat4x4(1), glm::radians(ptr->anglesDirection[0]), glm::vec3(1, 0, 0));
        auto mat2 = glm::rotate(glm::mat4x4(1), glm::radians(ptr->anglesDirection[1]), glm::vec3(0, 1, 0));
        auto mat3 = glm::rotate(glm::mat4x4(1), glm::radians(ptr->anglesDirection[2]), glm::vec3(0, 0, 1));

        // 		mat *= mat1;
        // 		mat *= mat2;
        // 		mat *= mat3;

        // Quake-ish order
        mat *= mat3;
        mat *= mat2;
        mat *= mat1;

        shader->SetTransform(mat);
        m_pSpotlightCone->BindAndDraw();

        shader->Unbind();
    }
    break;
    case LightTypes::Direct:
        break;
    default:
        break;
    }
}

RenderMode SceneRenderer::GetRenderMode()
{
    return m_RenderMode;
}

void SceneRenderer::DumpLightmapMesh()
{
    m_pScene->DumpLightmapMesh();
}

void SceneRenderer::DumpLightmapUV()
{
    m_pScene->DumpLightmapUV();
}

void SceneRenderer::RenderTransparentChain()
{
    BT_PROFILE("SceneRenderer::RenderTransparentChain()");

    SceneEntityWeakPtr ptr = m_TransparentEntitiesChain.Start();

    size_t chainLength = 0;

    while (!ptr.expired())
    {
        auto lockPtr = ptr.lock();

        lockPtr->Render(m_RenderMode, nullptr);

        chainLength++;
        ptr = lockPtr->Next();
    }
}

void SceneRenderer::SetEntityTransform(SceneEntityPtr &it)
{
    auto uniform = m_pSceneShader->UniformByKind(UniformKind::TransformMatrix);
    if (uniform)
        uniform->SetMat4(it->GetTransform());
}

void SceneRenderer::RenderPointEntityDefault(const glm::vec3 &m_Position, const glm::vec3 &m_Mins,
                                             const glm::vec3 &m_Maxs, const glm::vec3 &m_Color,
                                             const uint32_t objectSerialNumber)
{
    glm::vec3 scale  = m_Maxs - m_Mins;
    auto      shader = GLBackend::Instance()->SolidColorGeometryShader();

    shader->Bind();

    for (auto &it : shader->Uniforms())
    {
        switch (it->Kind())
        {
        case UniformKind::Color:
            it->SetFloat4({m_Color, 1});
            break;
        case UniformKind::TransformMatrix: {
            glm::vec3 offset = m_Mins + scale * 0.5f;

            glm::mat4x4 mat = glm::translate(glm::mat4x4(1.f), m_Position + offset);
            it->SetMat4(mat);
        }
        break;
        case UniformKind::Scale:
            it->SetFloat3(scale.xyz);
            break;
        case UniformKind::ObjectSerialNumber:
            it->SetInt(objectSerialNumber);
            break;
        default:
            GLBackend::SetUniformValue(it);
            break;
        }
    }

    m_pSolidCube->BindAndDraw();

    shader->Unbind();
}

glm::vec3 SceneRenderer::GetRenderPos()
{
    if (m_pCamera)
        return m_pCamera->GetOrigin();
    else
        return {0, 0, 0};
}

void SceneRenderer::AddTransparentEntity(SceneEntityWeakPtr pEntity)
{
    m_TransparentEntitiesChain.AddDistanceSorted(pEntity);
}

void SceneRenderer::SetRenderMode(RenderMode newMode)
{
    m_RenderMode = newMode;
}

void SceneRenderer::RenderGenericEntity(SceneEntity *pEntity)
{
    auto shader = GLBackend::Instance()->SolidColorGeometryShader();

    shader->Bind();

    for (auto &it : shader->Uniforms())
    {
        switch (it->Kind())
        {
        case UniformKind::Color: {
            it->SetFloat4(pEntity->GetRenderColor());
        }
        break;
        case UniformKind::TransformMatrix: {
            glm::mat4x4 mat = glm::translate(glm::mat4x4(1.f), pEntity->GetPosition());
            it->SetMat4(mat);
        }
        break;
        case UniformKind::Scale: {
            it->SetFloat3({16.f, 16.f, 16.f});
        }
        break;
        case UniformKind::ObjectSerialNumber:
            it->SetInt(pEntity->GetSerialNumber());
            break;
        default:
            GLBackend::SetUniformValue(it);
            break;
        }

        m_pUnitBoundingBox->BindAndDraw();
        shader->Unbind();
    }
}

void SceneRenderer::FocusCameraOnObject(SceneEntityPtr it)
{
    m_pCamera->LookAtPoint(it->GetPosition());
}

Scene *SceneRenderer::GetScene()
{
    if (!m_pScene)
        m_pScene = new Scene();

    return m_pScene;
}

glm::vec3 SceneRenderer::GetNewLightPos()
{
    return m_pCamera->GetOrigin() + m_pCamera->GetForwardVector() * 10.f;
}
