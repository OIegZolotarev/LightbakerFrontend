/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once
#include "world_entity.h"
#include "goldsource_bsp_level.h"
#include "goldsource_bsp_renderer.h"

namespace GoldSource
{
class BSPWorld: public IWorldEntity
{
    BSPLevel *m_pLevel;
    BSPRenderer *m_pRenderer;

  public:
    BSPWorld(const char* levelName);    
    ~BSPWorld();

    void ReloadLightmaps() override;
    void OnAdditionToScene() override;

    void RenderLightshaded() override;

    bool IsDataLoaded() override;

    std::string ExportForCompiling(const char *newPath, lightBakerSettings_t *lb3kOptions) override;
};
}

