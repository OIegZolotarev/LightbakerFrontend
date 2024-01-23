/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/



#include "application.h"
#include "common.h"
#include "loader_thread.h"
#include <windows.h>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"Setupapi.lib")
#pragma comment(lib,"version.lib")

void RedirectIOToConsole();

#include <iostream>

void EnableCRTDebug()
{
    int tmp;

    // Get the current bits
    tmp = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);

    // Clear the upper 16 bits and OR in the desired frequency
    tmp = (tmp & 0x0000FFFF) | _CRTDBG_CHECK_EVERY_16_DF;

    // Set the new bits
    _CrtSetDbgFlag(tmp);
}

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    // EnableCRTDebug();
#ifdef DEBUG_TO_CONSOLE
	RedirectIOToConsole();
#endif
		
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
