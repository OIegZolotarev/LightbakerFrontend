/*
        LightBaker3000 Frontend project,
        (c) 2023 CrazyRussian
*/
#pragma once

// Forward declaration start
class Viewport;
class IPlatformWindow;
// Forward declaration end

class ViewportsOrchestrator: public Singleton<ViewportsOrchestrator>
{
    std::list<Viewport *> m_lstViewports;
    nlohmann::json m_savedViewports;

    int CountViewports(const IPlatformWindow *it);

    Viewport *m_pHoveredViewport;

public:
    ~ViewportsOrchestrator();

    void      Init(nlohmann::json && persistentData);
    Viewport* AddNewViewport(const char *name, IPlatformWindow * pHostWindow,Viewport * pCloneFrom, bool initialyVisible = true);

    void FlagRepaintAll();

    void RenderViewports(IPlatformWindow *pWindow, float flFrameDelta);
    void DisplayViewports(IPlatformWindow *pWindow);

    void SaveViewports(nlohmann::json & persistentData);
    bool LoadViewports();
    void DestroyWindowViewports(IPlatformWindow *wind);
    void CloneViewportToLeastClutteredWindow(Viewport *param1);

    void EnsureAtLeastOneViewportExists();

    Viewport *GetHoveredViewport();
};
