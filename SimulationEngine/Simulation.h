#ifndef __SIMULATION_H_
#define __SIMULATION_H_

/* Safely reallocates memory.  Deletes data and initializes the pointer to nullptr. */
#define SafeDelete(p) { if (p) { delete p; p = nullptr; } }

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Sky.h"

/// <summary>
/// Defines the actual simulation creating 
/// what is rendered to the window.
/// </summary>
class Simulation
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
	std::shared_ptr<Mesh> m_pMesh = nullptr;
	std::shared_ptr<Camera> m_pCamera = nullptr;
	Shader* m_pShader = nullptr;
	Sky* m_pSky = nullptr;

public:
	/// <summary>
	/// Constructs the Simulation class.
	/// </summary>
	Simulation() = default;

	/// <summary>
	/// Frees up memory allocated by the Simulation class.
	/// </summary>
	~Simulation();

	// There is no copy operator or constructor.
	Simulation(const Simulation&) = delete;
	Simulation& operator=(const Simulation&) = delete;

	/// <summary>
	/// Initializes variables used by the Simulation.
	/// </summary>
	void Init();

	/// <summary>
	/// Per frame logic updating method for the Simulation.
	/// </summary>
	/// <param name="a_fDeltaTime">The change in time between frames.</param>
	void Update(float a_fDeltaTime);

	/// <summary>
	/// Per frame rendering method for the Simulation to its attached window context.
	/// </summary>
	/// <param name="a_fDeltaTime">The change in time between frames.</param>
	void Draw(float a_fDeltaTime);

	/// <summary>
	/// Callback for when the screen is resized by the user.
	/// </summary>
	void OnResize();
};

#endif //__SIMULATION_H_

