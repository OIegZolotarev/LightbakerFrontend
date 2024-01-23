/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#pragma once

#include "common.h"
#include "game_configuration.h"
#include "lb3k_wrapper.h"
#include "mod_manager.h"
#include "model_obj.h"
#include "r_bvh.h"
#include "r_octree.h"
#include "scene_entity.h"
#include "serial_counter.h"

// Forward declarations start

namespace GoldSource
{
class StudioModelV10;
};

class IWorldEntity;

// Forward declarations end

enum class LevelFormat
{
    Unknown = 0,
    WavefrontOBJ,
    BSP,
    MAP
};

typedef struct sceneCameraDescriptor_s
{
    std::string description;
    glm::vec3   position;
    glm::vec3   angles;

} sceneCameraDescriptor_t;

typedef std::list<sceneCameraDescriptor_t> CameraDescriptorsList;

class Scene
{
    friend class SceneRenderer;

    lightBakerSettings_s        m_lightBakerParams;
    SerialCounter<unsigned int> m_ObjectsCounter;

    LevelFormat DetermineLevelFormatFromFileName(std::string levelName);
    LevelFormat m_LevelFormat = LevelFormat::Unknown;

    GameConfigurationWeakPtr m_pGameConfiguration;

    // Scene Entity 0 - level model
    std::list<SceneEntityPtr> m_SceneEntities;

    // TODO: review
    float m_flSceneScale = 1.0f;

    CEditHistory *m_pEditHistory;

    void ClearEntities();
    void LoadLevel(const char *levelName);

    IModelWeakPtr                    pTestModel;
    const std::list<SceneEntityPtr> &GetEntities() const;

    // OctreeNode *m_pOctree;
    BVHTree *m_pBVHTree;


    // Camera tool
    CameraDescriptorsList m_lstEditorCameras;

public:
    Scene();
    ~Scene();

    void LoadLevel(const char *levelName, int loadFlags);

    // Scene objects
    SceneEntityPtr AddNewLight(glm::vec3 pos, LightTypes type, bool interactive = true);
    SceneEntityPtr AddNewSceneEntity(SceneEntity *entity);
    void           AddEntityWithSerialNumber(SceneEntityPtr it, uint32_t sn);    
    
    void           UpdateEntityBVH(const uint32_t serialNumber, const BVHBoundingBox &bboxAbsolute);
    void           OnEntityRegistered(SceneEntityPtr &it);

    std::list<SceneEntityPtr> &GetSceneObjects();
    SceneEntityWeakPtr         GetEntityBySerialNumber(size_t serialNumber);

    // Editing
    void          DeleteEntityWithSerialNumber(size_t serialNumber);
    void          DeleteEntity(SceneEntityWeakPtr l);
    CEditHistory *GetEditHistory() const;

    bool IsModelLoaded();

    // Scene scaling
    float GetSceneScale();
    void  SetScale(float f);
    void  RescaleLightPositions(float m_flScaleOriginal, float m_flScale);

    SceneEntityWeakPtr GetEntityWeakRef(SceneEntity *pEntity);
    void               Reload(int loadFlags);

    std::string ExportForCompiling(const char *newPath, lightBakerSettings_t *lb3kOptions);

    void DumpLightmapMesh();
    void DumpLightmapUV();

    GameConfigurationWeakPtr UsedGameConfiguration();
    uint32_t                 AllocSerialNumber();
    IWorldEntity *           GetWorldEntity();

    size_t TotalEntities();

    void     DebugRenderBVH();
    void     DebugRenderBVHUI();
    BVHTree *GetBVHTree();

    void DoDeleteSelection();

    // Camera tool
    const CameraDescriptorsList &GetSceneCamerasDescriptors() const;
    void                         AddSceneCameraDescriptor(sceneCameraDescriptor_t &newDescritor);
    void                         DeleteSceneCameraDescriptor(size_t idx);
    void                         UpdateSceneCameraDescriptor(size_t idx, sceneCameraDescriptor_t &newDescriptor);
    sceneCameraDescriptor_t      & GetSceneCameraDescriptor(size_t selected);
    static Scene *               ActiveInstance();
};