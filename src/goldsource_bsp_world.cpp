/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "common.h"
#include "goldsource_bsp_world.h"
#include "scene_renderer.h"
#include "r_camera.h"

using namespace GoldSource;

void BSPWorld::ReloadLightmaps()
{
    m_pLevel->ReloadLightmaps();
}

void BSPWorld::OnAdditionToScene(Scene * pScene)
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
    
    if (m_pLevel)
    {
        m_pRenderer = new BSPRenderer(m_pLevel);
    }

    short *minsmaxs = m_pLevel->GetNodes(0)->minmaxs;

    SetMins({
        (float)minsmaxs[0],
        (float)minsmaxs[1],
        (float)minsmaxs[2]
    });


    SetMaxs({(float)minsmaxs[3], (float)minsmaxs[4], (float)minsmaxs[5]});

}

BSPWorld::~BSPWorld()
{
}

void BSPWorld::RenderLightshaded()
{
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
