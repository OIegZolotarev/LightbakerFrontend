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
    // throw std::logic_error("The method or operation is not implemented.");
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
        }
    }

    if (pViewport == m_pDragViewport)
    {
        if (m_bMouseDragValid)
        {
            glm::vec2 p = pViewport->GetClientAreaPosAbs();

            glm::vec2 mins = (p + m_vecMouseDragStart);
            glm::vec2 maxs = (p + m_vecMouseDragEnd);

            for (int i = 0; i < 2; i++)
            {
                if (mins[i] > maxs[i])
                {
                    float t = maxs[i];
                    maxs[i] = mins[i];
                    mins[i] = t;
                }
            }

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