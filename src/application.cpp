#include "application.h"

Application::Application()
{
	m_pCommandsRegistry = new CCommandsRegistry;
}

Application::~Application()
{
	if (m_pMainWindow) delete m_pMainWindow;
}

void Application::Run()
{
	m_pMainWindow = new MainWindow("LightBaker3000 FrontEnd", glm::vec2(1280, 720));
	m_pMainWindow->MainLoop();
}

void Application::Init(std::string cmdLine)
{
	
}

Application* Application::Instance()
{
	static Application* sInstance = new Application;
	return sInstance;
}

CCommandsRegistry* Application::CommandsRegistry()
{
	return m_pCommandsRegistry;
}
