/*
	LightBaker3000 Frontend project,
	(c) 2022 CrazyRussian
*/

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

	std::string m_strBakingStatus;
    std::string m_strFileToLoad = "";

    // Baker state
    float m_flBakingPercentage = 0;
    bool  m_bDoBakingAgain     = false;

	bool m_bTerminated = false;

	std::list<IPlatformWindow *> m_lstWindows;
    bool             m_bEventsRedirectionEnabled = false;
    IPlatformWindow *m_pEventsRedirectionTarget = nullptr; 	

	int SuggestMonitorForNewWindow();

public:
	// Конструкторы\доступ
	~Application();
	static Application*				Instance();

	static CCommandsRegistry*		CommandsRegistry();

	bool IsMouseCursorVisible();

	// Функционал
	void Run();
	void InitMainWindow();
	void Init(std::string cmdLine);
	
	static MainWindow* GetMainWindow();
	static FileSystem* GetFileSystem();

	static void EPICFAIL(const char* fmt, ...);

	static DebugConsole* Console();

	void ExecuteBaking();
	void CheckIfBakngFinished();
	void NotifyBakingFinished(int code);


	static PersistentStorage* GetPersistentStorage();
	static bool DelayedInitDone();

	void FlagDelayedInitDone();

	LightBaker3000* GetLightBakerApplication();
	
	static void	ScheduleCompilationIfNecceseary();
	static bool IsWaitingForBakerToFinish();
	static void FlagToDoBakingAgain();
	
	void ParseLightBakerProgressMessage(std::string & captured);
	float GetBakingProgress();

	std::string& BakingStatusMessage();
	void ShowMouseCursor();
	void HideMouseCursor();
	   
    static int Q_buildnum(void);

	static IPlatformWindow *FindPlatformWindow(size_t id);

	void Terminate();
    IPlatformWindow *FindWindowBySDLId(size_t sdlid);
    void             SetupEventsRedirection(bool enabled, IPlatformWindow *targetWindow);

	std::list<IPlatformWindow *> &GetAllWindows()
    {
        return m_lstWindows;
    }
};
 
 inline int Con_Printf(const char* fmt, ...)
 {
 	va_list args;
 	va_start(args, fmt);
 	Application::Console()->AddLog(fmt, args);
 	va_end(args);
 
 	return 1;
 }

#define DEBUG_TO_CONSOLE
#ifdef DEBUG_TO_CONSOLE
	#define Con_Printf printf
#endif


 #define SDL_ORGANIZATION "QuiteOldOrange"
 #define SDL_APP_NAME "LightBaker3000Frontend"
 
#define APP_VERSION "1.3.1.0"