/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "common.h"
#include "commands_registry.h"
#include "ui_common.h"
#include "..\include\imgui\imgui_internal.h"
#include "application.h"
#include "text_utils.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

namespace ImGui
{
bool ImageButtonWithText(ImTextureID texId, const char* label, const ImVec2& imageSize, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col) {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImVec2 size = imageSize;
	if (size.x <= 0 && size.y <= 0) { size.x = size.y = ImGui::GetTextLineHeightWithSpacing(); }
	else {
		if (size.x <= 0)          size.x = size.y;
		else if (size.y <= 0)     size.y = size.x;
		size.x *= window->FontWindowScale * ImGui::GetIO().FontGlobalScale;
		size.y *= window->FontWindowScale * ImGui::GetIO().FontGlobalScale;
	}

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;

	const ImGuiID id = window->GetID(label);
	const ImVec2 textSize = ImGui::CalcTextSize(label, NULL, true);
	const bool hasText = textSize.x > 0;

	const float innerSpacing = hasText ? ((frame_padding >= 0) ? (float)frame_padding : (style.ItemInnerSpacing.x)) : 0.f;
	const ImVec2 padding = (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
	const ImVec2 totalSizeWithoutPadding(size.x + innerSpacing + textSize.x, size.y > textSize.y ? size.y : textSize.y);
	//const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + totalSizeWithoutPadding + padding * 2);
	ImRect bb;
	bb.Min = window->DC.CursorPos;
	bb.Max = window->DC.CursorPos;
	bb.Max.x += totalSizeWithoutPadding.x + padding.x * 2;
	bb.Max.y += totalSizeWithoutPadding.y + padding.y * 2;

	ImVec2 start(0, 0);
	start = window->DC.CursorPos;
	start.x	+= padding.x; 
	start.y += padding.y;
	
	if (size.y < textSize.y) start.y += (textSize.y - size.y) * .5f;
	ImRect image_bb;
	
	image_bb.Min = start;
	image_bb.Max = start;
	image_bb.Max.x += size.x;
	image_bb.Max.y += size.y;
	
	start = window->DC.CursorPos;
	start.x += padding.x;
	start.y += padding.y;
	
	
	start.x += size.x + innerSpacing; if (size.y > textSize.y) start.y += (size.y - textSize.y) * .5f;
	ItemSize(bb);
	if (!ItemAdd(bb, id))
		return false;

	bool hovered = false, held = false;
	bool pressed = ButtonBehavior(bb, id, &hovered, &held);

	// Render
	const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
	RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));
	if (bg_col.w > 0.0f)
		window->DrawList->AddRectFilled(image_bb.Min, image_bb.Max, GetColorU32(bg_col));

	window->DrawList->AddImage(texId, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));

	if (textSize.x > 0) ImGui::RenderText(start, label);
	return pressed;
}
}

CCommandsRegistry::CCommandsRegistry()
{
	m_vecCommandDescriptor.clear();
}

CCommandsRegistry::~CCommandsRegistry()
{
	ClearPointersVector(m_vecCommandDescriptor);
}

void CCommandsRegistry::RegisterCommand(CCommand* pCommand)
{
	m_vecCommandDescriptor.push_back(pCommand);

	if (pCommand->Flags() & CMD_ON_MAINTOOLBAR)
		m_vecMainToolbarCommands.push_back(m_vecCommandDescriptor.size() - 1);
}

std::vector<int>& CCommandsRegistry::GetMainToolbarCommands()
{
	return m_vecMainToolbarCommands;
}

CCommand* CCommandsRegistry::GetCommandByInternalIndex(int index)
{
	return m_vecCommandDescriptor[index];
}

CCommand* CCommandsRegistry::FindCommandByGlobalId(GlobalCommands id)
{
	for (auto& it : m_vecCommandDescriptor)
	{
		if (it->GetId() == id)
			return it;
	}

	return 0;
}

void CCommandsRegistry::RenderCommandAsMenuItem(GlobalCommands cmdId, bool checked)
{
	CCommand* cmd = FindCommandByGlobalId(cmdId);
	assert(cmd);

	bool tmp2 = checked;

	if (ImGui::MenuItem(cmd->GetDescription(), cmd->GetShortcutDescription(),&tmp2))
	{
		cmd->Execute();
	}
}

bool CCommandsRegistry::OnKeyDown()
{
	auto kbState = SDL_GetKeyboardState(nullptr);

	for (auto& cmd : m_vecCommandDescriptor)
	{
		if (cmd->IsKeystrokeActive(kbState))
		{
			cmd->Execute();
			return true;
		}
	}

	return false;
}

CCommand::CCommand(GlobalCommands id, const char* description,const char* keyStroke, gltexture_t* icon, int flags, pfnCommandCallback callback) :
	m_eCommandId(id),
	m_pIcon(icon),
	m_iFlags(flags),
	m_pfnCallback(callback)
{
	strcpy_s(m_szDescription,description);
	if (keyStroke)
		strcpy_s(m_szKeyStroke, keyStroke);
	else
		*m_szKeyStroke = 0;


	for (auto& it : m_rKeys)
		it = -1;

	if (keyStroke)
	{ 
		auto items = TextUtils::SplitTextSimple(keyStroke, strlen(keyStroke), '-');

		auto addKey = [&](int keyId) -> bool
		{
			int  i = 0;
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

			auto keyCode = SDL_GetKeyFromName((*it).c_str());
			auto scanCode = SDL_GetScancodeFromKey(keyCode);

			// Stupid workaround for RUS keyboard layout
			if ((*it).c_str()[0] == '`')
			{
				keyCode = 1073742048;
				scanCode = SDL_SCANCODE_GRAVE;
			}

			if (keyCode == SDLK_UNKNOWN)
				Application::EPICFAIL("Bad shortcut for command \"%s\" - unknown key %s", m_szDescription, (*it).c_str());

			if (scanCode == SDLK_UNKNOWN)
				Application::EPICFAIL("Bad shortcut for command \"%s\" - unknown key %s", m_szDescription, (*it).c_str());

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
	ImGui::SameLine();

	if (m_iFlags & CMD_ONLY_ICON)
	{
		if (ImGui::ImageButton(m_szDescription, (ImTextureID)m_pIcon->gl_texnum, ImVec2(size, size)))
			Execute();
	}
	else
	{
		auto style = ImGui::GetStyle();
		auto bgColor = style.Colors[ImGuiCol_Button];
		if (ImGui::ImageButtonWithText((ImTextureID)m_pIcon->gl_texnum, m_szDescription, ImVec2(size, size), ImVec2(1, 1), ImVec2(0, 0), style.FramePadding.x, bgColor, ImVec4(1, 1, 1, 1)))
		{
			Execute();
		}
	}


}

GlobalCommands CCommand::GetId()
{
	return m_eCommandId;
}

const char* CCommand::GetDescription()
{
	return m_szDescription;
}

bool CCommand::IsKeystrokeActive(const uint8_t* kbState)
{
	if (m_rKeys[0] == -1) // No hotkey assigned
		return false;

	bool bValidStroke = true;

	for (auto& key : m_rKeys)
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

const char* CCommand::GetShortcutDescription()
{
	return m_szKeyStroke;
}
