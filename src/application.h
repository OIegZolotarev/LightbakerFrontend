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
	// ������������\������
	~Application();
	static Application*		Instance();

	CCommandsRegistry*		CommandsRegistry();
	

	// ����������
	void Run();
	void Init(std::string cmdLine);
	
	static MainWindow* GetMainWindow();
	static FileSystem* GetFileSystem();

	static void EPICFAIL(const char* fmt, ...);
};

