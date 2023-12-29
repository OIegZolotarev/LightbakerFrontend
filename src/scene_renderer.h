/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/

#pragma once

#include "common.h"
#include "edit_history.h"
#include "gl_backend.h"
#include "lights.h"
#include "main_window.h"
#include "model_obj.h"
#include "r_chain.h"
#include "scene.h"
#include "viewport.h"
#include "viewport_rendermodes.h"

// Load-Reload flags
#define LRF_RELOAD_TEXTURES  (1 << 0)
#define LRF_RELOAD_LIGHTMAPS (1 << 1)
#define LRF_KEEP_ENTITIES    (1 << 2) // Keep editor entities, otherwise reload from file

#define LRF_LOAD_ALL (LRF_RELOAD_TEXTURES | LRF_RELOAD_LIGHTMAPS)

class Camera;
class MainWindow;

typedef struct
{
    SceneEntity *pEntity;
    ModelType    modType;
    float        renderDistance;
} sSortInfo;

class SceneRenderer : public IEventHandler
{
public:
    SceneRenderer(class MainWindow *pTargetWindow);
    ~SceneRenderer();

    void RegisterRendermodesCommands();

    class Camera *GetCamera();
    void          FocusCameraOnObject(SceneEntityPtr it);

    // Rendering
    void RenderScene(Viewport *pViewport);

    void ResetTransparentChain();

    void RenderHelperGeometry();
    void RenderGenericEntity(SceneEntity *pEntity);

    void RenderPointEntityDefault(const glm::vec3 &m_Position, const glm::vec3 &m_Mins, const glm::vec3 &m_Maxs,
                                  const glm::vec3 &m_Color, const uint32_t objectSerialNumber);

    // Render modes
    void       SetRenderMode(RenderMode param1);
    RenderMode GetRenderMode();

    int   HandleEvent(bool bWasHandled, SDL_Event &e, float flFrameDelta) override;
    float FrameDelta();

    // Loading
    void LoadModel(const char *dropped_filedir, int loadFlags);
    void ReloadScene(int loadFlags);

    // Billboards
    void SetupBuildboardsRenderer();
    void DrawBillboard(const glm::vec3 pos, const glm::vec2 size, const GLTexture *texture, const glm::vec3 tint,
                       const uint32_t objectSerialNumber);

    Scene *GetScene();

    glm::vec3 GetNewLightPos();
    glm::vec3 GetRenderPos();

    void AddTransparentEntity(SceneEntityWeakPtr pEntity);

void DrawBillboardMesh();

private:
    RenderChain m_TransparentEntitiesChain;

    RenderMode m_RenderMode        = RenderMode::Lightshaded;
    bool       m_bWireframeOverlay = false;

    void Debug_DrawGround();

    Camera *    m_pCamera;
    MainWindow *m_pTargetWindow;
    Scene *     m_pScene;

    VariantValue *m_pShowGround;

    // Helper meshes
    DrawMesh *m_pBillBoard;
    DrawMesh *m_pSolidCube;
    DrawMesh *m_pUnitBoundingBox;
    DrawMesh *m_pIntensitySphere;
    DrawMesh *m_pSpotlightCone;

    // Shaders
    ShaderProgram *m_pBillBoardsShader = nullptr;

    void           DrawLightHelperGeometry(SceneEntityWeakPtr pObject);
    lightDefWPtr_t m_pCurrentSelection;

    void DumpLightmapMesh();
    void DumpLightmapUV();

    

    std::vector<sSortInfo> m_vSortedSolidEntities;
    std::vector<sSortInfo> m_vSortedTransparentEntities;

    void                   SortRenderLists();

    ShaderProgram *m_pStudioShader;

    

    void RenderEntitiesChain(const sSortInfo * pData, const size_t nEntities, bool transparent) const;
    ShaderProgram *SetupShaderForModelType(const ModelType currentType) const;
};
