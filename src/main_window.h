/*
        LightBaker3000 Frontend project,
        (c) 2022 CrazyRussian
*/

#pragma once

#include "common.h"
#include "console.h"
#include "igui_panel.h"
#include "object_props.h"
#include "platform_window.h"
#include "scene_renderer.h"
#include "ui_common.h"

class Viewport;
class SceneRenderer;

BETTER_ENUM(StatusbarField, int, GameConfig, Position, ObjectDescription, ObjectSize, GridStep)

#define FL_UPDATE_ALL_STATUS_FIELDS                                                                                    \
    (1 << StatusbarField::GameConfig | 1 << StatusbarField::Position | 1 << StatusbarField::ObjectDescription |        \
     1 << StatusbarField::ObjectSize | 1 << StatusbarField::GridStep)

struct statusBarData
{
    std::string gameName          = "Conf.: <Conf. Name>";
    std::string position          = "Cam.: [512 500 300]";
    std::string objectDescription = "Sel.: Brush with 12 faces";
    std::string objectSize        = "Dim.: [128x64x260]";
    std::string gridStep          = "Grid: 32 units";
    //    std::string zoom              = "Zoom: ";
};



typedef struct defaultDockSides_s
{
    ImGuiID idDockLeftTop;
    ImGuiID idDockLeftBottom;
    ImGuiID idDockRightTop;
    ImGuiID idDockRightBottom;
    ImGuiID idDockUpLeft;
    ImGuiID idDockUpRight;
    ImGuiID idDockBottomLeft;
    ImGuiID idDockBottomRight;
} defaultDockSides_t;

#define NUM_VIEWPORTS 4

class MainWindow : public IPlatformWindow
{
public:
    MainWindow(const char *title, glm::vec2 defaultSize);
    ~MainWindow();

    void IterateUpdate() override;

    int Width();
    int Height();

    SDL_Window *Handle();

    DebugConsole * Console();
    SceneRenderer *GetSceneRenderer();

    void                UpdateDocks();
    defaultDockSides_s *GetDockSides();

    float FrameDelta();
    int   GetFPS();

    void ClearBackground(bool rebindShader);
    Viewport *GetViewport(int index);
    void      CloneViewport(Viewport *pViewport);

    void SetTitle(std::string &fileName);
    void UpdateStatusbar(int updateFlags);

    int  GetState();
    bool HandleEvent(SDL_Event &event) override;

    void InitStuff();

private:
    enum wndToolbarIcons
    {
        AddOmni = 0,
        AddSpot,
        AddDirect,
        BakeScene,
        MaxIcons
    };

    GLuint m_ToolbarIcons[wndToolbarIcons::MaxIcons];

    bool  RenderToolbarIcon(GLuint iconId);
    float RenderMainMenu();
    float RenderMainToolbar(float menuHeight);
        
    void UpdateTimers();
    void GL_BeginFrame();

    void RenderGUI();

    void DrawStatusBar();

    void DrawLoadingBanner();
    void DrawBakingInProgressBanner(float yBannerOffset);

    void LimitToTargetFPS();

    class SceneRenderer *m_pSceneRenderer;

    bool PropagateControlsEvent(SDL_Event &e);

    bool         m_bShowConsole = true;
    DebugConsole m_Console;

    // Initialization

    void InitBackend();
    void InitTimers();
    void InitCommands();
    void InitBackgroundRenderer();
    void InitViewports();
    void InitImGUISDL2Platform() override;

    // Docking
    defaultDockSides_s       m_defaultDockSides;
    std::vector<IGUIPanel *> m_vPanels;

    void    InitDocks();
    ImGuiID DockSpaceOverViewport(float heightAdjust, ImGuiDockNodeFlags dockspace_flags,
                                  const ImGuiWindowClass *window_class);

    int   m_i3DViewport[4];
    float m_flBakingProgress = 0;

    // Fancy backgound
    VariantValue *m_pBackgroudColorSetting1;
    VariantValue *m_pBackgroudColorSetting2;
    VariantValue *m_pUseGradientBackground;

    ShaderProgram *m_pBackgroundShader;
    DrawMesh *     m_pBackgroundMesh;

    statusBarData m_statusBarData;

protected:
    bool IsMainWindow() override;
};
