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
#include <regex>



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
	auto scene = sceneRenderer->GetScene();

	std::string lmMeshFile = scene->ExportForCompiling(nullptr);

	if (!lmMeshFile.empty())
	{
		m_bBakingFinished = false;
		m_bWaitingForBakingToFinish = true;
		m_pLightBakerApplication->ExecuteBaking(lmMeshFile.c_str());
	}

// 	auto sceneRenderer = m_pMainWindow->GetSceneRenderer();
// 
// 	if (!sceneRenderer->IsModelLoaded())
// 		return;
// 	
// 

// 	
// 	auto modelName = sceneRenderer->GetModelFileName();
// 	auto baseDir = m_pFileSystem->BaseDirectoryFromFileName(modelName.c_str());
// 	auto fileName = m_pFileSystem->BaseName(modelName);
// 
// 	auto exportedModel = std::format("{0}/{1}_exported.obj", baseDir, fileName);
// 	
// 	sceneRenderer->ExportModelForCompiling(nullptr);	
// 	m_pLightBakerApplication->ExecuteBaking(exportedModel.c_str());
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
		m_pMainWindow->GetSceneRenderer()->ReloadScene(LRF_RELOAD_LIGHTMAPS);
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

	// TODO: FIXME
	return;

	if (!Application::IsWaitingForBakerToFinish())
	{
		

		auto settings = inst->GetLightBakerApplication()->Settings();
		int oldSize[2] = { settings->m_lmSettings.size[0],settings->m_lmSettings.size[1] };
		int oldSamples = settings->m_iSamples;

		settings->m_iSamples = 256;
		settings->m_lmSettings.size[0] = 512;
		settings->m_lmSettings.size[1] = 512;
		

		inst->ExecuteBaking();

		settings->m_iSamples = oldSamples;
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

void Application::ParseLightBakerProgressMessage(std::string & captured)
{
	//__debugbreak();
	//Con_Printf("C: %s", captured.c_str());
	m_strBakingStatus = captured;
}

void Application::ShowMouseCursor()
{
	ImGui::GetIO().MouseDrawCursor = true;
}

void Application::HideMouseCursor()
{
	ImGui::GetIO().MouseDrawCursor = false;
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
