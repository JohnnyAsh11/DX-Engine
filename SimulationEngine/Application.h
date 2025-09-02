#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <vector>

/* Safely reallocates memory.  Deletes data and initializes the pointer to nullptr. */
#define SafeDelete(p) { if (p) { delete p; p = nullptr; } }

/// <summary>
/// Simplifies the construction of a window context for rendering.
/// </summary>
class Application
{
private:

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
};