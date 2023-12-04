/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "application.h"
#include "viewport.h"
#include "gl_screenspace_2d_renderer.h"
#include "imgui_internal.h"


 Viewport::Viewport(AnchoringCorner anchoringBits)
{
     m_Extents = {2048, 2048};

     // TODO: fix later
     static int counter = 0;

     AttachmentTypes fboTypes[] = {AttachmentTypes::RGBA, AttachmentTypes::R32UI};

     m_pFBO    = new GLFramebufferObject(m_Extents.x, m_Extents.y, 2, fboTypes);
     m_strName = std::format("Viewport: {0}", (int)counter++);

     m_pCamera = new Camera(this);
 }

Viewport::~Viewport()
{
    delete m_pCamera;
    delete m_pFBO;
}

void Viewport::RenderFrame(float flFrameDelta)
{
    m_pFBO->Enable();   
    m_pCamera->Apply(flFrameDelta);

    glClear(GL_COLOR_BUFFER_BIT);
    auto sr = Application::Instance()->GetMainWindow()->GetSceneRenderer();

    glViewport(0, 0, m_ClientAreaSize.x, m_ClientAreaSize.y);

    
    sr->RenderScene(this);

    m_pFBO->Disable();
}

void Viewport::DisplayRenderedFrame()
{
    ImGui::PushID(this);

    int *viewport = Application::GetMainWindow()->Get3DGLViewport();

    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;

    ImGui::SetNextWindowClass(&window_class);

    if (ImGui::Begin(m_strName.c_str(), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar))
    {   
        

        if (Application::Instance()->IsMouseCursorVisible())
            m_bHovered = ImGui::IsWindowHovered();

        auto r = GLScreenSpace2DRenderer::Instance();

        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);

        //r->DrawQuad({10, 10}, {600, 600}, m_pFBO->ColorTexture());

        auto textureId = m_pFBO->ColorTexture()->GLTextureNum(0);

        ImVec2 viewportSize = ImGui::GetContentRegionAvail();

        m_ClientAreaSize = {viewportSize.x,viewportSize.y};

        float uv_x = viewportSize.x / (m_Extents.x);
        float uv_y = viewportSize.y / (m_Extents.y);

        ImGui::Image((ImTextureID *)textureId, viewportSize, ImVec2(0, uv_y), ImVec2(uv_x, 0));

        auto & style = ImGui::GetStyle();

        ImGui::SetCursorPos(style.FramePadding * 2.f);
        if (ImGui::Button("3D Textured"))
        {
        }

        ImGui::End();
    }

    ImGui::PopID();
}

int Viewport::HandleEvent(bool bWasHandled, SDL_Event &e, float flFrameDelta)
{
    if (m_bHovered)        m_pCamera->HandleEvent(bWasHandled, e, flFrameDelta);
    
    return 0;
}

RenderMode Viewport::GetRenderMode()
{
    return m_RenderMode;
}

void Viewport::OutputDebug()
{
    auto pos = m_pCamera->GetOrigin();
    glm::vec3 spd = m_pCamera->GetAngles();

    bool b = m_pCamera->IsFPSNavigationEngaged();

    ImGui::Text("%s : cam at [%f %f %f], ang: [%f %f %f], fps: %d, hov: %d", m_strName.c_str(), pos.x, pos.y, pos.z, spd.x,spd.y,spd.z, b, m_bHovered);
}

const char *Viewport::Name()
{
    return m_strName.c_str();
}

glm::vec2 Viewport::GetClientArea()
{
    return m_ClientAreaSize;
}
