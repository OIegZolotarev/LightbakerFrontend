/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "common.h"
#include "goldsource_bsp_world.h"
#include "scene_renderer.h"
#include "camera.h"

using namespace GoldSource;

void BSPWorld::ReloadLightmaps()
{
    m_pLevel->ReloadLightmaps();
}

void BSPWorld::OnAdditionToScene()
{
    m_pLevel->PopulateScene();
}

BSPWorld::BSPWorld(const char *levelName) 
{
    FileData *fd = FileSystem::Instance()->LoadFile(levelName);

    if (fd)
    {
        m_pLevel = new GoldSource::BSPLevel(fd);
        fd->UnRef();
    }
    
    if (m_pLevel)
    {
        m_pRenderer = new BSPRenderer(m_pLevel);
    }

}

BSPWorld::~BSPWorld()
{
}

void BSPWorld::RenderLightshaded()
{

    glm::vec3 cameraPos = Application::GetMainWindow()->GetSceneRenderer()->GetCamera()->GetOrigin();

    m_pRenderer->RenderWorld(cameraPos);
}

bool BSPWorld::IsDataLoaded()
{
    return true;
}

std::string BSPWorld::ExportForCompiling(const char *newPath, lightBakerSettings_t *lb3kOptions)
{    
    return m_pLevel->Export(newPath, lb3kOptions);
}
