#include <iostream>
#include "Graphics.h"
#include "Application.h"
#include "Simulation.h"
#include "Input.h"

namespace
{
	// Setting a base Simulation variable.
	Application* pApp = Application::GetInstance();

	// Simple callback function for window resizing.
	void WindowResize()
	{
		Simulation* simulation = pApp->GetSimulation();

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
	// Seeding the random object.
	srand(static_cast<unsigned int>(time(0)));

	// Cosntructing the Application.
	Application* app = Application::GetInstance();

#if defined(DEBUG) | defined(_DEBUG)
	app->CreateConsoleWindow(500, 120, 32, 120);
	std::cout << "Debug window created.  Output here!" << std::endl;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// App/Api initialization details:
	unsigned int dWindowWidth = 1280;
	unsigned int dWindowHeight = 720;
	const wchar_t* cWindowTitle = L"DX-Engine";
	bool bVsync = false;

	// Creating the window context.
	HRESULT window = app->CreateWindowContext(
		a_hInstance,
		dWindowWidth,
		dWindowHeight,
		cWindowTitle,
		WindowResize
	);

	// Stopping the program early if the window creation failed.
	if (FAILED(window))
	{
		return window;
	}

	// Initialize the graphics API and verify
	HRESULT graphicsResult = Graphics::Initialize(
		app->GetWidth(),
		app->GetHeight(),
		app->GetHandle(),
		bVsync);

	if (FAILED(graphicsResult))
	{
		return graphicsResult;
	}

	// InitialiIng input for the application.
	Input::Initialize(app->GetHandle());

	// Running the Application.
	HRESULT result = app->Run();

	// Reallocating the memory used by the application.
	app->Release();

#if defined(DEBUG) | defined(_DEBUG)
	// Ensuring that there are no memory leaks.
	if (_CrtDumpMemoryLeaks())
	{
		std::cout << "ERROR: Memory leaks present." << std::endl;
	}
	/*Microsoft::WRL::ComPtr<ID3D11Debug> debug;
	Graphics::GetDevice().As(&debug);
	debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);*/

	// Stops the console window from closing so that logs can be read.
	//std::cin.get();
#endif

	return result;
}