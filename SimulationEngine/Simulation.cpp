#include "Simulation.h"
#include "Graphics.h"
#include "Logger.h"
#include "Vectors.h"

#include <vector>

#define RED Vector4(1.0f, 0.0f, 0.0f, 1.0f);
#define GREEN Vector4(0.0f, 1.0f, 0.0f, 1.0f);
#define BLUE Vector4(0.0f, 0.0f, 1.0f, 1.0f);

void Simulation::Init()
{
	Vertex* vertices = new Vertex[3];
	unsigned int* indices = new unsigned int[3];

	// Setting the Vertices of the first triangle.
	Vertex vert = Vertex();
	vert.Position = Vector3(+0.0f, +0.5f, +0.0f);
	vert.Color = RED;
	vertices[0] = vert;

	vert.Position = Vector3(+0.5f, -0.5f, +0.0f);
	vert.Color = GREEN;
	vertices[1] = vert;

	vert.Position = Vector3(-0.5f, -0.5f, +0.0f);
	vert.Color = BLUE;
	vertices[2] = vert;

	// Setting the indices of the first triangle.
	for (int i = 0; i < 3; i++) indices[i] = i;

	m_pMesh = new Mesh(vertices, 3, indices, 3);
	//m_pMesh = new Mesh("../SimulationEngine.Models/cube.graphics_obj");

	m_pShader = new Shader();
	m_pShader->SetShader();

	delete[] vertices;
	delete[] indices;
}

void Simulation::Update(float a_fDeltaTime)
{
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
	SafeDelete(m_pShader);
	SafeDelete(m_pMesh);
}