#ifndef __GRAPHICS_H_
#define __GRAPHICS_H_

#include <Windows.h>
#include <d3d11.h>
#include <string>
#include <wrl/client.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

/// <summary>
/// Container for DirectX api objects.
/// </summary>
class DirectObjects
{
public:
	// Main DirectX api objects.
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;

	// Rendering buffers.
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> BackBufferRTV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthBufferDSV;
};

/// <summary>
/// Initializes and provides the DirectX Api objects.
/// </summary>
namespace Graphics
{
	// Accessors.
	bool VsyncState();
	std::wstring APIName();

	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice(void);
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext(void);
	Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain(void);
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> GetBackBufferRTV(void);
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDepthBufferDSV(void);

	/// <summary>
	/// Initializes DirectX graphics rendering for the passed in windows context.
	/// </summary>
	HRESULT Initialize(
		unsigned int windowWidth, 
		unsigned int windowHeight, 
		HWND windowHandle, 
		bool vsyncIfPossible);

	/// <summary>
	/// Resizes the buffers based on updated screen bounds.
	/// </summary>
	void ResizeBuffers(unsigned int width, unsigned int height);

	void DebugLog();
}

#endif //__GRAPHICS_H_