/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "properties_editor.h"
#include "selection_tool.h"
#include "viewports_orchestrator.h"

SelectionTool::SelectionTool() : IEditingTool(EditingToolId::Selection)
{
    SetBoundCommand(GlobalCommands::ActivateSelectionTool);
    SetToolIcon(CommonIcons::SelectTool);
    SetDescritpion("Selection tool");

    m_pSelectionBoxColor = Application::GetPersistentStorage()->GetSetting(ApplicationSettings::SelectionBoxColor);
}

void SelectionTool::Render(float flFrameDelta)
{
    m_BoxSelectionFrustum.DrawDebug();
}

void SelectionTool::RenderUI()
{
    if (!ImGui::Begin(GetDescrition(), 0, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::SetCursorPosX(ImGui::CalcTextSize(GetDescrition()).x + 30);
        ImGui::End();
        return;
    }

    RenderModeSelectorUI();

    Viewport *pHoveredViewport = ViewportsOrchestrator::Instance()->GetHoveredViewport();

    if (pHoveredViewport)
    {
        ImGui::Text("Hovering above %s", pHoveredViewport->Name());
    }
    else
        ImGui::Text("Not hovering above viewport...");

    ImGui::Text("Ctrl status: %d", m_bCtrlHeld);

    // Hack to make sure window fits longer titles
    // https://discourse.dearimgui.org/t/how-can-make-window-size-elongate-with-the-long-title/128/2
    ImGui::SetCursorPosX(ImGui::CalcTextSize(GetDescrition()).x + 30);

    ImGui::End();
}

int SelectionTool::HandleKeyboardEvent(bool bWasHandled, const SDL_Event &e, const float flFrameDelta)
{
    switch (e.type)
    {
    case SDL_KEYDOWN:
        return HandleKeydownEvent(e, flFrameDelta);
    case SDL_KEYUP:
        return HandleKeyupEvent(e, flFrameDelta);
    }

    return 0;
}

int SelectionTool::HandleMouseEvent(bool bWasHandled, const SDL_Event &e, const float flFrameDelta)
{
    int buttons = e.button.button;

    switch (e.type)
    {
    case SDL_MOUSEBUTTONDOWN:

        if (buttons & SDL_BUTTON_LMASK)
            return HandleLeftClick();
    case SDL_MOUSEBUTTONUP:

        if (buttons & SDL_BUTTON_LMASK)
            return HandleLeftRelease();
    }

    return 0;
}

int SelectionTool::HandleLeftRelease()
{
    if (m_SelectionMode == BoxSelection)
    {
        if (m_bMouseDragged)
        {
            m_bMouseDragged   = false;
            m_vecMouseDragEnd = m_pActiveViewport->CalcRelativeMousePos();

            ValidateMinsMaxs();
            UpdateBoxSelectionFrustum();

            SelectItemsInSelectionFrustum();

            return EVENT_CONSUMED;
        }
    }

    return 0;
}

int SelectionTool::HandleLeftClick()
{
    switch (m_SelectionMode)
    {
    case Picker: {
        assert(m_pActiveViewport);
        assert(m_pActiveDocument);

        bool canSelect = !(SelectionManager::IsGizmoEnabled() && ImGuizmo::IsOver());

        // If got there - we are definitly hovered
        if (canSelect)
            SelectHoveredObject();
    }
    break;
    case BoxSelection: {
        m_pDragViewport     = m_pActiveViewport;
        m_bMouseDragValid   = true;
        m_bMouseDragged     = true;
        m_vecMouseDragStart = m_pActiveViewport->CalcRelativeMousePos();
        m_vecMouseDragEnd   = m_pActiveViewport->CalcRelativeMousePos();
        return EVENT_CONSUMED;
    }
    break;
    case Paint:
        break;
    default:
        break;
    }

    return 0;
}

void SelectionTool::SelectHoveredObject()
{
    auto obj = m_pActiveDocument->GetEntityBySerialNumber(m_pActiveViewport->GetHoveredObjectID());

    if (!obj.expired())
    {
        ObjectPropertiesEditor::Instance()->LoadObject(obj, m_bCtrlHeld);
        m_pActiveViewport->FlagUpdate();
    }
    else
    {
        ObjectPropertiesEditor::Instance()->UnloadObject();
    }
}

void SelectionTool::UpdateBoxSelectionFrustum()
{
    int lookupIndicies[4][2] = {
        {0, 0}, // top left
        {1, 0}, // top right
        {1, 1}, // bottom right
        {0, 1}, // bottom left
    };

    glm::vec2 screenPoints[2] = {m_vecMouseDragStart, m_vecMouseDragEnd};
    glm::vec3 nearestPoints[4];
    glm::vec3 farthestPoints[4];

    glm::vec2 p = m_pActiveViewport->GetClientAreaPosAbs();

    for (int i = 0; i < 4; i++)
    {
        glm::vec2 testPoint = {screenPoints[lookupIndicies[i][0]].x, screenPoints[lookupIndicies[i][1]].y};

        // testPoint -= p;

        nearestPoints[i]  = m_pActiveViewport->ScreenToWorld(testPoint, -1, false);
        farthestPoints[i] = m_pActiveViewport->ScreenToWorld(testPoint, 1, false);
    }

#define TOP_LEFT     0
#define TOP_RIGHT    1
#define BOTTOM_RIGHT 2
#define BOTTOM_LEFT  3

    glm::vec3 p1, p2, p3;

    p1 = nearestPoints[TOP_LEFT];
    p2 = farthestPoints[TOP_LEFT];
    p3 = farthestPoints[BOTTOM_LEFT];

    m_BoxSelectionFrustum.SetPlane(FrustumPlanes::Left, p1, p2, p3);

    p1 = nearestPoints[TOP_RIGHT];
    p2 = farthestPoints[TOP_RIGHT];
    p3 = farthestPoints[BOTTOM_RIGHT];

    m_BoxSelectionFrustum.SetPlane(FrustumPlanes::Right, p1, p2, p3);

    p1 = nearestPoints[TOP_LEFT];
    p2 = nearestPoints[TOP_RIGHT];
    p3 = farthestPoints[TOP_RIGHT];

    m_BoxSelectionFrustum.SetPlane(FrustumPlanes::Top, p1, p2, p3);

    p1 = nearestPoints[BOTTOM_LEFT];
    p2 = nearestPoints[BOTTOM_RIGHT];
    p3 = farthestPoints[BOTTOM_RIGHT];

    m_BoxSelectionFrustum.SetPlane(FrustumPlanes::Bottom, p1, p2, p3);

    p1 = nearestPoints[TOP_LEFT];
    p2 = nearestPoints[TOP_RIGHT];
    p3 = nearestPoints[BOTTOM_RIGHT];

    m_BoxSelectionFrustum.SetPlane(FrustumPlanes::NearZ, p1, p2, p3);

    p1 = farthestPoints[TOP_LEFT];
    p2 = farthestPoints[TOP_RIGHT];
    p3 = farthestPoints[BOTTOM_RIGHT];

    m_BoxSelectionFrustum.SetPlane(FrustumPlanes::FarZ, p1, p2, p3);



    m_BoxSelectionFrustum.LimitFarZDist(100.f);
}

void SelectionTool::SelectItemsInSelectionFrustum()
{
    assert(m_pActiveDocument);

    BVHTree *pTree = m_pActiveDocument->GetBVHTree();
    BVHNode *pNode = pTree->GetRootNode();

    if (!pNode)
        return;

    FrustumVisiblity visibility = m_BoxSelectionFrustum.CullBoxEx(pNode->aabb);

    if (visibility != FrustumVisiblity::None)
    {
        TraverseSelectionBoxFrustum(pTree, pNode, visibility);
    }

    m_pActiveViewport->FlagUpdate();
}

void SelectionTool::TraverseSelectionBoxFrustum(BVHTree *pTree, BVHNode *pNode, FrustumVisiblity parentVisibility)
{
    FrustumVisiblity myVisilibity = parentVisibility;

    // If our parent not completely visible - we need to check further if we are visible
    // otherwise it is not necessary and slows down execution
    if (myVisilibity != FrustumVisiblity::Complete)
    {
        myVisilibity = m_BoxSelectionFrustum.CullBoxEx(pNode->aabb);

        if (myVisilibity == FrustumVisiblity::None)
            return;
    }

    if (pNode->IsLeaf())
    {
        SceneEntityWeakPtr p = pNode->entity;
        ObjectPropertiesEditor::Instance()->LoadObject(p, m_bCtrlHeld);
    }
    else
    {
        if (pNode->left != -1)
            TraverseSelectionBoxFrustum(pTree, pTree->GetNodePtr(pNode->left), myVisilibity);

        if (pNode->right != -1)
            TraverseSelectionBoxFrustum(pTree, pTree->GetNodePtr(pNode->right), myVisilibity);
    }
}

void SelectionTool::ValidateMinsMaxs()
{
    for (int i = 0; i < 2; i++)
    {
        if (m_vecMouseDragStart[i] > m_vecMouseDragEnd[i])
        {
            float t                = m_vecMouseDragEnd[i];
            m_vecMouseDragEnd[i]   = m_vecMouseDragStart[i];
            m_vecMouseDragStart[i] = t;
        }
    }
}

void SelectionTool::RenderViewportUI(Viewport *pViewport)
{
    if (!m_pActiveViewport)
    {
        m_bMouseDragged = false;
    }

    if (m_pActiveViewport == m_pDragViewport)
    {
        if (m_bMouseDragged)
        {
            m_vecMouseDragEnd = m_pActiveViewport->CalcRelativeMousePos();
            // ValidateMinsMaxs();
        }
    }

    if (pViewport == m_pDragViewport)
    {
        if (m_bMouseDragValid)
        {
            glm::vec2 p = pViewport->GetClientAreaPosAbs();

            glm::vec2 mins = (p + m_vecMouseDragStart);
            glm::vec2 maxs = (p + m_vecMouseDragEnd);

            int boxColor = ToColorU32(m_pSelectionBoxColor->GetColorRGBA());

            auto drawDashedLine = [&](glm::vec2 start, glm::vec2 end) {
                float dist      = glm::length(end - start);
                float stepCount = glm::min(dist / 10.f, 1000.f);

                auto drawList = ImGui::GetWindowDrawList();

                for (int j = 0; j < stepCount; j++)
                {
                    float stepLength = 1.f / (float)stepCount;
                    float t1         = (float)j * stepLength;
                    float t2         = (float)j * stepLength + stepLength * 0.5f;

                    if (t2 > 1)
                        t2 = 1;

                    glm::vec2 pt1 = glm::lerp(start, end, glm::vec2(t1, t1));
                    glm::vec2 pt2 = glm::lerp(start, end, glm::vec2(t2, t2));

                    drawList->AddLine(FromGLMVec2(pt1), FromGLMVec2(pt2), boxColor, 2.f);
                }
            };

            glm::vec2 pt1 = mins;
            glm::vec2 pt2 = {maxs.x, mins.y};
            glm::vec2 pt3 = maxs;
            glm::vec2 pt4 = {mins.x, maxs.y};

            drawDashedLine(pt1, pt2);
            drawDashedLine(pt2, pt3);
            drawDashedLine(pt3, pt4);
            drawDashedLine(pt4, pt1);
        }
    }
}

int SelectionTool::HandleKeydownEvent(const SDL_Event &e, const float flFrameDelta)
{
    auto scancode = e.key.keysym.scancode;

    if (scancode == SDL_SCANCODE_LCTRL || scancode == SDL_SCANCODE_RCTRL)
    {
        m_bCtrlHeld = true;
        return EVENT_CONSUMED;
    }

    return 0;
}

int SelectionTool::HandleKeyupEvent(const SDL_Event &e, const float flFrameDelta)
{
    auto scancode = e.key.keysym.scancode;

    if (scancode == SDL_SCANCODE_LCTRL || scancode == SDL_SCANCODE_RCTRL)
    {
        m_bCtrlHeld = false;
        return EVENT_CONSUMED;
    }

    return 0;
}

void SelectionTool::RenderModeSelectorUI()
{
    std::pair<const char *, SelectionModes> modes[] = {
        {"Picker", SelectionModes::Picker},
        {"Paint", SelectionModes::Paint},
        {"Box", SelectionModes::BoxSelection},
    };

    ImGui::Text("Selection mode:");

    for (auto &it : modes)
    {
        if (ImGui::RadioButton(it.first, m_SelectionMode == it.second))
        {
            m_SelectionMode = it.second;

            // Make sure to reset box selection
            if (m_SelectionMode != SelectionModes::BoxSelection)
            {
                m_bMouseDragged   = false;
                m_bMouseDragValid = false;
            }
        }
    }
}