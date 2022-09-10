#pragma once
#include "main_window.h"
#include "commands_registry.h"
#include "file_system.h"

class Application
{
	MainWindow*			m_pMainWindow			= nullptr;
	CCommandsRegistry*	m_pCommandsRegistry		= nullptr;
	FileSystem*			m_pFileSystem			= nullptr;

	Application();

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
};

inline void Con_Printf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	Application::Console()->AddLog(fmt, args);
	va_end(args);
}