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

bool g_useBVH = true;

uberShaderDefs_t g_UberShaderTable[] = {
    // clang-format off
    {ModelType::StaticLightmapped , RenderMode::Lightshaded , {"STATIC_GEOMETRY" , "DIFFUSE"      , "LIGHTMAP"} , nullptr},
    {ModelType::StudioV10         , RenderMode::Lightshaded , {"USING_BONES"     , "DIFFUSE"}     , nullptr}    ,
    {ModelType::StudioV10         , RenderMode::Unshaded    , {"USING_BONES"     , "DIFFUSE"}     , nullptr}    ,
    {ModelType::StudioV10         , RenderMode::Flatshaded  , {"USING_BONES"     , "FLATSHADED"}  , nullptr}    ,    
    {ModelType::StaticLightmapped , RenderMode::Unshaded    , {"STATIC_GEOMETRY" , "DIFFUSE"}     , nullptr}    ,
    {ModelType::StaticLightmapped , RenderMode::Flatshaded  , {"STATIC_GEOMETRY" , "FLATSHADED"}  , nullptr}    ,
    {ModelType::Sprite            , RenderMode::Lightshaded , {"SPRITE"          ,"SOLID_COLOR"   , "DIFFUSE"}  , nullptr},
    {ModelType::Sprite            , RenderMode::Unshaded    , {"SPRITE"          ,"SOLID_COLOR"   , "DIFFUSE"}  , nullptr},
    {ModelType::Sprite            , RenderMode::Flatshaded  , {"SPRITE"          ,"SOLID_COLOR"   , "DIFFUSE"}  , nullptr},
    {ModelType::HelperPrimitive   , RenderMode::Lightshaded , {"STATIC_GEOMETRY" , "SOLID_COLOR"} , nullptr}    ,
    {ModelType::HelperPrimitive   , RenderMode::Unshaded    , {"STATIC_GEOMETRY" , "SOLID_COLOR"} , nullptr}    ,
    {ModelType::HelperPrimitive   , RenderMode::Flatshaded  , {"STATIC_GEOMETRY" , "SOLID_COLOR"} , nullptr}    ,
    // clang-format on
};

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

    m_pStudioShader = GLBackend::Instance()->QueryShader("res/glprogs/studio.glsl", {"USING_BONES"});

    Con_Printf("==== Building uber shader permutations ====\n");

    for (auto &it : g_UberShaderTable)
    {
        Con_Printf("Building permutation: ");
        for (auto & it: it.defines)
            Con_Printf(" %s", it);

        Con_Printf("\n");

        it.shader = GLBackend::Instance()->QueryShader("res/glprogs/scene_geometry.glsl", it.defines);
    }

    Con_Printf("==== Uber shader permutations done ====\n");
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
    GL_CheckForErrors();

    ResetTransparentChain();
    GL_CheckForErrors();

    Application::GetMainWindow()->ClearBackground(true);
    GL_CheckForErrors();

    // Render visible stuff

    // ObjectPropertiesEditor::Instance()->Render3DBoundingBox();

    // RenderPointEntityDefault(bbox.Center(), bboxRel.Mins(), bboxRel.Maxs(), {1, 0, 0}, 0);

    if (m_pScene)
    {
        if (m_RenderMode == RenderMode::Wireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        renderStats_s *stats = GLBackend::Instance()->RenderStats();

        stats->nEntitiesTotal    = m_pScene->TotalEntities();
        stats->nEntitiesRendered = 0;

        SortRenderLists();
        GL_CheckForErrors();

        RenderEntitiesChain(m_vSortedSolidEntities.data(), m_vSortedSolidEntities.size(), false);
        GL_CheckForErrors();

        GLBackend::SetBlending(true, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        GL_CheckForErrors();

        RenderEntitiesChain(m_vSortedTransparentEntities.data(), m_vSortedTransparentEntities.size(), true);
        GL_CheckForErrors();

        GLBackend::SetBlending(false);
        GL_CheckForErrors();

        if (m_RenderMode == RenderMode::Wireframe)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            GL_CheckForErrors();
        }


        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
        
        if (m_pScene)
            m_pScene->DebugRenderBVH();
        // ObjectPropertiesEditor::Instance()->RenderDebugOctree();
        
        

         glEnable(GL_CULL_FACE);
         glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    
    

    if (m_pShowGround->GetAsBool())
    {
        GridRenderer::Instance()->Render();
        GL_CheckForErrors();
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
    //glUseProgram(0);
#endif

//     for (auto &it : m_pScene->GetLightDefs())
//     {
//         //         if (!it)
//         //             return;
//         //
//         //         if (!it->IsLightEntity())
//         //             continue;
//         //
//         //         DrawBillboard(it->GetPosition(), glm::vec2(8, 8), it->GetEditorIcon(), it->GetRenderColor(),
//         //                       it->GetSerialNumber());
//         //
//         //         if (it->IsSelected())
//         //         {
//         //             selection = it;
//         //         }
//     }

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

void SceneRenderer::SortRenderLists()
{
    BT_PROFILE("SceneRenderer::SortRenderLists()");

    auto frustum = GetCamera()->GetFrustum();

    m_vSortedSolidEntities.clear();
    m_vSortedTransparentEntities.clear();

    auto &ents = m_pScene->GetEntities();

    glm::vec3 eyesPos = m_pCamera->GetOrigin();

    if (g_useBVH)
    {
        BVHTree *pSceneTree = m_pScene->GetBVHTree();
        FillSortListsBVH(pSceneTree, pSceneTree->GetRootNode(), frustum, eyesPos);
    }
    else
        FillSortListsLinear(ents, frustum, eyesPos);

    


    std::sort(m_vSortedSolidEntities.begin(), m_vSortedSolidEntities.end(),
              [](sSortInfo &a, sSortInfo &b) -> bool { return a.modType > b.modType; });

    std::sort(m_vSortedTransparentEntities.begin(), m_vSortedTransparentEntities.end(),
              [](sSortInfo &a, sSortInfo &b) -> bool {
                  return a.modType > b.modType && a.renderDistance > b.renderDistance;
              });
}

void SceneRenderer::FillSortListsBVH(BVHTree *pTree, BVHNode *pNode, Frustum *pFrustum, glm::vec3 eyesPos,
                                     FrustumVisiblity parentVisiblity)
{
    BT_PROFILE("SceneRenderer::FillSortListsBVH");

    if (pNode == nullptr)
        return;

    FrustumVisiblity myVisilibity = parentVisiblity;

    if (1)
    {
        if (parentVisiblity != FrustumVisiblity::Complete)
        {
            myVisilibity = pFrustum->CullBoxEx(pNode->aabb);

            if (myVisilibity == FrustumVisiblity::None)
                return;
        }
    }
    else
    {
        if (pFrustum->CullBox(pNode->aabb))
            return;
    }


    if (pNode->IsLeaf())
    {
        auto & it    = pNode->entity;        
        auto model = it->GetModel();

        if (model.expired())
            return;

        auto ptr = model.lock();

        if (ptr->IsTransparent())
        {
            float dist = glm::length2(it->GetPosition() - eyesPos);
            m_vSortedTransparentEntities.push_back({it.get(), ptr->GetType(), dist});
        }
        else
            m_vSortedSolidEntities.push_back({it.get(), ptr->GetType(), 0});
    }
    else
    {        
        if (pNode->left != -1)
            FillSortListsBVH(pTree, pTree->GetNodePtr(pNode->left), pFrustum, eyesPos, myVisilibity);

        if (pNode->right != -1)
            FillSortListsBVH(pTree, pTree->GetNodePtr(pNode->right), pFrustum, eyesPos, myVisilibity);
    }

}

void SceneRenderer::FillSortListsLinear(const std::list<SceneEntityPtr> &ents, Frustum *frustum, glm::vec3 eyesPos)
{
    BT_PROFILE("SceneRenderer::FillSortListsLinear");

    for (auto &it : ents)
    {
        if (!it)
            continue;

        if (!it->IsDataLoaded())
            continue;

        // assert(it->GetOctreeNode() != nullptr);

        if (frustum->CullBox(it->GetAbsoulteBoundingBox()))
            continue;

        // TODO: encapsulate model acquisition and fall back to box if original is unloaded?
        auto model = it->GetModel();

        if (model.expired())
            continue;

        auto ptr = model.lock();

        if (ptr->IsTransparent())
        {
            float dist = glm::length2(it->GetPosition() - eyesPos);
            m_vSortedTransparentEntities.push_back({it.get(), ptr->GetType(), dist});
        }
        else
            m_vSortedSolidEntities.push_back({it.get(), ptr->GetType(), 0});
    }
}

void SceneRenderer::RenderEntitiesChain(const sSortInfo *pData, const size_t nEntities, bool transparent) const
{
    BT_PROFILE(transparent ? "SceneRenderer::RenderEntitiesChain(transparent == true)"
                           : "SceneRenderer::RenderEntitiesChain(transparent == false)");

    renderStats_s *stats = GLBackend::Instance()->RenderStats();

    ModelType      currentType   = ModelType::Unset;
    ShaderProgram *currentShader = 0;

    for (size_t i = 0; i < nEntities; i++)
    {
        const sSortInfo *info = &pData[i];

        if (info->modType != currentType)
        {
            currentType   = info->modType;
            currentShader = SetupShaderForModelType(currentType);
        }

        stats->nEntitiesRendered++;
        info->pEntity->Render(m_RenderMode, currentShader);
    }
}

ShaderProgram *SceneRenderer::SetupShaderForModelType(const ModelType currentType) const
{
    if (currentType == ModelType::Sprite)
        m_pBillBoard->Bind();

    RenderMode lookupMode = m_RenderMode;

    if (lookupMode == RenderMode::Wireframe)
        lookupMode = RenderMode::Flatshaded;

    for (auto &it : g_UberShaderTable)
    {
        if (it.type == currentType && it.mode == lookupMode)
        {
            it.shader->Bind();

            for (auto &it : it.shader->Uniforms())
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
                    it->SetFloat3({1, 1, 1});
                    break;
                case UniformKind::TransformMatrix:
                    it->SetMat4(glm::mat4(1));
                    break;
                case UniformKind::ObjectSerialNumber:
                    it->SetInt(0);
                    break;
                default:
                    break;
                }
            }

            // glFinish();

            return it.shader;
        }
    }
    
    return nullptr;

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

void SceneRenderer::DrawBillboardMesh()
{
    m_pBillBoard->Draw();
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
