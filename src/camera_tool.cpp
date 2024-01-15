/*
    LightBaker3000 Frontend project,
    (c) 2022-2024 CrazyRussian
*/

#include "application.h"
#include "common.h"

#include "camera_tool.h"

CameraTool::CameraTool() : IEditingTool(EditingToolId::Camera)
{
    SetBoundCommand(GlobalCommands::ActivateCameraTool);
    SetToolIcon(CommonIcons::CameraTool);
    SetDescritpion("Camera tool");
}

void CameraTool::Render(float flFrameDelta)
{
}

void CameraTool::RenderUI()
{
    if (!ImGui::Begin("Camera tool"))
    {
        ImGui::End();
        return;
    }

    if (!m_pActiveDocument)
    {
        ImGui::Text("Select a viewport");
        ImGui::End();
        return;
    }

    if (ImGui::Button("Remember current position"))
    {
        CameraController *cameraController = m_pActiveViewport->GetCamera();

        sceneCameraDescriptor_t newDescriptor;

        // TODO: fix this later...
        static int idx = 0;

        newDescriptor.position    = cameraController->GetPosition();
        newDescriptor.angles      = cameraController->GetAngles();
        newDescriptor.description = std::format("Camera {0}", ++idx);

        m_pActiveDocument->AddSceneCameraDescriptor(newDescriptor);
    }

    const CameraDescriptorsList &sceneCams = m_pActiveDocument->GetSceneCamerasDescriptors();

    ImGui::Text("Defined cameras:");

    static size_t selected = 0;
    size_t        idx      = 0;

    if (ImGui::BeginListBox("###Defined cameras:"))
    {
        for (auto &it : sceneCams)
        {
            if (ImGui::Selectable(it.description.c_str(), idx == selected))
            {
                selected = idx;

                m_pActiveViewport->LookAt(it);
                m_pActiveViewport->FlagUpdate();
            }

            idx++;
        }

        ImGui::EndListBox();
    }

    // If selection is valid - display controls
    if (selected < sceneCams.size())
    {
        sceneCameraDescriptor_t &desc = m_pActiveDocument->GetSceneCameraDescriptor(selected);

        ImGui::InputFloat3("Position", &desc.position.x);
        ImGui::InputFloat3("Angles", &desc.angles.x);
        ImGui::InputText("Description", &desc.description);
    }

    ImGui::End();
}
