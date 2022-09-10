#include "application.h"
#include "../tiny-process-library/process.hpp"
#include <thread>
#include <corecrt_malloc.h>

Application::Application()
{
	m_pCommandsRegistry = new CCommandsRegistry;
	m_pFileSystem = new FileSystem;
}

Application::~Application()
{
	if (m_pMainWindow) delete m_pMainWindow;
	delete m_pFileSystem;
}

void Application::Run()
{
	m_pMainWindow = new MainWindow("LightBaker3000 FrontEnd", glm::vec2(1280, 720));
	m_pMainWindow->MainLoop();
}

void Application::Init(std::string cmdLine)
{
	
}

MainWindow* Application::GetMainWindow()
{
	return Instance()->m_pMainWindow;
}

FileSystem* Application::GetFileSystem()
{
	return Instance()->m_pFileSystem;
}

void Application::EPICFAIL(const char* format, ...)
{
	char tmp[4096];

	va_list argp;
	va_start(argp, format);
	vsprintf_s(tmp, sizeof(tmp), format, argp);
	va_end(argp);

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "EPIC FAIL", tmp, Application::GetMainWindow()->Handle());
	exit(1);
}

void Application::ExecuteBaking()
{
	if (!m_pMainWindow->GetSceneRenderer()->IsModelLoaded())
		return;


	const char* cmdTemplate = "%s/lb3k/LightBaker3000.exe %s %s -samples %d -rnm -rgba16";

	auto fileName	 = m_pMainWindow->GetSceneRenderer()->GetModelFileName();
	auto diffuseName = m_pMainWindow->GetSceneRenderer()->GetModelTextureName();

	m_pFileSystem->ChangeCurrentDirectoryToFileDirectory(fileName);
	
	char cmd[4096];
	sprintf_s(cmd, cmdTemplate,
		SDL_GetBasePath() ,
		fileName.c_str(), 
		diffuseName.c_str(),256);

	Con_Printf("[FRONTEND] Baking started\n");
	Con_Printf("[FRONTEND] cmd: %s\n",cmd);


	TinyProcessLib::Process process1a(cmd, "", [](const char* bytes, size_t n)
		{
			char* dup = (char*)malloc(sizeof(char) * (n + 1));
			memcpy(dup, bytes, n);
			dup[n] = 0;
			Con_Printf(dup);
			free(dup);
		});

	process1a.get_exit_status();

	m_pMainWindow->GetSceneRenderer()->ReloadModel();

	Con_Printf("[FRONTEND] Baking finished");
}

Application* Application::Instance()
{
	static Application* sInstance = new Application;
	return sInstance;
}

CCommandsRegistry* Application::CommandsRegistry()
{
	return Instance()->m_pCommandsRegistry;
}
