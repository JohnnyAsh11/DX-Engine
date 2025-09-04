#include "Application.h"
#include "Graphics.h"

Application* Application::m_pInstance = nullptr;

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
	SafeDelete(m_pSimulation);
}

void Application::Release(void)
{
	SafeDelete(m_pInstance);
}

HRESULT Application::Run(void)
{
	// Time tracking variables.
	LARGE_INTEGER perfFreq{ };
	double perfSeconds = 0;
	__int64 startTime = 0;
	__int64 currentTime = 0;
	__int64 previousTime = 0;

	// Query for accurate timing information.
	QueryPerformanceFrequency(&perfFreq);
	perfSeconds = 1.0 / (double)perfFreq.QuadPart;

	// Performance Counter gives high-resolution time stamps.
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
	currentTime = startTime;
	previousTime = startTime;

	// Initializing the simulation.
	m_pSimulation->Init();

	// Core application loop.
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		// Checking for messages from the operating system.
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Sending them through our message system.
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Setting and recalculating delta time and total time.
			QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
			float fDeltaTime = max((float)((currentTime - previousTime) * perfSeconds), 0.0f);
			float fTotalTime = (float)((currentTime - startTime) * perfSeconds);
			previousTime = currentTime;
			
			// Calling the simulation update and render methods.
			m_pSimulation->Update(fDeltaTime);
			m_pSimulation->Draw(fDeltaTime);

#if defined(DEBUG) || defined(_DEBUG)
			// Print any graphics debug messages that occurred this frame
			Graphics::DebugLog();
#endif
		}
	}

	return (HRESULT)msg.wParam;
}