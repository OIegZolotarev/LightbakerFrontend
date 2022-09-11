#pragma once

#include "ui_common.h"

//-----------------------------------------------------------------------------
// [SECTION] Example App: Debug Log / ShowExampleAppLog()
//-----------------------------------------------------------------------------
// Usage:
//  static ExampleAppLog my_log;
//  my_log.AddLog("Hello %d world\n", 123);
//  my_log.Draw("title");
struct DebugConsole
{
	SDL_SpinLock lock = 0;

	ImGuiTextBuffer     Buf;
	ImGuiTextFilter     Filter;
	ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
	bool                AutoScroll;  // Keep scrolling if already at the bottom.
	
	DebugConsole();
	
	void    Clear();
	void    AddLog(const char* fmt, ...);
	void    AddLog(const char* fmt, va_list& args);
	void    Draw(const char* title, bool* p_open = NULL);
};