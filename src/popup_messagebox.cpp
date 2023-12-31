/*
    LightBaker3000 Frontend project,
    (c) 2024 CrazyRussian
*/

#include "application.h"
#include "imgui_popups.h"
#include "common.h"

#include "popup_messagebox.h"
#include "imgui_helpers.h"


PopupMessageBox::PopupMessageBox() : IImGUIPopup(PopupWindows::MesasgeBox)
{
    m_Icon        = MessageBoxIcons::None;
    m_bPersistent = false;
}

PopupMessageBox::~PopupMessageBox()
{
}

void PopupMessageBox::Render()
{
    if (ImGui::BeginChild("MessageBoxContent", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())))
    {
        ImGui::Spacing();
        ImGuiHelpers::DisplayToolbarIcon(ToolbarIcons::Clear, 48);
        ImGui::SameLine();
        ImGui::Text(m_Message.c_str());
    }

    ImGui::EndChild();

    auto displayButton = [&](int flag, const char *title, MessageBoxButtons id) {
        if (m_iButtonsFlags & flag)
        {
            if (ImGui::Button(title))
            {
                if (m_Callback)
                    m_Callback(id);
            }

            ImGui::SameLine();
        }
    };

    displayButton(MSG_BOX_OK, "Ok", MessageBoxButtons::Ok);
    displayButton(MSG_BOX_YES, "Yes", MessageBoxButtons::Yes);
    displayButton(MSG_BOX_NO, "No", MessageBoxButtons::No);
    displayButton(MSG_BOX_CANCEL, "Cancel", MessageBoxButtons::Cancel);
}

void PopupMessageBox::SetMessage(const char *message)
{
    m_Message = message;
}

void PopupMessageBox::SetTitle(const char *title)
{
    m_Title = title;
    m_Key   = title;
}

void PopupMessageBox::SetIcon(MessageBoxIcons icon)
{
    m_Icon = icon;
}

void PopupMessageBox::SetButtons(int flags)
{
    m_iButtonsFlags = flags;
}

void PopupMessageBox::SetCallback(pfnMessageBoxCallback callback)
{
    m_Callback = callback;
}

int PopupMessageBox::RenderingFlags()
{
    return ImGuiWindowFlags_NoResize;
}

void PopupMessageBox::Show()
{
    PopupsManager::Instance()->ShowPopup(this);
    SetVisible(true);
}

bool PopupMessageBox::BeginRendering()
{
    ImGui::SetNextWindowSize(ImVec2(350, 150));
    ImGui::SetNextWindowPos(ImGui::GetWindowViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    return IImGUIPopup::BeginRendering();
}
