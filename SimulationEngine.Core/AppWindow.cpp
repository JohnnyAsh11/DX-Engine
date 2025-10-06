#include "Application.h"
#include "Input.h"

HRESULT Application::CreateWindowContext(
	HINSTANCE a_AppInstance,
	unsigned int a_uWidth,
	unsigned int a_uHeight,
	std::wstring a_sTitleBar,
	void (*a_pOnResize)())
{
	// Verify that the window is not already created.
	if (m_bWindowCreated)
	{
		return E_FAIL;
	}

	// Set the fields of this Application.
	m_dWindowWidth = a_uWidth;
	m_dWindowHeight = a_uHeight;
	m_sWindowTitle = a_sTitleBar;
	m_pOnResize = a_pOnResize;

	WNDCLASS wndClass = {};
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = ProcessWindowsMessage;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = a_AppInstance;	
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);	
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = L"GraphicsWindowClass";

	if (!RegisterClass(&wndClass))
	{
		DWORD error = GetLastError();
		if (error != ERROR_CLASS_ALREADY_EXISTS)
		{
			return HRESULT_FROM_WIN32(error);
		}
	}

	RECT clientRect;
	SetRect(&clientRect, 0, 0, m_dWindowWidth, m_dWindowHeight);
	AdjustWindowRect(
		&clientRect,
		WS_OVERLAPPEDWINDOW,
		false);	

	RECT desktopRect;
	GetClientRect(GetDesktopWindow(), &desktopRect);
	int centeredX = (desktopRect.right / 2) - (clientRect.right / 2);
	int centeredY = (desktopRect.bottom / 2) - (clientRect.bottom / 2);

	m_WindowHandle = CreateWindow(
		wndClass.lpszClassName,
		m_sWindowTitle.c_str(),
		WS_OVERLAPPEDWINDOW,
		centeredX,
		centeredY,
		clientRect.right - clientRect.left,
		clientRect.bottom - clientRect.top,
		0,	
		0,			
		a_AppInstance,
		0);	

	if (m_WindowHandle == NULL)
	{
		DWORD error = GetLastError();
		return HRESULT_FROM_WIN32(error);
	}

	m_bWindowCreated = true;
	ShowWindow(m_WindowHandle, SW_SHOW);

	return S_OK;
}

void Application::UpdateFPS(float a_fTotalTime)
{
	// Incrementing the frame counter.
	m_dFpsFrameCounter++;
	float elapsed = a_fTotalTime - m_dFpsFrameCounter;

	// Only update the fps once per second.
	if (!m_bWindowStats || elapsed < 1.0f)
	{
		return;
	}

	// Resetting the fps counter.
	m_dFpsFrameCounter = 0;
	m_fFpsTimeElapsed += elapsed;
}

void Application::Quit(void)
{
	PostMessage(m_WindowHandle, WM_CLOSE, 0, 0);
}

LRESULT Application::ProcessWindowsMessage(
	HWND a_hWnd, 
	UINT a_uMsg, 
	WPARAM a_wParam, 
	LPARAM a_lParam)
{
	// Call ImGui’s message handler and exit early if necessary
	// if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	// {
	//  	return true;
	// }
	Application* instance = Application::GetInstance();

	// Check the incoming message and handle any we care about
	switch (a_uMsg)
	{
		// Closing / Destroying the window.
	case WM_DESTROY:
		// Sending the quit message to the overarching program.
		PostQuitMessage(0);
		return 0;

		// Prevent beeping when alt-enter-ing into fullscreen.
	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);

		// Ensuring the window is not too small.
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)a_lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)a_lParam)->ptMinTrackSize.y = 200;
		return 0;

		// Sent when the window size changes
	case WM_SIZE:
		 // Not adjusting anything when the is minimized.
		instance->m_bIsMinimized = a_wParam == SIZE_MINIMIZED;
		 if (instance->m_bIsMinimized)
		 {
		 	return 0;
		 }

		// Save the new client area dimensions.
		instance->m_dWindowWidth = LOWORD(a_lParam);
		instance->m_dWindowHeight = HIWORD(a_lParam);

		// Letting other systems know.
		 Graphics::ResizeBuffers(instance->m_dWindowWidth, instance->m_dWindowHeight);
		if (instance->m_pOnResize)
		{
			instance->m_pOnResize();
		}

		return 0;

		// Setting the delta for how much the scroll wheel has been moved.
	case WM_MOUSEWHEEL:
		Input::SetWheelDelta(GET_WHEEL_DELTA_WPARAM(a_wParam) / (float)WHEEL_DELTA);
		return 0;

		// Altering the potential focus states.
	case WM_SETFOCUS:	
		instance->m_bHasFocus = true;
		return 0;

	case WM_KILLFOCUS:	
		instance->m_bHasFocus = false;
		return 0;

	case WM_ACTIVATE:	
		instance->m_bHasFocus = (LOWORD(a_wParam) != WA_INACTIVE);
		return 0;
	}

	// Let Windows handle any messages we're not touching
	return DefWindowProc(a_hWnd, a_uMsg, a_wParam, a_lParam);
}
void Application::CreateConsoleWindow(int bufferLines, int bufferColumns, int windowLines, int windowColumns)
{
	// Only creating a single console window.
	if (m_bConsoleCreated)
	{
		return;
	}

	// Temp console info struct.
	CONSOLE_SCREEN_BUFFER_INFO coninfo;

	// Getting the console info and set the number of lines.
	AllocConsole();
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = bufferLines;
	coninfo.dwSize.X = bufferColumns;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

	SMALL_RECT rect = {};
	rect.Left = 0;
	rect.Top = 0;
	rect.Right = windowColumns;
	rect.Bottom = windowLines;
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &rect);

	FILE* stream;
	freopen_s(&stream, "CONIN$", "r", stdin);
	freopen_s(&stream, "CONOUT$", "w", stdout);
	freopen_s(&stream, "CONOUT$", "w", stderr);

	// Prevent accidental console window closing.
	HWND consoleHandle = GetConsoleWindow();
	HMENU hmenu = GetSystemMenu(consoleHandle, FALSE);
	EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED);

	// Get the current console mode and append options that allow colored output.
	DWORD currentMode = 0;
	GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &currentMode);
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE),
		currentMode | ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

	m_bConsoleCreated = true;
}

unsigned int Application::GetWidth(void) { return m_dWindowWidth; }
unsigned int Application::GetHeight(void) { return m_dWindowHeight; }
HWND& Application::GetHandle(void) { return m_WindowHandle; }
Simulation* Application::GetSimulation(void) { return m_pSimulation; }