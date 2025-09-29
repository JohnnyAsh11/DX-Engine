#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include <sstream>
#include <d3d11.h>

#include "Simulation.h"

/// <summary>
/// Simplifies the construction of a window context for rendering.
/// </summary>
class Application
{
private:
    static Application* m_pInstance;
    Simulation* m_pSimulation = new Simulation();

    // Initialization progress tracking.
    bool m_bWindowCreated = false;
    bool m_bConsoleCreated = false;

    // Window information.
    bool m_bIsMinimized;
    unsigned int m_dWindowWidth;
    unsigned int m_dWindowHeight;
    bool m_bHasFocus;
    std::wstring m_sWindowTitle;
    bool m_bWindowStats = false;
    HWND m_WindowHandle = 0;

    // Function pointer to call on window resizing.
    void (*m_pOnResize)();

    // FPS Tracking.
    float m_fFpsTimeElapsed = 0.0f;
    __int64 m_dFpsFrameCounter = 0;

public:
    /// <summary>
    /// Gets the single instance of the Application.
    /// </summary>
    static Application* GetInstance();
    
    /// <summary>
    /// Runs the main simulation loop.
    /// </summary>
    HRESULT Run(void);

    /// <summary>
    /// Frees memory up used by the Application.  Called by the destructor.
    /// </summary>
    void Release(void);

    /// <summary>
    /// Actually creates the window context.
    /// </summary>
    HRESULT CreateWindowContext(
        HINSTANCE a_AppInstance,
        unsigned int a_uWidth,
        unsigned int a_uHeight,
        std::wstring a_sTitleBar,
        void (*a_pOnResize)());

    /// <summary>
    /// Creates a console window for debug output.
    /// </summary>
    void CreateConsoleWindow(
        int bufferLines, 
        int bufferColumns, 
        int windowLines, 
        int windowColumns);

    /// <summary>
    /// Sends a message to stop the program and close the window.
    /// </summary>
    void Quit(void);

    /// <summary>
    /// Updates the framerate tracking for the application window.
    /// </summary>
    /// <param name="a_fTotalTime">The total time that has elapsed for this application.</param>
    void UpdateFPS(float a_fTotalTime);

    /// <summary>
    /// Gets the Width of the window.
    /// </summary>
    unsigned int GetWidth(void);

    /// <summary>
    /// Gets the Height of the window.
    /// </summary>
    unsigned int GetHeight(void);

    /// <summary>
    /// Gets the Windows Handle object.
    /// </summary>
    HWND& GetHandle(void);

    /// <summary>
    /// Gets a pointer to this Application's Simulation.
    /// </summary>
    Simulation* GetSimulation(void);

private:
    /// <summary>
    /// Constructs the 
    /// </summary>
    Application(void);

    /// <summary>
    /// Clears memory from the application's usage.
    /// </summary>
    ~Application(void);

    // The application should not have a copy operator or constructor.
    Application& operator =(const Application& a_pApp) = delete;
    Application(const Application& a_pApp) = delete;

    /// <summary>
    /// Handles Windows operating system messages.
    /// </summary>
    static LRESULT ProcessWindowsMessage(HWND a_hWnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam);
};

#endif //__APPLICATION_H_