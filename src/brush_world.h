/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#pragma once
#include "world_entity.h"

class HammerMap;
typedef struct map220_entity_s map220_entity_t;

class BrushWorld : public IWorldEntity
{
    void BuildBrushesOfEntity(const HammerMap * pMap, const map220_entity_t *pEntity);

public:
    
    BrushWorld(Scene * pScene, HammerMap *pMapFile);
    BrushWorld(BrushWorld *pOther);

    SceneEntity *Clone() override;
    std::string ExportForCompiling(const char *newPath, lightBakerSettings_t *lb3kOptions) override;
    
    void ReloadLightmaps() override;
};
