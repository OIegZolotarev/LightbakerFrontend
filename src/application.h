#pragma once

#include "main_window.h"
#include "commands_registry.h"
#include "file_system.h"
#include "persistent.h"
#include "lb3k_wrapper.h"

class Application
{
	MainWindow*			m_pMainWindow			= nullptr;
	CCommandsRegistry*	m_pCommandsRegistry		= nullptr;
	FileSystem*			m_pFileSystem			= nullptr;

	Application();

	bool m_bWaitingForBakingToFinish = false;
	bool m_bBakingFinished = true;

	PersistentStorage*	m_pPersistentStorage;

	LightBaker3000*		m_pLightBakerApplication;

	bool m_bDelayedInitDone = false;

public:
	// Конструкторы\доступ
	~Application();
	static Application*				Instance();

	static CCommandsRegistry*		CommandsRegistry();
	

	// Функционал
	void Run();
	void Init(std::string cmdLine);
	
	static MainWindow* GetMainWindow();
	static FileSystem* GetFileSystem();

	static void EPICFAIL(const char* fmt, ...);

	static DebugConsole* Console()
	{
		return Instance()->GetMainWindow()->Console();
	}

	void ExecuteBaking();
	void CheckIfBakngFinished();
	void NotifyBakingFinished(int code);


	static PersistentStorage* GetPersistentStorage();
	static bool DelayedInitDone();

	void FlagDelayedInitDone();

	LightBaker3000* GetLightBakerApplication();
	static bool IsWaitingForBakerToFinish();
	static void FlagToDoBakingAgain();
	bool m_bDoBakingAgain = false;
};

inline void Con_Printf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	Application::Console()->AddLog(fmt, args);
	va_end(args);
}