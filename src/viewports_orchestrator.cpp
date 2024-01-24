/*
        LightBaker3000 Frontend project,
        (c) 2023 CrazyRussian
*/

#include "application.h"
#include "common.h"
#include "viewports_orchestrator.h"
#include "gl_framebuffer_object.h"
#include "imgui_internal.h"

ViewportsOrchestrator::~ViewportsOrchestrator()
{
    for (auto &it : m_lstViewports)
        delete it;

    for (auto &it : m_lstSharedFBO)
        delete it;
}

void ViewportsOrchestrator::Init(nlohmann::json &&data)
{
    m_savedViewports = data;
}

Viewport *ViewportsOrchestrator::AddNewViewport(const char *name, IPlatformWindow *pHostWindow, Viewport *pCloneFrom,
                                                bool initialyVisible /*= true*/)
{
    Viewport *pResult = new Viewport(name, pHostWindow, pCloneFrom);

    m_lstViewports.push_back(pResult);
    pHostWindow->AddEventHandler(pResult);
    return pResult;
}

void ViewportsOrchestrator::FlagRepaintAll()
{
    for (auto &it : m_lstViewports)
    {
        it->FlagUpdate();
    }
}

void ViewportsOrchestrator::OnNewApplicationTick()
{
    // m_pHoveredViewport = nullptr;
}

void ViewportsOrchestrator::RenderViewports(IPlatformWindow *pWindow, float flFrameDelta)
{
    BT_PROFILE("ViewportsOrchestrator::RenderViewports()");

    GLuint currentTexture = 0;

    for (auto &it : m_lstViewports)
    {
        auto sharedFBO = it->GetSharedFBOLeaf();
        if (!it->IsVisible())
        {
            if (sharedFBO)
            {
                sharedFBO->Release();
                it->SetSharedFBOLeaf(nullptr);
            }
            continue;
        }

        if (it->GetPlatformWindow() != pWindow)
            continue;

        

        if (!sharedFBO)
            continue;

        GLuint fboTex = sharedFBO->GetGLTextureNum();

        if (fboTex != currentTexture)
        {
            currentTexture = fboTex;
            sharedFBO->EnableFBO();
        }

        if (it->GetMouseHoveringStatus() != ViewportMouseHover::NotHovered)
        {
            // assert((m_pHoveredViewport == it) && "Error in hovering logic - multiple viewports seems to be hovered");
            m_pHoveredViewport = it;
        }
        else if (it == m_pHoveredViewport)
        {
            m_pHoveredViewport = nullptr;
        }

        it->RenderFrame(flFrameDelta);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ViewportsOrchestrator::DisplayViewports(IPlatformWindow *pWindow)
{
    BT_PROFILE("ViewportsOrchestrator::DisplayViewports()");

    for (auto &it : m_lstViewports)
    {
        if (it->GetPlatformWindow() != pWindow)
            continue;

        it->DisplayRenderedFrame();
    }
}

void ViewportsOrchestrator::SaveViewports(nlohmann::json &persistentData)
{
    nlohmann::json block;

    for (auto &it : m_lstViewports)
    {
        nlohmann::json descr;
        it->SaveState(descr);
        block.push_back(descr);
    }

    persistentData["Viewports"] = block;
}

bool ViewportsOrchestrator::LoadViewports()
{
    for (auto &it : m_savedViewports)
    {
        Viewport *pViewport = Viewport::LoadState(it);

        if (pViewport)
        {
            pViewport->RegisterEventHandlerAtHost();
            m_lstViewports.push_back(pViewport);
        }
    }

    return true;
}

void ViewportsOrchestrator::DestroyWindowViewports(IPlatformWindow *wind)
{
    m_lstViewports.remove_if([&](Viewport *pViewport) {
        if (pViewport->GetPlatformWindow() == wind)
        {
            delete pViewport;
            return true;
        }
        return false;
    });
}

void ViewportsOrchestrator::CloneViewportToLeastClutteredWindow(Viewport *param1)
{
    int              bestCount  = 999999;
    IPlatformWindow *bestWindow = nullptr;

    for (auto &it : Application::Instance()->GetAllWindows())
    {
        int viewportsCount = CountViewports(it);

        if (viewportsCount < bestCount)
        {
            bestCount  = viewportsCount;
            bestWindow = it;
        }
    }

    assert(bestWindow);

    static int counter = 1;

    std::string name = std::format("Viewport {0}", counter++);

    AddNewViewport(name.c_str(), bestWindow, param1);
}

void ViewportsOrchestrator::EnsureAtLeastOneViewportExists()
{
    bool hasVisible = false;

    for (auto &vp : m_lstViewports)
    {
        if (vp->IsVisible())
        {
            hasVisible = true;
            break;
        }
    }

    if (hasVisible)
        return;

    auto dockSides = Application::GetMainWindow()->GetDockSides();

    Viewport *pTarget = nullptr;

    if (m_lstViewports.size() > 0)
    {
        auto it       = m_lstViewports.begin();
        auto viewport = *it;

        pTarget = viewport;
    }
    else
        pTarget = AddNewViewport("Main", Application::GetMainWindow(), 0);

    pTarget->SetVisible(true);
    ImGui::DockBuilderDockWindow(pTarget->Name(), dockSides->idDockCenter);
    Application::GetMainWindow()->UpdateDocks();
}

Viewport *ViewportsOrchestrator::GetHoveredViewport()
{
    return m_pHoveredViewport;
}

Viewport *ViewportsOrchestrator::GetFocusedViewport()
{
    for (auto & it: m_lstViewports)
    {
        if (it->IsFocused() && it->IsVisible())
        {
            return it;
        }
    }

    return nullptr;
}

SharedFBOLeaf *ViewportsOrchestrator::AllocateSharedFBOViewport(Viewport * pViewport, glm::vec2 extents)
{
    btClock sampler;

    auto oldLeaf = pViewport->GetSharedFBOLeaf();

    for (auto & it1: m_lstViewports)
    {
        if (!it1->GetSharedFBOLeaf())
            continue;

        for (auto &it2 : m_lstViewports)
        {
            if (!it2->GetSharedFBOLeaf())
                continue;

            if (it1->GetSharedFBOLeaf() == it2->GetSharedFBOLeaf() && it1 != it2)
                __debugbreak();
        }
    }

    if (oldLeaf)
        oldLeaf->Release();


    SharedFBOLeaf *pLeaf = nullptr;

    for (auto &it : m_lstSharedFBO)
    {
        pLeaf = it->AllocateViewport(extents);

        if (pLeaf)
        {
            break;            
        }
    }

    if (!pLeaf)
    {
        // Allocate new shared FBO
        AttachmentTypes      fboTypes[] = {AttachmentTypes::RGB, AttachmentTypes::R32UI};
        GLFramebufferObject *pGLFBO     = new GLFramebufferObject(2048, 2048, 2, fboTypes);

        SharedFBO *pNewFBO = new SharedFBO(pGLFBO);
        m_lstSharedFBO.push_back(pNewFBO);
        pLeaf = pNewFBO->AllocateViewport(extents);
        assert(pLeaf);
    }

    for (auto & it : m_lstViewports)
    {
        if (it == pViewport)
            continue;
        
        if (it->GetSharedFBOLeaf() == pLeaf)
            __debugbreak();
    }

    glm::vec2 pos = pLeaf->GetPosition();
    pViewport->SetSharedFBOLeaf(pLeaf);
    SortViewportsByFBO();

    Con_Printf("Placing %s to [%.3f,%.3f]\n", pViewport->Name(), pos.x, pos.y);

    return pLeaf;
}

void ViewportsOrchestrator::MakeViewportFocused(Viewport *param1)
{
    for (auto & it : m_lstViewports)
    {
        if (it == param1)
            continue;

        it->SetFocused(false);
    }

    param1->SetFocused(true);
}

void ViewportsOrchestrator::SortViewportsByFBO()
{
    m_lstViewports.sort([](Viewport *a, Viewport *b) {
        SharedFBOLeaf *pLeafA = a->GetSharedFBOLeaf();
        SharedFBOLeaf *pLeafB = b->GetSharedFBOLeaf();

        if (!pLeafA)
            return false;

        if (!pLeafB)
            return false;

        return pLeafA->GetGLTextureNum() > pLeafB->GetGLTextureNum();
    });
}

int ViewportsOrchestrator::CountViewports(const IPlatformWindow *it)
{
    int result = 0;

    for (auto &vp : m_lstViewports)
    {
        if (vp->GetPlatformWindow() == it)
            result++;
    }

    return result;
}

SharedFBO::SharedFBO(GLFramebufferObject *pFrameBuffer)
{
    m_pFBO      = pFrameBuffer;
    m_pRootNode = new SharedFBONode(this, nullptr, glm::vec2(0, 0), pFrameBuffer->Dimensions());
}

SharedFBO::~SharedFBO()
{
    for (auto &node : m_Nodes)
        delete node;

    for (auto &leaf : m_Leafs)
        delete leaf;
}

SharedFBONode *SharedFBO::PoolAllocateNode(glm::vec2 position, glm::vec2 extents)
{
    SharedFBONode *pResult = nullptr;

    for (auto &it : m_Nodes)
    {
        if (it->IsFree())
        {
            pResult = it;
            break;
        }
    }

    if (!pResult)
    {
        pResult = new SharedFBONode(this, nullptr, position, extents);
        m_Nodes.push_back(pResult);
    }
    else
    {
        pResult->SetParent(nullptr);
        pResult->SetPosition(position);
        pResult->SetExtents(extents);
        pResult->Lock();
    }

    return pResult;
}

SharedFBOLeaf *SharedFBO::PoolAllocateLeaf(SharedFBONode *parent, glm::vec2 pos, glm::vec2 extents)
{
    SharedFBOLeaf *pResult = nullptr;

    for (auto &it : m_Leafs)
    {
        if (it->IsFree())
        {
            pResult = it;
            break;
        }
    }

    if (!pResult)
    {
        pResult = new SharedFBOLeaf(m_pFBO, pos, extents, parent);
        m_Leafs.push_back(pResult);
    }
    else
    {
        pResult->SetParent(parent);
        pResult->SetPosition(pos);
        pResult->SetExtents(extents);
        pResult->Lock();
    }

    return pResult;
}

SharedFBOLeaf *SharedFBO::AllocateViewport(glm::vec2 extents)
{
    return m_pRootNode->AllocateLeaf(extents);
}

SharedFBONode::SharedFBONode(SharedFBO *pFBO, SharedFBONode *parent, glm::vec2 position, glm::vec2 extents)
    : ISharedFBOTreeItem(position, extents, m_pParent)
{
    m_pSharedFBO = pFBO;
    m_pParent    = parent;
    m_Position   = position;
    m_Extents    = extents;

    Lock();
}

SharedFBOLeaf *SharedFBONode::AllocateLeaf(glm::vec2 extents)
{
    if (m_Extents.x == extents.x && m_Extents.y == extents.y)
    {
        SharedFBOLeaf *pLeaf = m_pSharedFBO->PoolAllocateLeaf(this, m_Position, extents);

        m_pChildren[0] = m_pChildren[1] = pLeaf;

        return pLeaf;
    }

    bool checkFreeSpace = true;

    if (!m_pChildren[0])
    {
        if (extents.y != m_Extents.y)
        {
            Split(SplitAxis::Vertical, extents.y);
        }
        else
        {
            Split(SplitAxis::Horizontal, extents.x);
        }

        checkFreeSpace = false;
    }

    for (int i = 0; i < 2; i++)
    {
        if (m_pChildren[i]->IsLeaf())
            continue;

        SharedFBONode *pChildNode = static_cast<SharedFBONode *>(m_pChildren[i]);

        // It is faster to check extents first
        if (!pChildNode->RectFits(extents))
            continue;

        if (checkFreeSpace && !pChildNode->HasFreeSpace())
            continue;

        return pChildNode->AllocateLeaf(extents);
    }

    return nullptr;
}

bool SharedFBONode::IsLeaf()
{
    return false;
}

void SharedFBONode::SetPosition(glm::vec2 position)
{
    m_Position = position;
}

void SharedFBONode::SetExtents(glm::vec2 extents)
{
    m_Extents = extents;
}

void SharedFBONode::OnLeafReleased()
{
    m_pChildren[0] = m_pChildren[1] = 0;

    // Paranoid check
    if (m_pParent)
    {
        SharedFBONode *pParent = static_cast<SharedFBONode *>(m_pParent);
        pParent->CondenseFreeSpace(this);
    }
}

bool SharedFBONode::HasLeafs()
{
    if (!m_pChildren[0])
        return false;

    // if node has children - it is pointed by both children

    if (m_pChildren[0]->IsLeaf())
        return true;

    // Otherwise - check child nodes
    SharedFBONode *pChild1 = static_cast<SharedFBONode *>(m_pChildren[0]);
    SharedFBONode *pChild2 = static_cast<SharedFBONode *>(m_pChildren[1]);

    return pChild1->HasLeafs() || pChild2->HasLeafs();
}

void SharedFBONode::CondenseFreeSpace(SharedFBONode *pEmptyNode)
{
    // Paranoid check
    if (!m_pChildren[0])
        return;

    // if node has children - it is pointed by both children

    if (m_pChildren[0]->IsLeaf())
        return;

    for (int i = 0; i < 2; i++)
    {
        SharedFBONode *pChild = static_cast<SharedFBONode *>(m_pChildren[i]);

        // Skip checking empty node

        if (pChild == pEmptyNode)
            continue;

        // If other node is also empty - we can condense space

        if (!pChild->HasLeafs())
        {
            m_pChildren[0]->Release();
            m_pChildren[1]->Release();

            m_pChildren[0] = nullptr;
            m_pChildren[1] = nullptr;

            // Paranoid check
            if (m_pParent)
            {
                SharedFBONode *pParent = static_cast<SharedFBONode *>(m_pParent);
                pParent->CondenseFreeSpace(this);
            }

            return;
        }
    }
}

void SharedFBONode::Split(SplitAxis axis, float pos)
{
    switch (axis)
    {
    case SplitAxis::Horizontal:

        m_pChildren[0] = m_pSharedFBO->PoolAllocateNode(m_Position, {pos, m_Extents.y});
        m_pChildren[1] =
            m_pSharedFBO->PoolAllocateNode(m_Position + glm::vec2(pos, 0), {m_Extents.x - pos, m_Extents.y});

        break;
    case SplitAxis::Vertical:

        m_pChildren[0] = m_pSharedFBO->PoolAllocateNode(m_Position, {m_Extents.x, pos});
        m_pChildren[1] =
            m_pSharedFBO->PoolAllocateNode(m_Position + glm::vec2(0, pos), {m_Extents.x, m_Extents.y - pos});

        break;
    }

    m_pChildren[0]->SetParent(this);
    m_pChildren[1]->SetParent(this);
}

bool SharedFBONode::HasFreeSpace()
{
    if (!m_pChildren[0])
        return true;

    if (!m_pChildren[0]->IsLeaf())
    {
        SharedFBONode *pChild1 = static_cast<SharedFBONode *>(m_pChildren[0]);
        SharedFBONode *pChild2 = static_cast<SharedFBONode *>(m_pChildren[1]);

        return pChild1->HasFreeSpace() || pChild2->HasFreeSpace();
    }

    return false;
}

SharedFBOLeaf::SharedFBOLeaf(GLFramebufferObject *pFrameBuffer, const glm::vec2 position, const glm::vec2 extents,
                             SharedFBONode *pParent)
    : ISharedFBOTreeItem(position, extents, pParent)
{
    m_pFBO = pFrameBuffer;
    Lock();
}

bool SharedFBOLeaf::IsLeaf()
{
    return true;
}

void SharedFBOLeaf::SetPosition(glm::vec2 position)
{
    m_Position = position;
}

void SharedFBOLeaf::SetExtents(glm::vec2 extents)
{
    m_Extents = extents;
}

glm::vec2 SharedFBOLeaf::GetPosition()
{
    return m_Position;
}

void SharedFBOLeaf::EnableFBO()
{
    m_pFBO->Enable();
}

void SharedFBOLeaf::DisableFBO()
{
    m_pFBO->Disable();
}

void SharedFBOLeaf::ApplyGLViewport()
{
    glViewport(m_Position.x, m_Position.y, m_Extents.x, m_Extents.y);
}

GLuint SharedFBOLeaf::GetGLTextureNum()
{
    return m_pFBO->ColorTexture()->GLTextureNum(0);
}

ISharedFBOTreeItem::ISharedFBOTreeItem(glm::vec2 position, glm::vec2 extents, ISharedFBOTreeItem *parent)
{
    m_Position = position;
    m_pParent  = parent;
    m_Extents  = extents;
}

bool ISharedFBOTreeItem::RectFits(glm::vec2 extents)
{
    return m_Extents.x >= extents.x && m_Extents.y >= extents.y;
}

void ISharedFBOTreeItem::Lock()
{
    m_bFree = false;
}

void ISharedFBOTreeItem::Release()
{
    m_bFree = true;

    if (IsLeaf() && m_pParent)
    {
        auto node = static_cast<SharedFBONode *>(m_pParent);
        node->OnLeafReleased();
    }
}

bool ISharedFBOTreeItem::IsFree()
{
    return m_bFree;
}
