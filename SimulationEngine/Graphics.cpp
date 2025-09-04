#include "Graphics.h"
#include <Windows.h>
#include <dxgi1_6.h>

// Use high-performance GPU in multi-GPU systems.
extern "C"
{
    // NVIDIA
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001; 

    // AMD
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

namespace Graphics
{
    // Variables local to this file.
	namespace
	{
		bool bApiInitialized = false;
		bool bSupportsTearing = false;
		bool bVsyncDesired = false;
		BOOL bIsFullscreen = false;

		D3D_FEATURE_LEVEL featureLevel;

		Microsoft::WRL::ComPtr<ID3D11InfoQueue> pInfoQueue;

		DirectObjects api = DirectObjects();
	}
}

HRESULT Graphics::Initialize(
    unsigned int a_dWindowWidth, 
    unsigned int a_dWindowHeight,
    HWND a_WindowHandle, 
    bool a_bVsyncIfPossible)
{
	if (bApiInitialized)
	{
		return E_FAIL;
	}

	// Attempting to set the vsync, only functions if supported by hardware.
	bVsyncDesired = a_bVsyncIfPossible;

	// Actually determining if vsync is possible to have enabled.
	Microsoft::WRL::ComPtr<IDXGIFactory5> factory;
	if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
	{
		// Check for this specific feature (must use BOOL typedef here!)
		BOOL tearingSupported = false;
		HRESULT featureCheck = factory->CheckFeatureSupport(
			DXGI_FEATURE_PRESENT_ALLOW_TEARING,
			&tearingSupported,
			sizeof(tearingSupported));

		// Final determination of support
		bSupportsTearing = SUCCEEDED(featureCheck) && tearingSupported;
	}

	// Setting a debug flag for error console output.
	unsigned int deviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Swap chain description.
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = a_dWindowWidth;
	swapDesc.BufferDesc.Height = a_dWindowHeight;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.Flags = bSupportsTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;
	swapDesc.OutputWindow = a_WindowHandle;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Windowed = true;

	// Attempt to initialize DirectX
	HRESULT hr = S_OK;
	hr = D3D11CreateDeviceAndSwapChain(
		0,							
		D3D_DRIVER_TYPE_HARDWARE,	
		0,							
		deviceFlags,				
		0,							
		0,							
		D3D11_SDK_VERSION,			
		&swapDesc,					
		api.SwapChain.GetAddressOf(),	
		api.Device.GetAddressOf(),		
		&featureLevel,				
		api.Context.GetAddressOf()
	);

	// Ensuring that the initialization was successful.
	if (FAILED(hr))
	{
		return hr;
	}

	bApiInitialized = true;

	// Calling the resize method since it happens to
	// initialize the Depth Stencil and Render Target View
	// as needed by the application.
	ResizeBuffers(a_dWindowWidth, a_dWindowHeight);

#if defined(DEBUG) || defined(_DEBUG)
	// If in debug mode, set up the info queue to get debug messages.
	Microsoft::WRL::ComPtr<ID3D11Debug> debug;
	api.Device->QueryInterface(IID_PPV_ARGS(debug.GetAddressOf()));
	debug->QueryInterface(IID_PPV_ARGS(pInfoQueue.GetAddressOf()));
#endif

	return S_OK;
}

void Graphics::ResizeBuffers(unsigned int width, unsigned int height)
{
	// Ensuring the graphics API is initialized
	if (!bApiInitialized)
	{
		return;
	}

	// Clearing out the buffer pointers.
	api.BackBufferRTV.Reset();
	api.DepthBufferDSV.Reset();

	// Resize the swap chain buffers.
	api.SwapChain->ResizeBuffers(
		2,
		width,
		height,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		bSupportsTearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0);

	// Grabbing the references to the first buffer.
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBufferTexture;
	api.SwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),
		(void**)backBufferTexture.GetAddressOf());

	// Create a render target view for the back buffer for rendering.
	api.Device->CreateRenderTargetView(
		backBufferTexture.Get(),
		0,
		api.BackBufferRTV.GetAddressOf());

	// Depth Stencil description struct.
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	// Create the depth buffer and its view.
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthBufferTexture;
	api.Device->CreateTexture2D(&depthStencilDesc, 0, &depthBufferTexture);
	api.Device->CreateDepthStencilView(
		depthBufferTexture.Get(),
		0,
		api.DepthBufferDSV.GetAddressOf());

	// Bind the views to the pipeline, so rendering properly uses their underlying textures.
	api.Context->OMSetRenderTargets(
		1,
		api.BackBufferRTV.GetAddressOf(),
		api.DepthBufferDSV.Get());

	// Set up the viewport so we render into to correct portion of the window.
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	api.Context->RSSetViewports(1, &viewport);

	// Setting in case fullscreen is active.
	api.SwapChain->GetFullscreenState(&bIsFullscreen, 0);
}

void Graphics::DebugLog()
{
	// Ensuring that there is an active info queue.
	if (!pInfoQueue)
	{
		return;
	}

	// Ensuring there are active messages to print.
	UINT64 messageCount = pInfoQueue->GetNumStoredMessages();
	if (messageCount == 0)
	{
		return;
	}

	// Printing out all messages.
	for (UINT64 i = 0; i < messageCount; i++)
	{
		// Getting the size to allocate necessary space.
		size_t messageSize = 0;
		pInfoQueue->GetMessage(i, 0, &messageSize);

		// Reserving space for this message.
		D3D11_MESSAGE* message = (D3D11_MESSAGE*)malloc(messageSize);
		pInfoQueue->GetMessage(i, message, &messageSize);

		// Printing and clearing up memory.
		if (message)
		{
			// Color code based on severity
			switch (message->Severity)
			{
				// Red colored.
				case D3D11_MESSAGE_SEVERITY_CORRUPTION:
				case D3D11_MESSAGE_SEVERITY_ERROR:
					printf("\x1B[91m"); break;

				// Yellow colored.
				case D3D11_MESSAGE_SEVERITY_WARNING:
					printf("\x1B[93m"); break;

				// Cyan colored.
				case D3D11_MESSAGE_SEVERITY_INFO:
				case D3D11_MESSAGE_SEVERITY_MESSAGE:
					printf("\x1B[96m"); break;
			}

			printf("%s\n\n", message->pDescription);
			free(message);

			// Reset color
			printf("\x1B[0m");
		}
	}

	// Clear any messages that were printed.
	pInfoQueue->ClearStoredMessages();
}

Microsoft::WRL::ComPtr<ID3D11Device> Graphics::GetDevice(void)
{
	return api.Device;
}
Microsoft::WRL::ComPtr<ID3D11DeviceContext> Graphics::GetContext(void)
{
	return api.Context;
}
Microsoft::WRL::ComPtr<IDXGISwapChain> Graphics::GetSwapChain(void)
{
	return api.SwapChain;
}
Microsoft::WRL::ComPtr<ID3D11RenderTargetView> Graphics::GetBackBufferRTV(void)
{
	return api.BackBufferRTV;
}
Microsoft::WRL::ComPtr<ID3D11DepthStencilView> Graphics::GetDepthBufferDSV(void)
{
	return api.DepthBufferDSV;
}
bool Graphics::VsyncState()
{
	return bVsyncDesired || !bSupportsTearing || bIsFullscreen;
}
std::wstring Graphics::APIName()
{
	switch (featureLevel)
	{
	case D3D_FEATURE_LEVEL_10_0: return L"D3D10";
	case D3D_FEATURE_LEVEL_10_1: return L"D3D10.1";
	case D3D_FEATURE_LEVEL_11_0: return L"D3D11";
	case D3D_FEATURE_LEVEL_11_1: return L"D3D11.1";
	default: return L"Unknown";
	}
}