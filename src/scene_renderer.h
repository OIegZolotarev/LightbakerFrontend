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
#include "scene.h"

enum class RenderMode
{
    Unshaded = 0,
    Lightshaded,
    Groups,
    WireframeUnshaded,
    WireframeShaded
};

// Load-Reload flags
#define LRF_RELOAD_TEXTURES  (1 << 0)
#define LRF_RELOAD_LIGHTMAPS (1 << 1)
#define LRF_KEEP_ENTITIES    (1 << 2) // Keep editor entities, otherwise reload from file

#define LRF_LOAD_ALL (LRF_RELOAD_TEXTURES | LRF_RELOAD_LIGHTMAPS)

class SceneRenderer : public IEventHandler
{
  public:
    SceneRenderer(class MainWindow *pTargetWindow);
    ~SceneRenderer();

    void SetupBuildboardsRenderer();
    void RegisterRendermodesCommands();

    class Camera *GetCamera();

    void RenderScene();
    void RenderHelperGeometry(SelectionManager *selectionManager);

    int HandleEvent(bool bWasHandled, SDL_Event &e) override;
    float FrameDelta();

    void LoadModel(const char *dropped_filedir, int loadFlags);
    void ReloadScene(int loadFlags);

    void DrawBillboard(const glm::vec3 pos, const glm::vec2 size, const GLTexture *texture, const glm::vec3 tint);
    void DrawBillboardSelection(const glm::vec3 pos, const glm::vec2 size, const GLTexture *texture, const int index);

    void FocusCameraOnObject(SceneEntityPtr it);

    Scene *GetScene();

    glm::vec3 GetNewLightPos();

    void SetRenderMode(RenderMode param1);
    void RenderGenericEntity(SceneEntity *pEntity);
    RenderMode GetRenderMode();

  private:
    RenderMode m_RenderMode  = RenderMode::Lightshaded;
    bool m_bWireframeOverlay = false;

    void Debug_DrawGround();

    class Camera *m_pCamera;
    class MainWindow *m_pTargetWindow;
    Scene *m_pScene;

    DrawMesh *m_pBillBoard;

    DrawMesh *m_pSolidCube;
    DrawMesh *m_pUnitBoundingBox;
    DrawMesh *m_pIntensitySphere;
    DrawMesh *m_pSpotlightCone;

    ShaderProgram *m_pBillBoardsShader = nullptr;
    ShaderProgram *m_pBillBoardsShaderSel = nullptr;

    void DrawLightHelperGeometry(SceneEntityWeakPtr pObject);
    lightDefWPtr_t m_pCurrentSelection;

    void DumpLightmapMesh();
    void DumpLightmapUV();

  public:
    void RenderPointEntityDefault(glm::vec3 m_Position, glm::vec3 m_Mins, glm::vec3 m_Maxs, glm::vec3 m_Color);
};
