/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/



#include "common.h"
#include "application.h"
#include "loader_thread.h"
#include <windows.h>

// TODO: recompile those as x64

#ifdef _DEBUG
#pragma comment(lib,"../lib/SDL2_Debug.lib")
#else
#pragma comment(lib,"../lib/SDL2_Release.lib")
#endif

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"Setupapi.lib")
#pragma comment(lib,"version.lib")

void RedirectIOToConsole();

#include <iostream>

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{

	RedirectIOToConsole();
		
	Application* app = Application::Instance();

	LoaderThread::Instance()->Start();

	app->Init(lpCmdLine);
	app->InitMainWindow();
	app->Run();

	LoaderThread::Instance()->Stop();

	delete Application::Instance();
	delete LoaderThread::Instance();

}


#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#ifndef _USE_OLD_IOSTREAMS
using namespace std;
#endif
// maximum mumber of lines the output console should have
static const WORD MAX_CONSOLE_LINES = 500;

void RedirectIOToConsole()
{
	FILE* conin = stdin;
	FILE* conout = stdout;
	FILE* conerr = stderr;
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen_s(&conin, "CONIN$", "r", stdin);
	freopen_s(&conout, "CONOUT$", "w", stdout);
	freopen_s(&conerr, "CONOUT$", "w", stderr);
	
	
}

//End of File
