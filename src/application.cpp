/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

#include "application.h"
#include "../tiny-process-library/process.hpp"
#include <thread>
#include <corecrt_malloc.h>
#include <SDL_thread.h>
#include <functional>
#include "properties_editor.h"

#include <nlohmann/json.hpp>
#include "lb3k_wrapper.h"

Application::Application()
{
	m_pCommandsRegistry =	new CCommandsRegistry;
	m_pFileSystem =			new FileSystem;	
	m_pPersistentStorage =	nullptr;

	m_pLightBakerApplication = new LightBaker3000("lb3k/LightBaker3000.exe");
}

Application::~Application()
{
	if (m_pMainWindow) delete m_pMainWindow;
	delete m_pFileSystem;
		
	
	delete m_pPersistentStorage;
	delete m_pLightBakerApplication;
}

void Application::Run()
{
	m_pPersistentStorage =	new PersistentStorage(this);
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

	SDL_Window* window = nullptr;

	if (Application::GetMainWindow())
		window = Application::GetMainWindow()->Handle();

	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "EPIC FAIL", tmp, window);
	exit(1);
}



void Application::ExecuteBaking()
{
	auto sceneRenderer = m_pMainWindow->GetSceneRenderer();

	if (!sceneRenderer->IsModelLoaded())
		return;
	

	m_bBakingFinished = false;
	m_bWaitingForBakingToFinish = true;

	sceneRenderer->ExportModelForCompiling(nullptr);	
	m_pLightBakerApplication->ExecuteBaking(sceneRenderer->GetModelFileName().c_str());
	
/*	const char* cmdTemplate = "%s/lb3k/LightBaker3000.exe %s %s -samples %d -rnm -rgba16";

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
	
	//Con_Printf("[FRONTEND] Baking finished");*/
}

void Application::CheckIfBakngFinished()
{
	if (!m_bWaitingForBakingToFinish && m_bBakingFinished)
		return;

	if (!m_bBakingFinished)
		return;

	m_bWaitingForBakingToFinish = false;

	if (m_bDoBakingAgain)
	{
		m_bDoBakingAgain = false;
		ExecuteBaking();
		Con_Printf("[FRONTEND] Need to rebake scene");
	}
	else
	{
		Con_Printf("[FRONTEND] Baking finished");
		m_pMainWindow->GetSceneRenderer()->LoadModel(nullptr, true);
	}


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

bool Application::DelayedInitDone()
{
	return Instance()->m_bDelayedInitDone;
}

void Application::FlagDelayedInitDone()
{
	m_bDelayedInitDone = true;
}

LightBaker3000* Application::GetLightBakerApplication()
{
	return m_pLightBakerApplication;
}

void Application::ScheduleCompilationIfNecceseary()
{
	auto inst = Instance();

	if (!inst->GetPersistentStorage()->GetSettingBool(ApplicationSettings::DynamicallyRecompileLighting))
		return;

	if (!Application::IsWaitingForBakerToFinish())
	{
		

		auto settings = inst->GetLightBakerApplication()->Settings();
		int oldSize[2] = { settings->m_lmSettings.size[0],settings->m_lmSettings.size[1] };

		settings->m_lmSettings.size[0] = 128;
		settings->m_lmSettings.size[1] = 128;

		inst->ExecuteBaking();

		settings->m_lmSettings.size[0] = oldSize[0];
		settings->m_lmSettings.size[1] = oldSize[1];
	}
	else
		FlagToDoBakingAgain();
}

bool Application::IsWaitingForBakerToFinish()
{
	return Instance()->m_bWaitingForBakingToFinish;
}

void Application::FlagToDoBakingAgain()
{
	Instance()->m_bDoBakingAgain = true;
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
