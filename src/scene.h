/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "common.h"
#include "game_configuration.h"
#include "lb3k_wrapper.h"
#include "model_obj.h"
#include "scene_entity.h"
#include "serial_counter.h"
#include "mod_manager.h"

enum class LevelFormat
{
    Unknown = 0,
    WavefrontOBJ,
    BSP
};

namespace GoldSource
{
class StudioModelV10;
};

class IWorldEntity;

class Scene
{
    lightBakerSettings_s        m_lightBakerParams;
    SerialCounter<unsigned int> m_ObjectsCounter;

    LevelFormat DetermineLevelFormatFromFileName(std::string levelName);
    LevelFormat m_LevelFormat = LevelFormat::Unknown;

    GameConfigurationWeakPtr m_pGameConfiguration;

    // Scene Entity 0 - level model
    std::list<SceneEntityPtr> m_SceneEntities;

    // TODO: review
    float m_flSceneScale = 1.0f;

    CEditHistory *     m_pEditHistory;
    SceneEntityWeakPtr m_pCurrentSelection;

    void ClearEntities();
    void LoadLevel(const char *levelName);

     IModelWeakPtr pTestModel;

     friend class SceneRenderer;

     const std::list<SceneEntityPtr> & GetEntities() const
     {
         return m_SceneEntities;
     }

public:
    Scene();
    ~Scene();

    void LoadLevel(const char *levelName, int loadFlags);

    void           DoDeleteSelection();

    SceneEntityPtr AddNewLight(glm::vec3 pos, LightTypes type, bool interactive = true);
    SceneEntityPtr AddNewGenericEntity();

    void AddNewSceneEntity(SceneEntity * entity);

    std::list<SceneEntityPtr> &GetSceneObjects();
    std::list<SceneEntityPtr> &GetLightDefs();

    CEditHistory *GetEditHistory() const;

    void               HintSelected(SceneEntityWeakPtr weakRef);
    SceneEntityWeakPtr GetSelection();

    bool IsModelLoaded();

    std::string GetModelFileName();
    std::string GetModelTextureName();

    // Entity manipulation
    void               AddEntityWithSerialNumber(SceneEntityPtr it, uint32_t sn);
    SceneEntityWeakPtr GetEntityBySerialNumber(size_t serialNumber);
    void               DeleteEntityWithSerialNumber(size_t serialNumber);
    void               DeleteEntity(SceneEntityWeakPtr l);

    // Scene scaling
    float GetSceneScale();
    void  SetScale(float f);
    void  RescaleLightPositions(float m_flScaleOriginal, float m_flScale);

    SceneEntityWeakPtr GetEntityWeakRef(SceneEntity *pEntity);
    void               Reload(int loadFlags);

    std::string ExportForCompiling(const char *newPath, lightBakerSettings_t *lb3kOptions);
    
    void        DumpLightmapMesh();
    void        DumpLightmapUV();

    GameConfigurationWeakPtr UsedGameConfiguration();
    uint32_t                 AllocSerialNumber();
    IWorldEntity* GetWorldEntity();
  
    size_t TotalEntities();
};