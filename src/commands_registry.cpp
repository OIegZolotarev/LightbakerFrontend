/*
    LightBaker3000 Frontend project,
    (c) 2022 CrazyRussian
*/

#include "commands_registry.h"
#include "common.h"
#include "ui_common.h"

#include "application.h"
#include "imgui_helpers.h"
#include "imgui_internal.h"
#include "text_utils.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

CCommandsRegistry::CCommandsRegistry()
{
    m_vecCommandDescriptor.clear();
}

CCommandsRegistry::~CCommandsRegistry()
{
    ClearPointersVector(m_vecCommandDescriptor);
}

void CCommandsRegistry::RegisterCommand(CCommand *pCommand)
{
    m_vecCommandDescriptor.push_back(pCommand);

    if (pCommand->Flags() & CMD_ON_MAINTOOLBAR)
        m_vecMainToolbarCommands.push_back(m_vecCommandDescriptor.size() - 1);
}

std::vector<size_t> &CCommandsRegistry::GetMainToolbarCommands()
{
    return m_vecMainToolbarCommands;
}

CCommand *CCommandsRegistry::GetCommandByInternalIndex(int index)
{
    return m_vecCommandDescriptor[index];
}

CCommand *CCommandsRegistry::FindCommandByGlobalId(GlobalCommands id)
{
    for (auto &it : m_vecCommandDescriptor)
    {
        if (it->GetId() == id)
            return it;
    }

    return 0;
}

void CCommandsRegistry::RenderCommandAsMenuItem(GlobalCommands cmdId, bool checked)
{
    CCommand *cmd = FindCommandByGlobalId(cmdId);
    assert(cmd);

    bool tmp2 = checked;

    if (ImGui::MenuItem(cmd->GetDescription(), cmd->GetShortcutDescription(), &tmp2))
    {
        cmd->Execute();
    }
}

bool CCommandsRegistry::OnKeyDown()
{
    auto kbState = SDL_GetKeyboardState(nullptr);

    for (auto &cmd : m_vecCommandDescriptor)
    {
        if (cmd->IsKeystrokeActive(kbState))
        {
            cmd->Execute();
            return true;
        }
    }

    return false;
}

CCommand::CCommand(GlobalCommands id, const char *description, const char *keyStroke, GLTexture *icon, int flags,
                   pfnCommandCallback callback)
    : m_eCommandId(id), m_pIcon(icon), m_iFlags(flags), m_pfnCallback(callback)
{
    strcpy_s(m_szDescription, description);
    if (keyStroke)
        strcpy_s(m_szKeyStroke, keyStroke);
    else
        *m_szKeyStroke = 0;

    for (auto &it : m_rKeys)
        it = -1;

    if (keyStroke)
    {
        auto items = TextUtils::SplitTextSimple(keyStroke, strlen(keyStroke), '-');

        auto addKey = [&](int keyId) -> bool {
            int i = 0;
            for (int i = 0; i < ARRAY_SIZE(m_rKeys); i++)
            {
                if (m_rKeys[i] == -1)
                {
                    m_rKeys[i] = keyId;
                    return true;
                }
            }

            return false;
        };

        auto it = items.begin();

        while (it != items.end())
        {
            bool r = false;

            auto scanCode = SDL_GetScancodeFromName((*it).c_str());

            if (scanCode == SDLK_UNKNOWN)
            {
                auto err = SDL_GetError();
                Application::EPICFAIL("Bad shortcut for command \"%s\" - unknown key %s", m_szDescription,
                                      (*it).c_str());
            }

            r = addKey(scanCode);

            if (!r)
                Application::EPICFAIL("Bad shortcut for command \"%s\" - key stroke is too long", m_szDescription);

            it++;
        }
    }
}

void CCommand::Execute()
{
    m_pfnCallback();
}

void CCommand::RenderImGUI(int size)
{
    // ImGUI quirks
#pragma warning(disable : 4312)
    ImGui::SameLine();

    if (m_iFlags & CMD_ONLY_ICON)
    {
        if (ImGui::ImageButton(m_szDescription, (ImTextureID)m_pIcon->GLTextureNum(), ImVec2(size, size)))
            Execute();
    }
    else
    {
        auto style   = ImGui::GetStyle();
        auto bgColor = style.Colors[ImGuiCol_Button];

        // if (ImGui::ImageButton(m_szDescription, (ImTextureID)m_pIcon->gl_texnum, ImVec2(size, size)))
        if (ImGuiHelpers::ImageButtonWithText((ImTextureID)m_pIcon->GLTextureNum(), m_szDescription, ImVec2(size, size),
                                              ImVec2(1, 1), ImVec2(0, 0), style.FramePadding.x, bgColor,
                                              ImVec4(1, 1, 1, 1)))
        {
            Execute();
        }
    }
#pragma warning(restore : 4312)
}

GlobalCommands CCommand::GetId()
{
    return m_eCommandId;
}

const char *CCommand::GetDescription()
{
    return m_szDescription;
}

bool CCommand::IsKeystrokeActive(const uint8_t *kbState)
{
    if (m_rKeys[0] == -1) // No hotkey assigned
        return false;

    bool bValidStroke = true;

    for (auto &key : m_rKeys)
    {
        if (key == -1) // Reached end of key combo
            break;

        if (!kbState[key])
        {
            bValidStroke = false;
            break;
        }
    }

    return bValidStroke;
}

const char *CCommand::GetShortcutDescription()
{
    return m_szKeyStroke;
}
