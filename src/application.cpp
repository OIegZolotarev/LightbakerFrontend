#include "application.h"
#include "../tiny-process-library/process.hpp"
#include <thread>
#include <corecrt_malloc.h>
#include <SDL_thread.h>
#include <functional>
#include "properties_editor.h"

#include <nlohmann/json.hpp>

Application::Application()
{
	m_pCommandsRegistry =	new CCommandsRegistry;
	m_pFileSystem =			new FileSystem;	
	m_pPersistentStorage =	nullptr;
}

Application::~Application()
{
	if (m_pMainWindow) delete m_pMainWindow;
	delete m_pFileSystem;

	delete ObjectPropertiesEditor::Instance();
	delete m_pPersistentStorage;
}

void Application::Run()
{
	m_pPersistentStorage =	new PersistentStorage;
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

int BakeThread(void*args)
{
	TinyProcessLib::Process process1a((char*)args, "", [](const char* bytes, size_t n)
		{
			char* dup = (char*)malloc(sizeof(char) * (n + 1));
			memcpy(dup, bytes, n);
			dup[n] = 0;

			// Make progress output a bit nicer
			for (size_t i = 0; i <(n - 1); i++)
			{
				if (dup[i] == '\r' && dup[i + 1] != '\n')
					dup[i] = '\n';
			}
 						
			Con_Printf(dup);
			// Ќаверное довольно плохое решение =/ - 30 миллисекунд будет не всегда хватать (на более мощном процессоре\GPU чем у мен€);
			SDL_Delay(30);
			free(dup);
		});

	int code = process1a.get_exit_status();
	Application::Instance()->NotifyBakingFinished(code);

	return code;
}

void Application::ExecuteBaking()
{
	if (!m_pMainWindow->GetSceneRenderer()->IsModelLoaded())
		return;


	const char* cmdTemplate = "%s/lb3k/LightBaker3000.exe %s %s -samples %d -rnm -rgba16";

	auto fileName	 = m_pMainWindow->GetSceneRenderer()->GetModelFileName();
	auto diffuseName = m_pMainWindow->GetSceneRenderer()->GetModelTextureName();

	fileName = "edited.obj";
	diffuseName = "sample.png";

	char export_path[4096];
	sprintf_s(export_path, "%s/lb3k/%s", SDL_GetBasePath(), "edited.obj");

	m_pMainWindow->GetSceneRenderer()->ExportModelForCompiling(export_path);

	//m_pFileSystem->ChangeCurrentDirectoryToFileDirectory(fileName);
	
	static char cmd[4096];
	sprintf_s(cmd, cmdTemplate,
		SDL_GetBasePath() ,
		fileName.c_str(), 
		diffuseName.c_str(),256);

	Con_Printf("[FRONTEND] Baking started\n");
	Con_Printf("[FRONTEND] cmd: %s\n",cmd);

	m_bBakingFinished = false;
	m_bWaitingForBakingToFinish = true;
	SDL_Thread* threadID = SDL_CreateThread(BakeThread, "LightBaker3k thread", (void*)cmd);
	
	//Con_Printf("[FRONTEND] Baking finished");
}

void Application::CheckIfBakngFinished()
{
	if (!m_bWaitingForBakingToFinish && m_bBakingFinished)
		return;

	if (!m_bBakingFinished)
		return;

	m_bWaitingForBakingToFinish = false;
	Con_Printf("[FRONTEND] Baking finished");

	char export_path[4096];
	sprintf_s(export_path, "%s/lb3k/%s", SDL_GetBasePath(), "edited.obj");

	m_pMainWindow->GetSceneRenderer()->LoadModel(export_path);

	//m_pMainWindow->GetSceneRenderer()->ReloadModel();
}

void Application::NotifyBakingFinished(int code)
{
	m_bBakingFinished = true;
}

PersistentStorage* Application::GetPersistentStorage()
{
	return Instance()->m_pPersistentStorage;
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
