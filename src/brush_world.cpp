/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "brush_world.h"
#include "brush_object.h"

#include "hammer_map.h"
#include "scene.h"
#include "scene_entity.h"

BrushWorld::BrushWorld(BrushWorld *pOther) : IWorldEntity(pOther->m_pScene)
{
}

BrushWorld::BrushWorld(Scene *pScene, HammerMap *pMapFile) : IWorldEntity(pScene)
{
    const map220_entity_t *pWorldEntity = pMapFile->GetEntity(0);

    BuildBrushesOfEntity(pMapFile, pWorldEntity);
}

SceneEntity *BrushWorld::Clone()
{
    return new BrushWorld(this);
}

std::string BrushWorld::ExportForCompiling(const char *newPath, lightBakerSettings_t *lb3kOptions)
{
    return "";
}

void BrushWorld::ReloadLightmaps()
{
    throw std::logic_error("The method or operation is not implemented.");
}

void BrushWorld::BuildBrushesOfEntity(const HammerMap *pMap, const map220_entity_t *pEntity)
{

    const size_t firstBrush = pEntity->firstBrush;
    const size_t lastBrush  = pEntity->firstBrush + pEntity->numBrushes;

    for (size_t b = firstBrush; b < lastBrush; b++)
    {
        const map220brush_t *pBrush = pMap->GetBrush(b);

        BrushObject *pNewEntity = new BrushObject(m_pScene);

        const size_t firstFace = pBrush->firstFace;
        const size_t lastFace  = pBrush->firstFace + pBrush->numFaces;

        pNewEntity->GenerateModel(pMap, firstFace, lastFace, b);
        m_pScene->AddNewSceneEntity(pNewEntity);
    }
}
