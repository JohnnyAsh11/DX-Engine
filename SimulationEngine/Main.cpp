#include <iostream>
#include "Application.h"
#include "Simulation.h"

namespace
{
	// Setting a base Simulation variable.
	Simulation* simulation = 0;

	// Simple callback function for window resizing.
	void WindowResize()
	{
		// Using the simulation's resize function.
		if (simulation)
		{
			simulation->OnResize();
		}
	}
}


int WINAPI WinMain(
	_In_ HINSTANCE a_hInstance,
	_In_opt_ HINSTANCE a_hPrevInstance,
	_In_ LPSTR a_lpCmdLine,
	_In_ int a_nCmdShow)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// Cosntructing the Application.
	Application* app = Application::GetInstance();

	HRESULT window = app->CreateWindowContext(
		a_hInstance,
		1280,
		720,
		L"DX-Engine",
		WindowResize
	);

	// Stopping the program early if the window creation failed.
	if (FAILED(window))
	{
		return window;
	}

	// Running the Application.
	app->Run();

	// Reallocating the memory used by the application.
	app->Release();

	// Ensuring that there are no memory leaks.
	if (_CrtDumpMemoryLeaks())
	{
		std::cout << "ERROR: Memory leaks present." << std::endl;
	}

	return 0;
}