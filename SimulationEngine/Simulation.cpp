#include "Simulation.h"
#include "Graphics.h"
#include <vector>

typedef DirectX::XMFLOAT3 Vector3;

void Simulation::Init()
{
	Vertex* vertices = new Vertex[3];
	unsigned int* indices = new unsigned int[3];
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	Vertex vert = Vertex();
	vert.Position = Vector3(-1.0f, -0.5f, 0.0f);
	vertices[0] = vert;
	vert.Position = Vector3(0.0f, 0.5f, 0.0f);
	vertices[1] = vert;
	vert.Position = Vector3(1.0f, -0.5f, 0.0f);
	vertices[2] = vert;

	m_pMesh = new Mesh(vertices, 3, indices, 3);

	delete[] vertices;
	delete[] indices;
}

void Simulation::Update(float a_fDeltaTime)
{
}

void Simulation::Draw(float a_fDeltaTime)
{
	float background[4] = { 0.1f, 0.8f, 0.5f, 0.0f };
	// Clearing the screen.
	Graphics::GetContext()->ClearRenderTargetView(
		Graphics::GetBackBufferRTV().Get(),
		background);
	Graphics::GetContext()->ClearDepthStencilView(
		Graphics::GetDepthBufferDSV().Get(), 
		D3D11_CLEAR_DEPTH, 1.0f, 0);

	m_pMesh->Draw();

	// Present at the end of the frame
	bool vsync = Graphics::VsyncState();
	Graphics::GetSwapChain()->Present(
		vsync ? 1 : 0,
		vsync ? 0 : DXGI_PRESENT_ALLOW_TEARING);

	// Re-bind back buffer and depth buffer after presenting
	Graphics::GetContext()->OMSetRenderTargets(
		1,
		Graphics::GetBackBufferRTV().GetAddressOf(),
		Graphics::GetDepthBufferDSV().Get());
}

void Simulation::OnResize()
{
}

Simulation::~Simulation()
{
	SafeDelete(m_pMesh);
}