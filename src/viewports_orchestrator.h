/*
        LightBaker3000 Frontend project,
        (c) 2023 CrazyRussian
*/
#pragma once

// Forward declaration start
class Viewport;
class IPlatformWindow;
class SharedFBO;
class SharedFBONode;
// Forward declaration end

enum class SplitAxis
{
    Horizontal = 0,
    Vertical
};

class ISharedFBOTreeItem
{
    bool m_bFree = true;

protected:
    glm::vec2 m_Position;
    glm::vec2 m_Extents;

    ISharedFBOTreeItem *m_pParent;

public:
    ISharedFBOTreeItem(glm::vec2 position, glm::vec2 extents, ISharedFBOTreeItem *parent);

    bool         RectFits(glm::vec2 extents);
    virtual bool IsLeaf() = 0;

    void Lock();
    void Release();
    bool IsFree();

    void SetParent(ISharedFBOTreeItem *pParent)
    {
        m_pParent = pParent;
    }
};

class SharedFBOLeaf : public ISharedFBOTreeItem
{
    GLFramebufferObject *m_pFBO;

public:
    SharedFBOLeaf(GLFramebufferObject *pFrameBuffer, const glm::vec2 position, const glm::vec2 extents,
                  SharedFBONode *pParent);

    bool IsLeaf() override;

    void SetPosition(glm::vec2 position);
    void SetExtents(glm::vec2 extents);

    glm::vec2 GetPosition();
    void      EnableFBO();
    void      DisableFBO();
    void      ApplyGLViewport();
    
    GLuint GetGLTextureNum();
};

class SharedFBONode : public ISharedFBOTreeItem
{    
    ISharedFBOTreeItem *m_pChildren[2] = {nullptr, nullptr};

    SharedFBO *m_pSharedFBO = nullptr;
    void       Split(SplitAxis axis, float pos);

    bool HasFreeSpace();
    bool HasLeafs();

protected:
    void CondenseFreeSpace(SharedFBONode *pEmptyNode);

public:
    SharedFBONode(SharedFBO *pFBO, SharedFBONode *parent, glm::vec2 position, glm::vec2 extents);
    SharedFBOLeaf *AllocateLeaf(glm::vec2 extents);

    bool IsLeaf() override;

    void SetPosition(glm::vec2 position);
    void SetExtents(glm::vec2 extents);

    void OnLeafReleased();
};

class SharedFBO
{
    std::list<SharedFBONode *> m_Nodes;
    std::list<SharedFBOLeaf *> m_Leafs;

    GLFramebufferObject *m_pFBO;

    SharedFBONode *m_pRootNode;

public:
    SharedFBO(GLFramebufferObject *pFrameBuffer);
    ~SharedFBO();

    SharedFBONode *PoolAllocateNode(glm::vec2 position, glm::vec2 extents);
    SharedFBOLeaf *PoolAllocateLeaf(SharedFBONode *parent, glm::vec2 pos, glm::vec2 extents);

    SharedFBOLeaf *AllocateViewport(glm::vec2 extents);
};

class ViewportsOrchestrator : public Singleton<ViewportsOrchestrator>
{
    glm::vec2 m_MaxFBOExtents;

    std::list<SharedFBO *> m_lstSharedFBO;

    std::list<Viewport *> m_lstViewports;
    nlohmann::json        m_savedViewports;

    int CountViewports(const IPlatformWindow *it);

    Viewport *m_pHoveredViewport;

public:
    ~ViewportsOrchestrator();

    void      Init(nlohmann::json &&persistentData);
    Viewport *AddNewViewport(const char *name, IPlatformWindow *pHostWindow, Viewport *pCloneFrom,
                             bool initialyVisible = true);

    void FlagRepaintAll();

    void OnNewApplicationTick();
    void RenderViewports(IPlatformWindow *pWindow, float flFrameDelta);
    void DisplayViewports(IPlatformWindow *pWindow);

    void SaveViewports(nlohmann::json &persistentData);
    bool LoadViewports();
    void DestroyWindowViewports(IPlatformWindow *wind);
    void CloneViewportToLeastClutteredWindow(Viewport *param1);

    void EnsureAtLeastOneViewportExists();

    Viewport *GetHoveredViewport();

    SharedFBOLeaf *AllocateSharedFBOViewport(Viewport * pViewport, glm::vec2 newExtents);

private:
    void SortViewportsByFBO();
};
