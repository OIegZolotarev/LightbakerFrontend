#pragma once
#include "main_window.h"

class Application
{
	MainWindow* m_pMainWindow = nullptr;

	Application();
public:
	// Конструкторы\доступ
	~Application();
	static Application* Instance();

	// Функционал
	void Run();
	void Init(std::string cmdLine);
};

