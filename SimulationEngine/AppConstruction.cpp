#include "Application.h"

Application* Application::GetInstance()
{
	// If the application already exists then return that.
	if (Application::m_pInstance != nullptr)
	{
		return m_pInstance;
	}

	// Otherwise, construct the application.
	m_pInstance = new Application();
	return m_pInstance;
}

Application::Application(void)
{
	m_dWindowWidth = 0;
	m_dWindowHeight = 0;
	m_bIsMinimized = false;
	m_bHasFocus = false;
	m_pOnResize = 0;
}

Application::~Application(void)
{

}

Application& Application::operator =(const Application& a_pApp)
{
	Application app = Application();
	return app;
}

Application::Application(const Application& a_pApp)
{

}

void Application::Release(void)
{
	delete m_pInstance;
}

void Application::Run(void)
{
	while (true)
	{

	}
}