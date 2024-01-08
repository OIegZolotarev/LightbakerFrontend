/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "viewport.h"
#include "editing_toolbox.h"
#include "gl_screenspace_2d_renderer.h"
#include "imgui_internal.h"
#include "properties_editor.h"
#include "viewports_orchestrator.h"

glm::vec2 Viewport::CalcRelativeMousePos(bool yAtTop)
{
    ImVec2 s = ImGui::GetMousePos();

    if (yAtTop)
        return {s.x - m_DisplayWidgetPosition.x, (s.y - m_DisplayWidgetPosition.y)};
    else
        return {s.x - m_DisplayWidgetPosition.x, m_ClientAreaSize.y - (s.y - m_DisplayWidgetPosition.y)};
}

bool Viewport::PointInClientRect(glm::vec2 pt)
{
    return (pt.x >= 0 && pt.x < m_ClientAreaSize.x) && (pt.y >= 0 && pt.y < m_ClientAreaSize.y);
}

int Viewport::ReadPixel(unsigned int x, unsigned int y)
{
    int r[4];

    m_pFBO->Enable();
    glReadBuffer(GL_COLOR_ATTACHMENT1);

    glReadPixels(x, y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &r);

    glReadBuffer(GL_NONE);
    m_pFBO->Disable();

    return r[0];
}

Viewport::Viewport(const char *title, IPlatformWindow *pHostWindow, Viewport *pCopyFrom)
    : m_pPlatformWindow(pHostWindow)
{
    m_FrameBufferSize = {2048, 2048};

    // TODO: fix later
    static int counter = 0;

    AttachmentTypes fboTypes[] = {AttachmentTypes::RGB, AttachmentTypes::R32UI};

    // TODO: FBO pool for reusing by viewports
    m_pFBO = new GLFramebufferObject(m_FrameBufferSize.x, m_FrameBufferSize.y, 2, fboTypes);

    if (!title)
        m_strName = std::format("Viewport: {0}", (int)counter++);
    else
        m_strName = title;

    m_strNamePopupKey = m_strName + "_popup";

    m_pCamera = new Camera(this);

    if (pCopyFrom)
    {
        m_pCamera->CopyOrientation(pCopyFrom->GetCamera());
    }
}

Viewport::~Viewport()
{
    delete m_pCamera;
    delete m_pFBO;
}

void Viewport::RenderFrame(float flFrameDelta)
{
    BT_PROFILE("Viewport::RenderFrame()");

    m_pCamera->Apply(flFrameDelta);

    if (!m_bNeedUpdate)
        return;

    m_pFBO->Enable();
    GL_CheckForErrors();

// Debug FBO usage
#if DEBUG_FBO_AREA_USAGE
    glClearColor(1, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
#endif

    auto sr = Application::Instance()->GetMainWindow()->GetSceneRenderer();

    glViewport(0, 0, m_ClientAreaSize.x, m_ClientAreaSize.y);

    sr->RenderScene(this);
    GL_CheckForErrors();

    EditingToolbox::Instance()->RenderTool(flFrameDelta);

    m_pFBO->Disable();
    GL_CheckForErrors();

    m_bNeedUpdate = false;
}

void Viewport::RenderGuizmo()
{
    if (!SelectionManager::IsGizmoEnabled())
        return;

    ObjectPropertiesEditor::Instance()->RenderGuizmo(this);
}

void Viewport::DisplayRenderedFrame()
{
    if (!m_bVisible)
    {
        return;
    }

    // if (Application::GetMainWindow()->GetState() == 0)
    //        __debugbreak();

    ImGui::PushID(this);

    if (m_bDocked)
    {
        ImGuiWindowClass window_class;
        window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;

        ImGui::SetNextWindowClass(&window_class);
    }

    int flags = m_bDocked ? ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar : 0;

    if (m_bForceUndock)
    {
        flags |= ImGuiWindowFlags_NoDocking;
        m_bForceUndock = false;
    }

    // flags |= ImGuiWindowFlags_NoInputs;

    if (ImGui::Begin(m_strName.c_str(), &m_bVisible, flags))
    {
        m_bDocked = ImGui::IsWindowDocked();

        auto r = GLScreenSpace2DRenderer::Instance();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);

        auto textureId = m_pFBO->ColorTexture()->GLTextureNum(0);

        ImVec2 viewportSize = ImGui::GetContentRegionAvail();

        if (abs(viewportSize.x - m_ClientAreaSize.x) > 0.5f || abs(viewportSize.y - m_ClientAreaSize.y) > 0.5f)
            m_bNeedUpdate = true;

        m_ClientAreaSize = {viewportSize.x, viewportSize.y};

        float uv_x = viewportSize.x / (m_FrameBufferSize.x);
        float uv_y = viewportSize.y / (m_FrameBufferSize.y);

#if DEBUG_FBO_AREA_USAGE
        uv_x = 1;
        uv_y = 1;
#endif

        auto pos        = ImGui::GetCursorPos();
        m_ClientAreaPos = {pos.x, pos.y};

        ImGui::Image((ImTextureID *)textureId, viewportSize, ImVec2(0, uv_y), ImVec2(uv_x, 0), {1, 1, 1, 1});
        // ImGui::SetHoveredID((ImGuiID) this);

        if (Application::Instance()->IsMouseCursorVisible())
            m_bHoveredImGUI = ImGui::IsItemHovered();

        auto ratPos = CalcRelativeMousePos();

        if (PointInClientRect(ratPos))
        {
            m_bHovered = true;
        }
        else
        {
            m_bHovered      = false;
            m_bHoveredImGUI = false;
        }

        UpdateDisplayWidgetPos();

        DisplayViewportUI(pos);
        RenderGuizmo();
        
        EditingToolbox::Instance()->RenderToolViewportUI(this);
    }

    OutputDebug();

    ImGui::End();

    HandlePicker();

    ImGui::PopID();
}

void Viewport::HandlePicker()
{
    BT_PROFILE("Viewport::HandlePicker()");

    auto ratPos = CalcRelativeMousePos(false);

    if (PointInClientRect(ratPos))
    {
        m_hoveredObjectId = ReadPixel(ratPos.x, ratPos.y);

        bool canSelect = !(SelectionManager::IsGizmoEnabled() && ImGuizmo::IsOver()) && m_bHovered && m_bHoveredImGUI;

        canSelect = false;

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && canSelect)
        {
            FlagUpdate();

            auto sr    = Application::GetMainWindow()->GetSceneRenderer();
            auto scene = sr->GetScene();

            auto obj = scene->GetEntityBySerialNumber(m_hoveredObjectId);
            auto ptr = obj.lock();

            if (ptr)
            {
                ptr->OnSelect(obj);
            }
            else
            {
                ObjectPropertiesEditor::Instance()->UnloadObject();
            }
        }
    }
}

void Viewport::DisplayViewportUI(ImVec2 pos)
{
    ImGui::SetCursorPos(pos);

    if (ImGui::Button("3D Textured"))
    {
        ImGui::OpenPopup(m_strNamePopupKey.c_str());
    }

    if (ImGui::IsItemHovered())
        m_bHoveredImGUI = false;

    if (ImGui::BeginPopup(m_strNamePopupKey.c_str()))
    {
        ImGui::SeparatorText("Shading");

        std::pair<RenderMode, const char *> menuItems[] = {{RenderMode::Lightshaded, "Lightshaded"},
                                                           {RenderMode::Unshaded, "Unshaded"},
                                                           {RenderMode::Flatshaded, "Flatshaded"},
                                                           {RenderMode::Wireframe, "Wireframe shaded"}};

        for (auto &it : menuItems)
        {
            if (ImGui::MenuItem(it.second, 0, m_RenderMode == it.first))
            {
                m_RenderMode  = it.first;
                m_bNeedUpdate = true;
            }
        }

        ImGui::SeparatorText("2D View");
        ImGui::MenuItem("Top");
        ImGui::MenuItem("Side");
        ImGui::MenuItem("Front");

        ImGui::Separator();

        if (ImGui::MenuItem("Clone"))
        {
            DoCloneViewport();
        }

        if (ImGui::MenuItem("Clone to new window"))
        {
            // FIXME: when extra window is opened - viepwort cloned to second to last window
            auto cmd = Application::CommandsRegistry()->FindCommandByGlobalId(GlobalCommands::OpenNewWindow);
            cmd->Execute();

            DoCloneViewport();
        }

        if (ImGui::BeginMenu("Send to other window"))
        {
            auto &lstWindow = Application::Instance()->GetAllWindows();

            for (auto &it : lstWindow)
            {
                if (ImGui::MenuItem(it->GetDescription(), 0, it == m_pPlatformWindow))
                {
                    if (it != m_pPlatformWindow)
                    {
                        m_pPlatformWindow->RemoveEventHandler(this);
                        m_pPlatformWindow = it;
                        m_pPlatformWindow->AddEventHandler(this);
                    }
                }
            }

            ImGui::EndMenu();
        }

        if (m_bDocked)
        {
            if (ImGui::MenuItem("Undock"))
            {
                m_bForceUndock = true;
                m_bDocked      = false;
            }
        }

        ImGui::EndPopup();
    }
}

void Viewport::UpdateDisplayWidgetPos()
{
    ImVec2 vMin = ImGui::GetWindowContentRegionMin();
    //  ImVec2 vMax = ImGui::GetWindowContentRegionMax();

    vMin.x += ImGui::GetWindowPos().x;
    vMin.y += ImGui::GetWindowPos().y;
    //     vMax.x += ImGui::GetWindowPos().x;
    //     vMax.y += ImGui::GetWindowPos().y;

    m_DisplayWidgetPosition = {vMin.x, vMin.y};
}

int Viewport::HandleEvent(bool bWasHandled, const SDL_Event &e, const float flFrameDelta)
{
    // Rendering logic will handle m_bHovered flag properly
    if (m_bHovered)
    {
        m_pCamera->HandleEvent(bWasHandled, e, flFrameDelta);
    }

    return 0;
}

Camera *Viewport::GetCamera()
{
    return m_pCamera;
}

RenderMode Viewport::GetRenderMode()
{
    return m_RenderMode;
}

void Viewport::OutputDebug()
{
    auto      position = m_pCamera->GetOrigin();
    glm::vec3 angles = m_pCamera->GetAngles();

    bool bFPSNav = m_pCamera->IsFPSNavigationEngaged();

    

    ImGui::Text("%s : cam at [%.3f %.3f %.3f], ang: [%.3f %.3f %.3f],\nfps_nav: %d, hov: %d, hov (imgui): %d ,docked: %d",
                m_strName.c_str(), position.x, position.y, position.z, angles.x, angles.y, angles.z, bFPSNav, m_bHovered, m_bHoveredImGUI,
                m_bDocked);

    auto ratPos = CalcRelativeMousePos(false);

    if (PointInClientRect(ratPos))
    {
        // int id = ReadPixel(ratPos.x, ratPos.y);
        ImGui::Text("Mouse: %f %f (%d)", ratPos.x, ratPos.y, m_hoveredObjectId);

    }

    
}

const char *Viewport::Name()
{
    return m_strName.c_str();
}

glm::vec2 Viewport::GetClientArea()
{
    return m_ClientAreaSize;
}

void Viewport::SaveState(nlohmann::json &persistentData)
{
    persistentData["Name"]       = m_strName;
    persistentData["Visible"]    = m_bVisible;
    persistentData["Docked"]     = m_bDocked;
    persistentData["HostWindow"] = m_pPlatformWindow->GetId();
}

Viewport *Viewport::LoadState(nlohmann::json &persistentData)
{
    std::string name    = persistentData["Name"];
    bool        visible = persistentData["Visible"];
    bool        docked  = persistentData["Docked"];
    size_t      id      = persistentData["HostWindow"];

    IPlatformWindow *pWindow = Application::FindPlatformWindow(id);

    Viewport *pResult = new Viewport(name.c_str(), pWindow, nullptr);

    pResult->m_bVisible = visible;
    pResult->m_bDocked  = docked;

    return pResult;
}

void Viewport::RegisterEventHandlerAtHost()
{
    assert(m_pPlatformWindow);
    m_pPlatformWindow->AddEventHandler(this);
}

bool Viewport::IsVisible()
{
    return m_bVisible;
}

glm::vec2 Viewport::GetClientAreaPosAbs()
{
    return m_DisplayWidgetPosition + m_ClientAreaPos;
}

void Viewport::FlagUpdate()
{
    m_bNeedUpdate = true;
}

size_t Viewport::GetHoveredObjectID()
{
    return m_hoveredObjectId;
}

void Viewport::SetVisible(bool flag)
{
    m_bVisible = true;
}

ViewportMouseHover Viewport::GetMouseHoveringStatus()
{
    if (!m_bHovered && !m_bHoveredImGUI)
        return ViewportMouseHover::NotHovered;
    else if (m_bHovered && !m_bHoveredImGUI)
        return ViewportMouseHover::HoveredButObstructedWithUI;

    return ViewportMouseHover::Hovered;
}

const glm::vec3 Viewport::ScreenToWorld(glm::vec2 viewportCoords, float depthFraction) const
{
 
    #if 0
     glm::vec3 winCoords = glm::vec3(viewportCoords.xy, depthFraction);
     glm::vec4 viewport  = {0, 0, m_ClientAreaSize.x, m_ClientAreaSize.y};
 
     return glm::unProject(winCoords, m_pCamera->GetViewMatrix(), m_pCamera->GetProjectionMatrix(), viewport);
     #endif


    // assert(depthFraction >= -1 && depthFraction <= 1);

    glm::vec3 ndc = {};

    ndc.x = viewportCoords.x / m_ClientAreaSize.x;
    ndc.y = viewportCoords.y / m_ClientAreaSize.y;
    ndc.z = depthFraction;

    // Convert from [0,1] to [-1,1] range
    ndc.x = (ndc.x - 0.5f) * 2;
    ndc.y = (ndc.y - 0.5f) * 2;

    //return ndc;
    return m_pCamera->ScreenToWorld(ndc);
}

IPlatformWindow *Viewport::GetPlatformWindow()
{
    return m_pPlatformWindow;
}

void Viewport::DoCloneViewport()
{
    ViewportsOrchestrator::Instance()->CloneViewportToLeastClutteredWindow(this);
}
