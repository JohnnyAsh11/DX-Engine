#include "Simulation.h"
#include "Graphics.h"

void Simulation::Init()
{
}

void Simulation::Update(float a_fDeltaTime)
{
}

void Simulation::Draw(float a_fDeltaTime)
{
	float background[4] = { 0.4f, 0.6f, 0.75f, 0.0f };
	// Clearing the screen.
	Graphics::GetContext()->ClearRenderTargetView(
		Graphics::GetBackBufferRTV().Get(),
		background);
	Graphics::GetContext()->ClearDepthStencilView(
		Graphics::GetDepthBufferDSV().Get(), 
		D3D11_CLEAR_DEPTH, 1.0f, 0);

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
}