#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <vector>
#include <sstream>

/* Safely reallocates memory.  Deletes data and initializes the pointer to nullptr. */
#define SafeDelete(p) { if (p) { delete p; p = nullptr; } }

/// <summary>
/// Simplifies the construction of a window context for rendering.
/// </summary>
class Application
{
private:
    // Initialization progress tracking.
    bool m_bWindowCreated = false;
    bool m_bConsoleCreated = false;

    // Window information.
    std::wstring m_sWindowTitle;
    unsigned int m_dWindowWidth = 0;
    unsigned int m_dWindowHeight = 0;
    bool m_bWindowStats = false;
    HWND m_WindowHandle = 0;
    bool m_bHasFocus = false;
    bool m_bIsMinimized = false;

    // Function pointer to call on window resizing.
    void (*m_pOnResize)() = 0;

    // FPS Tracking.
    float m_fFpsTimeElapsed = 0.0f;
    __int64 m_dFpsFrameCounter = 0;

public:
    /// <summary>
    /// Constructs the 
    /// </summary>
    Application(void);

    /// <summary>
    /// Clears memory from the application's usage.
    /// </summary>
    ~Application(void);

    /// <summary>
    /// Standard copy operator for the Application class
    /// </summary>
    Application& operator =(const Application& a_pApp);

    /// <summary>
    /// Copy constructor for the Aplication.
    /// </summary>
    Application(const Application& a_pApp);

    /// <summary>
    /// Runs the main simulation loop.
    /// </summary>
    void Run(void);

    /// <summary>
    /// Initializes variables used by the Application.  Called by the constructor.
    /// </summary>
    void Init(void);

    /// <summary>
    /// Frees memory up used by the Application.  Called by the destructor.
    /// </summary>
    void Release(void);

    /// <summary>
    /// Updates the logic of objects in the world every frame.
    /// </summary>
    /// <param name="a_fDeltaTime">The change in time between frames.</param>
    void Update(float a_fDeltaTime);

    /// <summary>
    /// Renders the current frame to the window context.
    /// </summary>
    /// <param name="a_fDeltaTime">The change in time between windows.</param>
    void Render(float a_fDeltaTime);

private:
    /// <summary>
    /// Handles Windows operating system messages.
    /// </summary>
    LRESULT ProcessWindowsMessage(HWND a_hWnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam);

    /// <summary>
    /// Sends a message to stop the program and close the window.
    /// </summary>
    void Quit(void);

    /// <summary>
    /// Updates the framerate tracking for the application window.
    /// </summary>
    /// <param name="a_fTotalTime">The total time that has elapsed for this application.</param>
    void UpdateFPS(float a_fTotalTime);

    HRESULT CreateWindowContext(HINSTANCE a_AppInstance, unsigned int a_uWidth, unsigned int a_uHeight, std::wstring a_sTitleBar, void (*a_pOnResize)());
};