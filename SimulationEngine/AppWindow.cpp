#include "Application.h"

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
		 m_bIsMinimized = a_wParam == SIZE_MINIMIZED;
		 if (m_bIsMinimized)
		 {
		 	return 0;
		 }

		// Save the new client area dimensions.
		m_dWindowWidth = LOWORD(a_lParam);
		m_dWindowHeight = HIWORD(a_lParam);

		// Letting other systems know.
		// Graphics::ResizeBuffers(windowWidth, windowHeight);
		if (m_pOnResize)
		{
			m_pOnResize();
		}

		return 0;

		// Setting the delta for how much the scroll wheel has been moved.
	case WM_MOUSEWHEEL:
		// TODO: insert this logic in some way.
		//Input::SetWheelDelta(GET_WHEEL_DELTA_WPARAM(a_wParam) / (float)WHEEL_DELTA);
		return 0;

		// Altering the potential focus states.
	case WM_SETFOCUS:	
		m_bHasFocus = true;	
		return 0;

	case WM_KILLFOCUS:	
		m_bHasFocus = false;	
		return 0;

	case WM_ACTIVATE:	
		m_bHasFocus = (LOWORD(a_wParam) != WA_INACTIVE);
		return 0;
	}

	// Let Windows handle any messages we're not touching
	return DefWindowProc(a_hWnd, a_uMsg, a_wParam, a_lParam);
}