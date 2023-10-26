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

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	Application* app = Application::Instance();

	LoaderThread::Instance()->Start();

	app->Init(lpCmdLine);
	app->Run();

	LoaderThread::Instance()->Stop();

	delete Application::Instance();
	delete LoaderThread::Instance();
}