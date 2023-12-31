/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "viewport.h"
#include "application.h"
#include "gl_screenspace_2d_renderer.h"
#include "imgui_internal.h"
#include "properties_editor.h"

glm::vec2 Viewport::CalcRelativeMousePos()
{
    ImVec2 s = ImGui::GetMousePos();
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

    // RenderGuizmo();

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

    if (ImGui::Begin(m_strName.c_str(), &m_bVisible, flags))
    {
        m_bDocked = ImGui::IsWindowDocked();



        auto r = GLScreenSpace2DRenderer::Instance();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);

        auto textureId = m_pFBO->ColorTexture()->GLTextureNum(0);

        ImVec2 viewportSize = ImGui::GetContentRegionAvail();

        m_ClientAreaSize = {viewportSize.x, viewportSize.y};

        float uv_x = viewportSize.x / (m_FrameBufferSize.x);
        float uv_y = viewportSize.y / (m_FrameBufferSize.y);

#if DEBUG_FBO_AREA_USAGE
        uv_x = 1;
        uv_y = 1;
#endif

        auto pos        = ImGui::GetCursorPos();
        m_ClientAreaPos = {pos.x, pos.y};

        ImGui::Image((ImTextureID *)textureId, viewportSize, ImVec2(0, uv_y), ImVec2(uv_x, 0), {1,1,1,1});

        if (Application::Instance()->IsMouseCursorVisible())
            m_bHovered = ImGui::IsItemHovered();
        
        UpdateDisplayWidgetPos();
        DisplayViewportUI(pos);

        RenderGuizmo();
    }

    OutputDebug();

    ImGui::End();

    HandlePicker();

    ImGui::PopID();
}

void Viewport::HandlePicker()
{
    BT_PROFILE("Viewport::HandlePicker()");

    auto ratPos = CalcRelativeMousePos();

    if (PointInClientRect(ratPos))
    {
        m_hoveredObjectId = ReadPixel(ratPos.x, ratPos.y);

        bool canSelect = !(SelectionManager::IsGizmoEnabled() && ImGuizmo::IsOver()) && m_bHovered;

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
        m_bHovered = false;

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
                m_RenderMode = it.first;
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

        if (ImGui::BeginMenu("Send to other window"))
        {
            auto & lstWindow = Application::Instance()->GetAllWindows();

            for (auto & it: lstWindow)
            {
                if (ImGui::MenuItem(it->GetDescription(),0, it == m_pPlatformWindow))
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

int Viewport::HandleEvent(bool bWasHandled, SDL_Event &e, float flFrameDelta)
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
    auto      pos = m_pCamera->GetOrigin();
    glm::vec3 spd = m_pCamera->GetAngles();

    bool bFPSNav = m_pCamera->IsFPSNavigationEngaged();

    ImGui::Text("%s : cam at [%f %f %f], ang: [%f %f %f], fps_nav: %d, hov: %d, docked: %d", m_strName.c_str(), pos.x,
                pos.y, pos.z, spd.x, spd.y, spd.z, bFPSNav, m_bHovered, m_bDocked);

    auto ratPos = CalcRelativeMousePos();

    if (PointInClientRect(ratPos))
    {
        int id = ReadPixel(ratPos.x, ratPos.y);
        ImGui::Text("Mouse: %f %f (%d)", ratPos.x, ratPos.y, id);
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

IPlatformWindow *Viewport::GetPlatformWindow()
{
    return m_pPlatformWindow;
}

void Viewport::DoCloneViewport()
{
    Application::GetMainWindow()->CloneViewport(this);
}
