/*
    LightBaker3000 Frontend project,
    (c) 2022-2023 CrazyRussian
*/

#define _CRT_SECURE_NO_WARNINGS

#include "application.h"
#include "model_obj.h"
#include "r_camera.h"
#include "common_resources.h"
#include "gl_backend.h"
#include "mod_obj_asynch_exporter.h"
#include "mod_obj_asynch_loader.h"
#include "mod_obj_atlas_gen.h"
#include "text_utils.h"
#include <algorithm>
#include <intsafe.h>
#include "world_entity.h"

#define WHITE_PNG "res/textures/white.png"
#define DUMMY_PNG "res/textures/dummy.png"

ModelOBJ::ModelOBJ(const char *fileName, Scene *pScene) : SceneEntity(pScene)
{
    m_strModelName = fileName;
    auto fs        = Application::GetFileSystem();

    m_strLMModelPath = fs->MakeTemplatePath(fileName, "{0}/{1}.lm.obj");

    LoadLMMesh();

    ModObjAsynchLoader *loaderTask = new ModObjAsynchLoader(this, fileName);
    loaderTask->Schedule();
    // BuildDrawMesh();
}

void ModelOBJ::LoadLMMesh()
{
    auto fs = Application::GetFileSystem();

    if (fs->FileExists(m_strLMModelPath))
    {
        ModObjAsynchLoader *lmLoaderTask = new ModObjAsynchLoader(this, m_strLMModelPath.c_str());
        lmLoaderTask->SetOnlyLoadUV(true);
        lmLoaderTask->Schedule();
    }
}

ModelOBJ::~ModelOBJ()
{

    // TODO: FIXME
//     for (auto &it : m_ModelData.meshes)
//     {
//         FreeGLTexture(it.diffuse_texture);
// 
//         for (int i = 0; i < MAX_LIGHT_STYLES; i++)
//             FreeGLTexture(it.lightmap_texture[i]);
//     }

    FreeVector(m_ModelData.faces);
    FreeVector(m_ModelData.normals);
    FreeVector(m_ModelData.uvs);
    FreeVector(m_ModelData.verts);
    FreeVector(m_ModelData.lightDefs);
    FreeVector(m_ModelData.groups);
}

void ModelOBJ::DrawDebug()
{
    mesh.Bind();

    glEnable(GL_TEXTURE_2D);

    for (auto &it : m_ModelData.meshes)
    {
        if (it.diffuse_texture)
            GLBackend::BindTexture(0, it.diffuse_texture);
        else
            GLBackend::BindTexture(0, nullptr);

        mesh.Draw((uint32_t)it.first_face, (uint32_t)it.num_faces);
    }

    mesh.Unbind();
}

void ModelOBJ::ClearLightDefinitions()
{
    m_ModelData.lightDefs.clear();
}

void ModelOBJ::AddLight(lightDefPtr_t &it)
{
    m_ModelData.lightDefs.push_back(new Lb3kLightEntity(*it));
}

void ModelOBJ::SetLightmapDimensions(int w, int h)
{
    m_ModelData.lightmapDimensions[0] = w;
    m_ModelData.lightmapDimensions[1] = h;
}

void ModelOBJ::BuildDrawMesh()
{
    if (m_ModelData.faces.size() == 0)
        return;

    if (m_ModelData.verts.size() == 0)
        return;

    ValidateLightmap();

    // Sort faces by mesh_id

    std::qsort(m_ModelData.faces.data(), m_ModelData.faces.size() / 3, sizeof(mobjface_t) * 3,
               [](const void *pa, const void *pb) -> int {
                   const mobjface_t *faceA = static_cast<const mobjface_t *>(pa);
                   const mobjface_t *faceB = static_cast<const mobjface_t *>(pb);

                   if (faceA->materialId == faceB->materialId)
                       return 0;

                   else if (faceA->materialId > faceB->materialId)
                       return 1;

                   else if (faceA->materialId < faceB->materialId)
                       return -1;

                   return 0;
               });

    bool bNoTextures = false;

    size_t currentMaterial = 0;
    mobjmesh_t *pMaterial  = nullptr;

    currentMaterial = m_ModelData.faces[0].materialId;
    pMaterial       = &m_ModelData.meshes[currentMaterial];

    if (pMaterial->lightmap_texture[0])
        bNoTextures = false;
    else
        bNoTextures = true;

    pMaterial->first_face = mesh.CurrentElement();

    BoundingBox bbox(4);


    mesh.Begin(GL_TRIANGLES);
    mesh.Color4f(1, 1, 1, 1);

    for (auto &face : m_ModelData.faces)
    {
        if (face.materialId != currentMaterial)
        {
            pMaterial->num_faces = mesh.CurrentElement() - pMaterial->first_face;

            // Con_Printf("%d : First face: %d, num faces: %d\n", currentMaterial, pMaterial->first_face,
            // pMaterial->num_faces);

            currentMaterial = face.materialId;
            pMaterial       = &m_ModelData.meshes[currentMaterial];

            pMaterial->first_face = mesh.CurrentElement();

            // assert(pMaterial->material);

            if (pMaterial->lightmap_texture[0])
                bNoTextures = false;
            else
                bNoTextures = true;
        }

        if (m_ModelData.uvs.size() > 0 && face.uv > 0)
        {
            size_t uvOffset = (face.uv - 1) * m_ModelData.uvSize;

            mesh.TexCoord2fv(&m_ModelData.uvs[uvOffset]);

            if (m_hasLMMesh)
            {
                uvOffset    = (face.uv - 1) * m_LightmapModelData.uvSize;
                float *lmUv = &m_LightmapModelData.uvs[uvOffset];
                mesh.Color3f(lmUv[0], lmUv[1], 0);
            }
        }
        else
        {
            float nullUv[2] = {0, 0};
            mesh.TexCoord2fv(nullUv);
        }

        if (m_ModelData.normals.size() > 0 && face.norm > 0)
        {
            float *norm = &m_ModelData.normals[(face.norm - 1) * 3];
            mesh.Normal3fv(norm);
        }
        else
        {
            float nullNorm[3] = {0, 0, 0};
            mesh.Normal3fv(nullNorm);
        }

        float *p = &m_ModelData.verts[(face.vert - 1) * m_ModelData.vertSize];
        // Con_Printf("%f %f %f\n", p[0], p[1], p[2]);

        float *v = &m_ModelData.verts[(face.vert - 1) * m_ModelData.vertSize];
        mesh.Vertex3f(v[0], v[2], v[1]);
        bbox.AddPoint(glm::vec3(v[0], v[2], v[1]));
        
    }

    pMaterial->num_faces = mesh.CurrentElement() - pMaterial->first_face;

    mesh.End();

    SetBoundingBox(bbox);

    IWorldEntity* ent = m_pScene->GetWorldEntity();
    ent->SetBoundingBox(bbox);

    FlagDataLoaded();
}

void ModelOBJ::ValidateLightmap()
{
    auto sc = Application::GetMainWindow()->GetSceneRenderer();

    // Check if lm mesh is valid
    if (m_LightmapModelData.UVCount() != m_ModelData.UVCount())
    {
        m_hasLMMesh = false;
    }

    if (m_hasLMMesh)
    {
        auto mesh    = m_LightmapModelData.meshes[0];
        auto texture = mesh.lightmap_texture[0];

        if (!texture->GLTextureNum())
            m_hasLMMesh = false;
    }

    if (m_hasLMMesh)
        sc->SetRenderMode(RenderMode::Lightshaded);
    else
        sc->SetRenderMode(RenderMode::Unshaded);
}

float ModelOBJ::GetSceneScale()
{
    return m_ModelData.sceneScale;
}

DrawMesh *ModelOBJ::GetDrawMesh()
{
    return &mesh;
}

void ModelOBJ::OnHovered()
{
}

void ModelOBJ::OnMouseMove(glm::vec2 delta)
{
}

void ModelOBJ::OnSelect(ISelectableObjectWeakRef myWeakRef)
{
    // Con_Printf("ModelOBJ::OnSelect()\n");
}

void ModelOBJ::OnUnSelect()
{
}

void ModelOBJ::OnUnhovered()
{
}

void ModelOBJ::Render(RenderMode mode, ShaderProgram* shader)
{
}



void ModelOBJ::CommonDrawGeometryWithShader(const ISceneShader *shader)
{
    mesh.Bind();

    auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();
    auto scene         = sceneRenderer->GetScene();

    shader->Bind();
    shader->SetScale(scene->GetSceneScale());
    shader->SetDefaultCamera();
    shader->SetTransformIdentity();

    glEnable(GL_TEXTURE_2D);

    if (m_hasLMMesh && m_LightmapModelData.flags & FL_DATA_LOADED)
    {
        auto mesh    = m_LightmapModelData.meshes[0];
        auto texture = mesh.lightmap_texture[0];

        GLBackend::BindTexture(1, texture);
    }

    // for (int i = 5 ;  i < m_ModelData.meshes.size() - 1; i++)
    for (auto &it : m_ModelData.meshes)
    {
        // auto it = m_ModelData.meshes[i];

        if (it.num_faces == 0)
        {
            continue;
        }

        if (it.material)
        {
            GLBackend::BindTexture(0, it.material->map_Kd ? it.material->map_Kd : nullptr);
        }
        else
        {
            GLBackend::BindTexture(0, it.diffuse_texture ? it.diffuse_texture : nullptr);
        }

        mesh.Draw((uint32_t)it.first_face, (uint32_t)it.num_faces);

        // break;
    }

    shader->Unbind();
    mesh.Unbind();
}

// void ModelOBJ::RenderUnshaded()
// {
//     auto shader = GLBackend::Instance()->DiffuseSceneShader();
//     CommonDrawGeometryWithShader(shader);
// }
// 
// void ModelOBJ::RenderGroupShaded()
// {
//     auto shader = GLBackend::Instance()->GroupShadedSceneShader();
// 
//     mesh.Bind();
// 
//     auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();
//     auto scene         = sceneRenderer->GetScene();
// 
//     shader->Bind();
//     shader->SetScale(scene->GetSceneScale());
//     shader->SetDefaultCamera();
//     shader->SetTransformIdentity();
// 
//     shader->SetObjectColor(glm::vec4(0, 1, 0, 1));
// 
//     glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
// 
//     // for (int i = 5 ;  i < m_ModelData.meshes.size() - 1; i++)
// 
//     size_t index = 0;
// 
//     for (auto &it : m_ModelData.meshes)
//     {
//         // auto it = m_ModelData.meshes[i];
// 
//         if (it.num_faces == 0)
//         {
//             continue;
//         }
// 
//         int hash = (it.num_faces * it.first_face * index * (int)&it) % 360;
// 
//         float hue = (float)hash / 360.f;
// 
//         glm::vec3 rgb;
// 
//         ImGui::ColorConvertHSVtoRGB(hue, 1, 1, rgb[0], rgb[1], rgb[2]);
// 
//         shader->SetObjectColor(glm::vec4(rgb, 1));
// 
//         mesh.Draw((uint32_t)it.first_face, (uint32_t)it.num_faces);
//         index++;
//     }
// 
//     glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
// 
//     shader->Unbind();
//     mesh.Unbind();
// }

mobjdata_t *ModelOBJ::GetModelData()
{
    return &m_ModelData;
}

mobjdata_t *ModelOBJ::GetLMData()
{
    return &m_LightmapModelData;
}

void ModelOBJ::AddLightsIntoScene()
{
    auto sceneRenderer = Application::Instance()->GetMainWindow()->GetSceneRenderer();
    auto scene         = sceneRenderer->GetScene();

    for (auto def : m_ModelData.lightDefs)
    {
        auto ptr = std::make_shared<Lb3kLightEntity>(*def);

        scene->AddEntityWithSerialNumber(ptr, def->GetSerialNumber());
    }
}

void ModelOBJ::FlagHasLMMesh()
{
    m_hasLMMesh = true;

    // Cleanup
    ReloadLightmapTextures();
    BuildDrawMesh();
}

const char *ModelOBJ::Description()
{
    return "<world - obj model>";
}

void ModelOBJ::PrepareLights()
{
    // TODO: fixme
    //     auto scene  = Application::GetMainWindow()->GetSceneRenderer()->GetScene();
//     auto lights = scene->GetLightDefs();
// 
//     m_ModelData.lightDefs.clear();
//     m_LightmapModelData.lightDefs.clear();
// 
//     for (auto light = lights.begin(); light != lights.end(); light++)
//     {
//         auto lightPointer = std::dynamic_pointer_cast<Lb3kLightEntity>(*light);
// 
//         if (lightPointer)
//         {
//             // Наверное тупой подход
//             Lb3kLightEntity *l1 = new Lb3kLightEntity(*lightPointer);
//             Lb3kLightEntity *l2 = new Lb3kLightEntity(*lightPointer);
// 
//             m_LightmapModelData.lightDefs.push_back(l1);
//             m_ModelData.lightDefs.push_back(l2);
//         }
//     }
}

void ModelOBJ::ReloadTextures()
{
    if (m_hasLMMesh)
        ReloadLightmapTextures();
    else
        LoadLMMesh();
}

void ModelOBJ::ReloadLightmapTextures()
{
  //  for (auto &it : m_LightmapModelData.meshes)
//    {
//         if (it.diffuse_texture)
//             GLReloadTexture(it.diffuse_texture);
//         else
//         {
//             it.diffuse_texture = LoadGLTexture(it.diffuse_texture_path.c_str());
// 
//             if (it.diffuse_texture)
//                 if (it.diffuse_texture->Width() == -1)
//                     it.diffuse_texture = LoadGLTexture(DUMMY_PNG);
//         }
// 
//         for (int i = 0; i < MAX_LIGHT_STYLES; i++)
//         {
//             if (it.lightmap_texture[i])
//                 GLReloadTexture(it.lightmap_texture[i]);
//             else
//             {
//                 it.lightmap_texture[i] = LoadGLTexture(it.lightmap_texture_path[i].c_str());
//             }
//         }
//     }
}

std::string &ModelOBJ::Export(const char *fileName, lightBakerSettings_t *lb3kOptions, glm::vec3 envColor)
{
    if (fileName)
    {
    }
    else
    {
        // TODO: check if nesseceary
        if (!m_hasLMMesh)
        {
            GenerateAtlasTask *tsk = new GenerateAtlasTask(this);
            tsk->TransferNewUV();
            delete tsk;
        }
        else
        {
            UpdateLMMesh();
        }

        // FIXME

        // m_LightmapModelData.envColor =
        m_LightmapModelData.lightmapDimensions[0] = lb3kOptions->m_lmSettings.size[0];
        m_LightmapModelData.lightmapDimensions[1] = lb3kOptions->m_lmSettings.size[1];

        m_LightmapModelData.envColor = envColor;

        PrepareLights();

        ModObjAsynchExporter *exporter = new ModObjAsynchExporter(this, m_strLMModelPath.c_str(), true);
        exporter->ExportSynch();
        delete exporter;

        if (!m_hasLMMesh)
        {
            LoadLMMesh();
        }

        exporter = new ModObjAsynchExporter(this, m_strModelName.c_str(), false);
        exporter->ExportSynch();
        delete exporter;
    }

    return m_strLMModelPath;
}

void ModelOBJ::UpdateLMMesh()
{
    m_LightmapModelData.verts    = m_ModelData.verts;
    m_LightmapModelData.normals  = m_ModelData.normals;
    m_LightmapModelData.faces    = m_ModelData.faces;
    m_LightmapModelData.vertSize = m_ModelData.vertSize;

    m_LightmapModelData.objects.clear();

    for (auto &face : m_LightmapModelData.faces)
        face.groupId = 1;

    m_LightmapModelData.groups = m_ModelData.groups;

    m_LightmapModelData.groups.clear();

    mobjegroup_t grp;
    strncpy_s(grp.name, "default", sizeof(grp.name) - 1);

    grp.object_id    = 0;
    grp.first_vertex = 0;

    m_LightmapModelData.groups.push_back(grp);
    m_LightmapModelData.groups.push_back(grp);
}

std::string ModelOBJ::GetModelFileName()
{
    return m_strModelName;
}

std::string ModelOBJ::GetModelTextureName()
{
    return m_strDiffuseName;
}
