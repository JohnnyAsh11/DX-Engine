#include "Simulation.h"
#include "Graphics.h"
#include "Logger.h"
#include "Vectors.h"
#include "Application.h"
#include "Input.h"
#include "Transform.h"

#include <vector>

#define RED Vector4(1.0f, 0.0f, 0.0f, 1.0f);
#define GREEN Vector4(0.0f, 1.0f, 0.0f, 1.0f);
#define BLUE Vector4(0.0f, 0.0f, 1.0f, 1.0f);

void Simulation::Init()
{
	m_pTransform = new Transform();

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

	Vertex* vertices = new Vertex[3];
	unsigned int* indices = new unsigned int[3];

	// Setting the Vertices of the first triangle.
	Vertex vert = Vertex();
	vert.Position = Vector3(+0.0f, +0.5f, +0.0f);
	vert.Color = BLUE;
	vertices[0] = vert;

	vert.Position = Vector3(+0.5f, -0.5f, +0.0f);
	vert.Color = RED;
	vertices[1] = vert;

	vert.Position = Vector3(-0.5f, -0.5f, +0.0f);
	vert.Color = GREEN;
	vertices[2] = vert;

	// Setting the indices of the first triangle.
	for (int i = 0; i < 3; i++) indices[i] = i;

	//m_pMesh = std::make_shared<Mesh>(vertices, 3, indices, 3);
	m_pMesh = std::make_shared<Mesh>("../SimulationEngine.Assets/Models/cube.graphics_obj");
	m_pSky = std::make_shared<Sky>(m_pMesh, pSampler);
	m_pSky->CreateCubemap(
		L"../SimulationEngine.Assets/Textures/Skies/right.png",
		L"../SimulationEngine.Assets/Textures/Skies/left.png",
		L"../SimulationEngine.Assets/Textures/Skies/up.png",
		L"../SimulationEngine.Assets/Textures/Skies/down.png",
		L"../SimulationEngine.Assets/Textures/Skies/front.png",
		L"../SimulationEngine.Assets/Textures/Skies/back.png"
	);

	m_pShader = std::make_shared<Shader>();
	m_pShader->SetShader();

	m_pCBuffer = std::make_shared<CBufferMapper<CBufferData>>(0);

	delete[] vertices;
	delete[] indices;

	Graphics::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Simulation::Update(float a_fDeltaTime)
{
	m_pCamera->UpdateMovement(a_fDeltaTime);

	m_pTransform->Rotate(Vector3(0.0f, 0.005f, 0.0f));

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
		D3D11_CLEAR_DEPTH, 1.0f, 0.0f);

	Matrix4 m4View = m_pCamera->GetView();
	Matrix4 m4Proj = m_pCamera->GetProjection();

	// Setting the shader.
	m_pShader->SetShader();

	// Setting Constant Buffer data and sending it through the CBuffer Factory.
	CBufferData dto{};
	dto.World = m_pTransform->GetWorld();
	dto.WorldInvTranspose = m_pTransform->GetWorldInvTra();
	dto.Projection = m4Proj;
	dto.View = m4View;

	// Mapping the set CBuffer data and rendering the mesh.
	m_pCBuffer->MapBufferData(dto);
	m_pMesh->Draw();

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
	SafeDelete(m_pTransform);
	// Nothing active to destruct.
}