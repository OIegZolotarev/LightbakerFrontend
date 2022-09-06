#pragma once
#include "main_window.h"
#include "commands_registry.h"

class Application
{
	MainWindow*			m_pMainWindow			= nullptr;
	CCommandsRegistry*	m_pCommandsRegistry		= nullptr;

	Application();

public:
	// ������������\������
	~Application();
	static Application*		Instance();

	CCommandsRegistry*		CommandsRegistry();
	

	// ����������
	void Run();
	void Init(std::string cmdLine);
};

