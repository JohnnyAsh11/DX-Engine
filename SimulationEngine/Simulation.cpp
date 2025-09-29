#include "Simulation.h"
#include "Graphics.h"
#include "Vectors.h"
#include "Application.h"
#include "Input.h"
#include "Transform.h"

#include <vector>

#define CUBE_FILE "../SimulationEngine.Assets/Models/cube.graphics_obj"
#define SPHERE_FILE "../SimulationEngine.Assets/Models/sphere.graphics_obj"
#define CYLINDER_FILE "../SimulationEngine.Assets/Models/cylinder.graphics_obj"

void Simulation::Init()
{
	int width = Application::GetInstance()->GetWidth();
	int height = Application::GetInstance()->GetHeight();
	float fAspectRatio = (float)width / height;
	m_pCamera = std::make_shared<Camera>(fAspectRatio, Vector3(0.0f, 0.0f, -5.0f), 45.0f);

	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	D3D11_SAMPLER_DESC sampleDesc;
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampleDesc.MaxAnisotropy = 8;
	sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;
	sampleDesc.MipLODBias = 0;
	sampleDesc.MinLOD = 0;
	sampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	Graphics::GetDevice()->CreateSamplerState(&sampleDesc, &pSampler);

	std::shared_ptr<Mesh> cube = std::make_shared<Mesh>(CUBE_FILE);
	std::shared_ptr<Mesh> sphere = std::make_shared<Mesh>(SPHERE_FILE);
	std::shared_ptr<Mesh> cylinder = std::make_shared<Mesh>(CYLINDER_FILE);
	m_pSky = std::make_shared<Sky>(cube, pSampler);
	m_pSky->CreateCubemap(
		L"../SimulationEngine.Assets/Textures/Skies/right.png",
		L"../SimulationEngine.Assets/Textures/Skies/left.png",
		L"../SimulationEngine.Assets/Textures/Skies/up.png",
		L"../SimulationEngine.Assets/Textures/Skies/down.png",
		L"../SimulationEngine.Assets/Textures/Skies/front.png",
		L"../SimulationEngine.Assets/Textures/Skies/back.png"
	);
	Entity eCube = Entity(cube);
	Entity eCylinder = Entity(cylinder);
	Entity eSphere = Entity(sphere);

	m_lEntities.push_back(eCube);
	m_lEntities.push_back(eCylinder);
	m_lEntities.push_back(eSphere);

	/*for (UINT i = 0; i < m_lEntities.size(); i++)
	{
		std::shared_ptr<Transform> t = m_lEntities[i].GetTransform();

		t->MoveRelative(Vector3(i, 0.0f, 0.0f));
		t->Scale(Vector3(0.25f, 0.25f, 0.25f));
	}*/

	m_pShader = std::make_shared<Shader>();
	m_pShader->SetShader();
	Graphics::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Simulation::Update(float a_fDeltaTime)
{
	m_pCamera->UpdateMovement(a_fDeltaTime);

	/*for (UINT i = 0; i < m_lEntities.size(); i++)
	{
		std::shared_ptr<Transform> t = m_lEntities[i].GetTransform();
		t->Rotate(Vector3(0.0f, 0.00005f, 0.0f));
	}*/

	if (Input::KeyDown(VK_ESCAPE))
	{
		Application::GetInstance()->Quit();
	}
}

void Simulation::Draw(float a_fDeltaTime)
{
	float background[4] = { 0.1f, 0.2f, 0.5f, 0.0f };
	// Clearing the screen.
	Graphics::GetContext()->ClearRenderTargetView(
		Graphics::GetBackBufferRTV().Get(),
		background);
	Graphics::GetContext()->ClearDepthStencilView(
		Graphics::GetDepthBufferDSV().Get(),
		D3D11_CLEAR_DEPTH, 1.0f, 0);

	Matrix4 m4View = m_pCamera->GetView();
	Matrix4 m4Proj = m_pCamera->GetProjection();

	// Setting the shader.
	m_pShader->SetShader();

	m_lEntities[0].Draw(m4View, m4Proj);
	//for (UINT i = 0; i < m_lEntities.size(); i++)
	//{
	//	m_lEntities[i].Draw(m4View, m4Proj);
	//}

	// Rendering the skybox last since last is slightly more efficient.
	m_pSky->Draw(m4View, m4Proj);

	// Present at the end of the frame
	bool vsync = Graphics::VsyncState();
	Graphics::GetSwapChain()->Present(
		vsync ? TRUE : FALSE,
		vsync ? FALSE : DXGI_PRESENT_ALLOW_TEARING);

	// Re-bind back buffer and depth buffer after presenting
	Graphics::GetContext()->OMSetRenderTargets(
		1,
		Graphics::GetBackBufferRTV().GetAddressOf(),
		Graphics::GetDepthBufferDSV().Get());
}

void Simulation::OnResize()
{
	if (m_pCamera != nullptr) 
	{
		int width = Application::GetInstance()->GetWidth();
		int height = Application::GetInstance()->GetHeight();
		float fAspectRatio = (float)width / height;
		m_pCamera->UpdateProjection(fAspectRatio);
	}
}

Simulation::~Simulation()
{
	// Nothing active to destruct.
}