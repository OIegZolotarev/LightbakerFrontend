#pragma once
#include "main_window.h"

class Application
{
	MainWindow* m_pMainWindow = nullptr;

	Application();
public:
	// ������������\������
	~Application();
	static Application* Instance();

	// ����������
	void Run();
	void Init(std::string cmdLine);
};

