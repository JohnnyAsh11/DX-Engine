#ifndef __SIMULATION_H_
#define __SIMULATION_H_

/* Safely reallocates memory.  Deletes data and initializes the pointer to nullptr. */
#define SafeDelete(p) { if (p) { delete p; p = nullptr; } }

#define TRUE 1
#define FALSE 0

#include "Mesh.h"
#include "Shader.h"
#include "CBufferMapper.h"
#include "Camera.h"
#include "Sky.h"
#include "Transform.h"

/// <summary>
/// Buffer vertex containing the Color followed by an Offset.
/// </summary>
struct CBufferData
{
	Matrix4 World;
	Matrix4 WorldInvTranspose;
	Matrix4 View;
	Matrix4 Projection;
};

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
	std::shared_ptr<Sky> m_pSky = nullptr;
	std::shared_ptr<CBufferMapper<CBufferData>> m_pCBuffer = nullptr;
	std::shared_ptr<Shader> m_pShader = nullptr;

	Transform* m_pTransform = nullptr;

public:
	/// <summary>
	/// Constructs the Simulation class.
	/// </summary>
	Simulation(void) = default;

	/// <summary>
	/// Frees up memory allocated by the Simulation class.
	/// </summary>
	~Simulation(void);

	// There is no copy operator or constructor.
	Simulation(const Simulation&) = delete;
	// There is no copy operator or constructor.
	Simulation& operator=(const Simulation&) = delete;

	/// <summary>
	/// Initializes variables used by the Simulation.
	/// </summary>
	void Init(void);

	/// <summary>
	/// Per frame logic updating method for the Simulation.
	/// </summary>
	/// <param name="a_fDeltaTime">The change in time between frames.</param>
	void Update(float a_fDeltaTime);

	/// <summary>
	/// Updates ImGui every frame of the Simulation.
	/// </summary>
	/// <param name="a_fDeltaTime">The change in time between frames.</param>
	void UpdateImGui(float a_fDeltaTime);

	/// <summary>
	/// Per frame rendering method for the Simulation to its attached window context.
	/// </summary>
	/// <param name="a_fDeltaTime">The change in time between frames.</param>
	void Draw(float a_fDeltaTime);

	/// <summary>
	/// Callback for when the screen is resized by the user.
	/// </summary>
	void OnResize(void);
};

#endif //__SIMULATION_H_

