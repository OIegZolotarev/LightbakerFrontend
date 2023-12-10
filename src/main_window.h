/*
        LightBaker3000 Frontend project,
        (c) 2022 CrazyRussian
*/

#pragma once

#include "common.h"
#include "console.h"
#include "igui_panel.h"
#include "object_props.h"
#include "scene_renderer.h"
#include "ui_common.h"
#include "platform_window.h"

class Viewport;

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

struct timersData
{
    double frame_delta            = 0;
    int    actual_fps             = 0;
    Uint64 timestamp_now          = 0;
    Uint64 timestamp_last         = 0;
    int    frames_until_init      = 3;
    double fps_accum              = 0;
    int    num_frames_this_second = 0;
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

    void InitStuff();

    ~MainWindow();

    void MainLoop();

    int           Width();
    int           Height();
    SDL_Window *  Handle();
    float         FrameDelta();
    DebugConsole *Console();

    class SceneRenderer *GetSceneRenderer();
    // int *                Get3DGLViewport();

    void                UpdateDocks();
    defaultDockSides_s *GetDockSides();

    int  GetFPS();
    void ClearBackground();

    void SetTitle(std::string &fileName);

    void UpdateStatusbar(int updateFlags);
    
    Viewport *GetViewport(int index);
    void      CloneViewport(Viewport *pViewport);

    void InitImGUISDL2Platform() override;

private:
    enum ToolbarIcons
    {
        AddOmni = 0,
        AddSpot,
        AddDirect,
        BakeScene,
        MaxIcons
    };

    GLuint m_ToolbarIcons[ToolbarIcons::MaxIcons];

    bool  RenderToolbarIcon(GLuint iconId);
    float RenderMainMenu();
    float RenderMainToolbar(float menuHeight);

    std::string m_strTitle;
    SDL_Window *m_pSDLWindow;

    int m_iWindowHeight;
    int m_iWindowWidth;

    SDL_GLContext m_pGLContext;
    ImGuiContext *m_pImGUIContext;

    timersData m_TimersData;
    bool       m_bTerminated;

    bool HandleEvents(bool loop);

    void UpdateTimers();
    void GL_BeginFrame();

    void RenderGUI();

    void DrawLoadingBanner();
    void DrawBakingInProgressBanner(float yBannerOffset);

    void LimitToTargetFPS();

    class SceneRenderer *m_pSceneRenderer;
    
    bool PropagateControlsEvent(SDL_Event &e);
    

    bool         m_bShowConsole = true;
    DebugConsole m_Console;

    // Initialization
    void InitBackend();

    

    void InitCommands();
    void InitBackgroundRenderer();
    

    // Docking
    defaultDockSides_s       m_defaultDockSides;
    std::vector<IGUIPanel *> m_vPanels;

    void InitDocks();
    ImGuiID DockSpaceOverViewport(float heightAdjust, ImGuiDockNodeFlags dockspace_flags,
                                  const ImGuiWindowClass *window_class);

    int m_i3DViewport[4];    
    float m_flBakingProgress = 0;

    // Fancy backgound
    VariantValue *m_pBackgroudColorSetting1;
    VariantValue *m_pBackgroudColorSetting2;
    VariantValue *m_pUseGradientBackground;

    ShaderProgram *m_pBackgroundShader;
    DrawMesh *     m_pBackgroundMesh;

    statusBarData m_statusBarData;

    // Viewports    
    void InitViewports();  
    


};
