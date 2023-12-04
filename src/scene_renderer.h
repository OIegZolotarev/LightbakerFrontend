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
#include "viewport.h"


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

    void RenderScene(Viewport * pViewport);


    void RenderHelperGeometry(SelectionManager *selectionManager);

    int   HandleEvent(bool bWasHandled, SDL_Event &e, float flFrameDelta) override;
    float FrameDelta();

    void LoadModel(const char *dropped_filedir, int loadFlags);
    void ReloadScene(int loadFlags);

    void DrawBillboard(const glm::vec3 pos, const glm::vec2 size, const GLTexture *texture, const glm::vec3 tint,
                       const uint32_t objectSerialNumber);
    

    void FocusCameraOnObject(SceneEntityPtr it);

    Scene *GetScene();

    glm::vec3 GetNewLightPos();

    void       SetRenderMode(RenderMode param1);
    void       RenderGenericEntity(SceneEntity *pEntity);
    RenderMode GetRenderMode();

private:
    RenderMode m_RenderMode        = RenderMode::Lightshaded;
    bool       m_bWireframeOverlay = false;

    void Debug_DrawGround();

    Camera *    m_pCamera;

    class MainWindow *m_pTargetWindow;
    Scene *           m_pScene;

    DrawMesh *m_pBillBoard;

    DrawMesh *m_pSolidCube;
    DrawMesh *m_pUnitBoundingBox;
    DrawMesh *m_pIntensitySphere;
    DrawMesh *m_pSpotlightCone;

    ShaderProgram *m_pBillBoardsShader    = nullptr;
    ShaderProgram *m_pBillBoardsShaderSel = nullptr;

    void           DrawLightHelperGeometry(SceneEntityWeakPtr pObject);
    lightDefWPtr_t m_pCurrentSelection;

    void DumpLightmapMesh();
    void DumpLightmapUV();

public:
    void RenderPointEntityDefault(const glm::vec3 & m_Position, const glm::vec3 & m_Mins, const glm::vec3 & m_Maxs, const glm::vec3 & m_Color, const uint32_t objectSerialNumber);
    glm::vec3 GetRenderPos();
    
};
