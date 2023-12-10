/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "goldsource_bsp_asynch_loader.h"
#include "goldsource_bsp_disk_structs.h"
#include "goldsource_bsp_level.h"
#include "goldsource_bsp_world.h"

GoldSource::BSPAsynchLoader::BSPAsynchLoader(BSPWorld *pWorld, BSPLevel *level)
{
    m_pLevel = level;
    m_pWorld = pWorld;

    m_strDescription  = std::format("Loading \"{0}.bsp\"", level->GetBaseName());
    m_nTotalSteps     = HEADER_LUMPS;
    m_nPerformedSteps = 0;
}

ITaskStepResult *GoldSource::BSPAsynchLoader::ExecuteStep(LoaderThread *loaderThread)
{
    // Lumps not located in load order, so we need to reorder them in order to load in loop

    int ordering[] = {LUMP_ENTITIES,   LUMP_VERTEXES, LUMP_EDGES,   LUMP_SURFEDGES, LUMP_TEXTURES,
                      LUMP_LIGHTING,   LUMP_PLANES,   LUMP_TEXINFO, LUMP_FACES,     LUMP_MARKSURFACES,
                      LUMP_VISIBILITY, LUMP_LEAFS,    LUMP_NODES,   LUMP_CLIPNODES, LUMP_MODELS};

    static_assert(HEADER_LUMPS == ARRAYSIZE(ordering));

    int  lumpNumber = ordering[m_nPerformedSteps];
    auto lump       = &m_pLevel->m_Header->lumps[lumpNumber];

    switch (lumpNumber)
    {
    case LUMP_ENTITIES:
        m_pLevel->Mod_LoadEntities(lump);
        break;
    case LUMP_PLANES:
        m_pLevel->Mod_LoadPlanes(lump);
        break;
    case LUMP_TEXTURES:
        m_pLevel->Mod_LoadTextures(lump);
        break;
    case LUMP_VERTEXES:
        m_pLevel->Mod_LoadVertexes(lump);
        break;
    case LUMP_VISIBILITY:
        m_pLevel->Mod_LoadVisibility(lump);
        break;
    case LUMP_NODES:
        m_pLevel->Mod_LoadNodes(lump);
        break;
    case LUMP_TEXINFO:
        m_pLevel->Mod_LoadTexinfo(lump);
        break;
    case LUMP_FACES:
        m_pLevel->Mod_LoadFaces(lump);
        break;
    case LUMP_LIGHTING:
        m_pLevel->Mod_LoadLighting(lump);
        break;
    case LUMP_CLIPNODES:
        m_pLevel->Mod_LoadClipnodes(lump);
        break;
    case LUMP_LEAFS:
        m_pLevel->Mod_LoadLeafs(lump);
        break;
    case LUMP_MARKSURFACES:
        m_pLevel->Mod_LoadMarksurfaces(lump);
        break;
    case LUMP_EDGES:
        m_pLevel->Mod_LoadEdges(lump);
        break;
    case LUMP_SURFEDGES:
        m_pLevel->Mod_LoadSurfedges(lump);
        break;
    case LUMP_MODELS:
        m_pLevel->Mod_LoadSubmodels(lump);
        break;
    }

    m_nPerformedSteps++;

    //SDL_Delay(300);

    if (m_nPerformedSteps == HEADER_LUMPS)
        return new BSPLoaderCompletionStep(m_pWorld);

    return new BSPLoaderTaskStepResult(m_nPerformedSteps, HEADER_LUMPS);
}

void GoldSource::BSPAsynchLoader::OnCompletion()
{
    return;
}

GoldSource::BSPLoaderTaskStepResult::BSPLoaderTaskStepResult(size_t nCurrentSteps, size_t nTotalSteps)
    : ITaskStepResult(TaskStepResultType::StepPerformed)
{
    const char *lumps[] = {"LUMP_ENTITIES",   "LUMP_VERTEXES", "LUMP_EDGES",   "LUMP_SURFEDGES", "LUMP_TEXTURES",
                     "LUMP_LIGHTING",   "LUMP_PLANES",   "LUMP_TEXINFO", "LUMP_FACES",     "LUMP_MARKSURFACES",
                     "LUMP_VISIBILITY", "LUMP_LEAFS",    "LUMP_NODES",   "LUMP_CLIPNODES", "LUMP_MODELS"};

    m_strElementDescription = std::format("Step {0} of {1} ({2})", nCurrentSteps, nTotalSteps, lumps[nCurrentSteps]);
}

GoldSource::BSPLoaderTaskStepResult::~BSPLoaderTaskStepResult()
{
}

GoldSource::BSPLoaderCompletionStep::BSPLoaderCompletionStep(BSPWorld *pLevel)
    : ITaskStepResult(TaskStepResultType::FinishedSuccesfully)
{
    m_pLevel = pLevel;
}

void GoldSource::BSPLoaderCompletionStep::ExecuteOnCompletion()
{
    m_pLevel->OnLevelLoaded();
}

bool GoldSource::BSPLoaderCompletionStep::NeedEndCallback()
{
    return true;
}
