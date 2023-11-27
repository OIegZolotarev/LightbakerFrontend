/*
    LightBaker3000 Frontend project,
    (c) 2023 CrazyRussian
*/

#include "imgui_helpers.h"
#include "application.h"
#include "imgui_internal.h"
#include "lb3k_imgui_icons.cpp"
#include "lb3k_imgui_icons.h"
#include "ui_common.h"

void ImGuiHelpers::Init()
{
    ImGui::GetIO().Fonts->AddFontDefault();
    static const ImWchar icons_ranges[] = {ICON_MIN_lb3k, ICON_MAX_lb3k, 0};

    // static const ImWchar icons_ranges[] = {58000, 58014, 0};

    ImFontConfig icons_config;
    icons_config.MergeMode        = true;
    icons_config.OversampleH      = 1;
    icons_config.GlyphMinAdvanceX = 8;

    ImGui::GetIO().Fonts->AddFontFromMemoryCompressedBase85TTF(lb3k_compressed_data_base85, 21, &icons_config,
                                                               icons_ranges);
}

bool ImGuiHelpers::ImageButtonWithText(ImTextureID texId, const char *label, const ImVec2 &imageSize, const ImVec2 &uv0,
                                       const ImVec2 &uv1, int frame_padding, const ImVec4 &bg_col,
                                       const ImVec4 &tint_col)
{
    using namespace ImGui;

    ImGuiWindow *window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImVec2 size = imageSize;
    if (size.x <= 0 && size.y <= 0)
    {
        size.x = size.y = ImGui::GetTextLineHeightWithSpacing();
    }
    else
    {
        if (size.x <= 0)
            size.x = size.y;
        else if (size.y <= 0)
            size.y = size.x;
        size.x *= window->FontWindowScale * ImGui::GetIO().FontGlobalScale;
        size.y *= window->FontWindowScale * ImGui::GetIO().FontGlobalScale;
    }

    ImGuiContext &g         = *GImGui;
    const ImGuiStyle &style = g.Style;

    const ImGuiID id      = window->GetID(label);
    const ImVec2 textSize = ImGui::CalcTextSize(label, NULL, true);
    const bool hasText    = textSize.x > 0;

    const float innerSpacing =
        hasText ? ((frame_padding >= 0) ? (float)frame_padding : (style.ItemInnerSpacing.x)) : 0.f;
    const ImVec2 padding =
        (frame_padding >= 0) ? ImVec2((float)frame_padding, (float)frame_padding) : style.FramePadding;
    const ImVec2 totalSizeWithoutPadding(size.x + innerSpacing + textSize.x, size.y > textSize.y ? size.y : textSize.y);
    // const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + totalSizeWithoutPadding + padding * 2);
    ImRect bb;
    bb.Min = window->DC.CursorPos;
    bb.Max = window->DC.CursorPos;
    bb.Max.x += totalSizeWithoutPadding.x + padding.x * 2;
    bb.Max.y += totalSizeWithoutPadding.y + padding.y * 2;

    ImVec2 start(0, 0);
    start = window->DC.CursorPos;
    start.x += padding.x;
    start.y += padding.y;

    if (size.y < textSize.y)
        start.y += (textSize.y - size.y) * .5f;
    ImRect image_bb;

    image_bb.Min = start;
    image_bb.Max = start;
    image_bb.Max.x += size.x;
    image_bb.Max.y += size.y;

    start = window->DC.CursorPos;
    start.x += padding.x;
    start.y += padding.y;

    start.x += size.x + innerSpacing;
    if (size.y > textSize.y)
        start.y += (size.y - textSize.y) * .5f;
    ItemSize(bb);
    if (!ItemAdd(bb, id))
        return false;

    bool hovered = false, held = false;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    // Render

    const ImU32 col = GetColorU32((hovered && held) ? ImGuiCol_ButtonActive
                                  : hovered         ? ImGuiCol_ButtonHovered
                                                    : ImGuiCol_Border);

    RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, style.FrameRounding));

    window->DrawList->AddImage(texId, image_bb.Min, image_bb.Max, uv0, uv1, GetColorU32(tint_col));

    if (textSize.x > 0)
        ImGui::RenderText(start, label);
    return pressed;
}

bool ImGuiHelpers::ButtonWithCommonIcon(CommonIcons icon, const char *label, const float icon_size,
                                        ImVec4 tint /*= {1,1,1,1}*/)
{
    auto icn    = GetCommonIcon(icon);
    auto style   = ImGui::GetStyle();
    auto bgColor = style.Colors[ImGuiCol_Button];

    return ImGuiHelpers::ImageButtonWithText((ImTextureID)icn->GLTextureNum(), label, ImVec2(icon_size, icon_size),
                                             ImVec2(1, 1), ImVec2(0, 0), style.FramePadding.x, bgColor, tint);
}
