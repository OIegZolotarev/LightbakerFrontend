/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "common.h"
#include "goldsource_bsp_disk_structs.h"
#include "goldsource_bsp_mem_structs.h"
#include "mod_bsp.h"
#include "gl_backend.h"
#include "goldsource_bsp_world.h"

BSPModelAdapter::BSPModelAdapter(const char *modelName) : IModel(modelName)
{
    m_ModelIndex = atoi(modelName + 1);
    SetType(ModelType::StaticLightmapped);

   
    
}

BSPModelAdapter::~BSPModelAdapter()
{
}

void BSPModelAdapter::Render(SceneEntity *pEntity, SceneRenderer * sr, RenderMode mode, ShaderProgram* currentShader)
{
    auto model = m_Model.lock();

    if (!model)
        return;

    auto pMesh = model->GetDrawMesh();
    pMesh->Bind();

    auto &lists = model->GetDisplayList();

    for (auto &it : lists)
    {
        GLBackend::BindTexture(1, it.lm);
        GLBackend::BindTexture(0, it.diffuse);

        pMesh->Draw((uint32_t)it.first, (uint32_t)it.count);
    }

    pMesh->Unbind();

    
}

void BSPModelAdapter::OnSceneLoaded(Scene *pScene)
{   
    IWorldEntity *pWorld = pScene->GetWorldEntity();

    assert(instanceof <GoldSource::BSPWorld>(pWorld));

    GoldSource::BSPWorld *pBSPWorld = (GoldSource::BSPWorld *)pWorld;
    GoldSource::BSPModelRenderCookiePtr cookie = pBSPWorld->GetBSPModelRenderCookie(m_ModelIndex);

    m_Model = cookie;

    SetTransparent(cookie->HasTransparentFaces());
}


const std::optional<BoundingBox> BSPModelAdapter::GetBoundingBox() const
{
    if (m_Model.expired())
        return std::optional<BoundingBox>();

    auto ptr = m_Model.lock();

    return std::optional<BoundingBox>(ptr->GetBoundingBox());
}
