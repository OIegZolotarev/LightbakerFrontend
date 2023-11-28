/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/
#include "application.h"
#include "common.h"

#include "inputfield_ex.h"
#include "imgui_helpers.h"
#include "imgui_listview_ex.h"
#include "popup_loadfile_dialog.h"

InputFieldEx::InputFieldEx(const char *title, std::string &container, unsigned int flags /*= 0*/) : m_Container(container)
{
    m_Header = title;
    m_ImGUIIdHeader = std::format("###{0}", m_Header);

    m_iFlags = flags;
}

InputFieldEx::~InputFieldEx()
{
}

void InputFieldEx::SetHeaderPosition(HeaderPosition pos)
{
    m_HeaderPos = pos;
}

void InputFieldEx::RenderGui()
{
    ImGui::PushID(this);

    switch (m_HeaderPos)
    {
    case HeaderPosition::Left:
        ImGui::Text(m_Header.c_str());
        ImGui::SameLine();
        ImGui::InputText(m_ImGUIIdHeader.c_str(), &m_Container);
        break;
    case HeaderPosition::Top:
        ImGui::Text(m_Header.c_str());
        ImGui::InputText(m_ImGUIIdHeader.c_str(), &m_Container);
        break;
    case HeaderPosition::None:
        ImGui::InputText(m_ImGUIIdHeader.c_str(), &m_Container);
        break;
    case HeaderPosition::Right:
    default:
        ImGui::InputText(m_Header.c_str(), &m_Container);
        break;
    }
    
    if (m_iFlags & FL_CHOOSE_FILE_BUTTON)
    {
        ImGui::SameLine();
        if (ImGuiHelpers::ButtonWithCommonIcon(CommonTextures::InputFieldMore, "###SelectFile", LV_TOOLBAR_ICON_SIZE))
            OnButtonPressed(Buttons::ChooseFile);
    }

    if (m_iFlags & FL_CLEAR_BUTTON)
    {
        ImGui::SameLine();
        if (ImGuiHelpers::ButtonWithCommonIcon(CommonTextures::InputFieldClear, "###ClearFile", LV_TOOLBAR_ICON_SIZE))
            OnButtonPressed(Buttons::Clear);
    }

    ImGui::PopID();
}

void InputFieldEx::SetFileDialogTitle(std::string title)
{
    m_FileDialogTitle = title;
}

void InputFieldEx::SetFileDialogFilter(std::string filter)
{
    m_FileDialogFilters = filter;
}

void InputFieldEx::OnButtonPressed(Buttons id)
{
    switch (id)
    {
    case Buttons::Clear:
        m_Container = "";
        break;
    case Buttons::ChooseFile: 
    {
        auto lfd = LoadFileDialog::Instance();

        lfd->SetTitle(m_FileDialogTitle.c_str());
        lfd->SetFilters(m_FileDialogFilters.c_str());
        lfd->SetOnSelectCallback([&](std::string &fileName) { 
            m_Container = fileName;
        });

        lfd->Show();
    }
        break;
    default:
        break;
    
    }
}
