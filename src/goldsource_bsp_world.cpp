/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "goldsource_bsp_asynch_loader.h"
#include "goldsource_bsp_world.h"
#include "r_camera.h"
#include "scene_renderer.h"
#include "mod_manager.h"

using namespace GoldSource;

void BSPWorld::ReloadLightmaps()
{
    m_pLevel->ReloadLightmaps();
    m_pRenderer->ReloadLightmaps();
}

void BSPWorld::OnAdditionToScene(Scene *pScene)
{
    m_pLevel->PopulateScene(pScene);
}

BSPWorld::BSPWorld(const char *levelName, Scene *pScene) : IWorldEntity(pScene)
{
    FileData *fd = FileSystem::Instance()->LoadFile(levelName);

    if (fd)
    {
        m_pLevel = new GoldSource::BSPLevel(fd, pScene);
        fd->UnRef();
    }

    BSPAsynchLoader *pTask = new BSPAsynchLoader(this, m_pLevel);
    pTask->Schedule();

    m_pRenderer = 0;
}

BSPWorld::~BSPWorld()
{
}

void GoldSource::BSPWorld::Render(RenderMode mode, ShaderProgram* shader)
{
    if (!m_pRenderer)
        return;

    auto      sr        = Application::GetMainWindow()->GetSceneRenderer();
    glm::vec3 cameraPos = Application::GetMainWindow()->GetSceneRenderer()->GetCamera()->GetOrigin();

    m_pRenderer->RenderWorld(cameraPos);
    //    sr->RenderPointEntityDefault(m_Position, m_Mins, m_Maxs, {1, 0, 1});
}

bool BSPWorld::IsDataLoaded()
{
    return true;
}

std::string BSPWorld::ExportForCompiling(const char *newPath, lightBakerSettings_t *lb3kOptions)
{
    return m_pLevel->Export(newPath, lb3kOptions);
}

void BSPWorld::OnLevelLoaded()
{
    if (m_pLevel)
    {
        m_pRenderer = new BSPRenderer(m_pLevel);
    }

    short *minsmaxs = m_pLevel->GetNodes(0)->minmaxs;

    SetBoundingBox(BoundingBox(minsmaxs));
    SetPosition(glm::vec3{0, 0, 0});

    auto worldModel = ModelsManager::Instance()->LookupModel("*0");

    SetModel(worldModel);

    ModelsManager::Instance()->OnSceneLoaded(m_pScene); 

    m_pLevel->PopulateScene(m_pScene);
    
}

GoldSource::BSPModelRenderCookiePtr BSPWorld::GetBSPModelRenderCookie(size_t idx)
{
    return m_pRenderer->GetBSPModelRenderCookie(idx);
}
