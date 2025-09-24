#include "Sky.h"
#include "Sky.h"
#include "Graphics.h"

#include <WICTextureLoader.h>

using namespace DirectX;
#define SKY_CBUFFER_REGISTER 1

Sky::Sky(std::shared_ptr<Mesh> a_pMesh, Microsoft::WRL::ComPtr<ID3D11SamplerState> a_pSampler)
{
	m_pSkyMesh = a_pMesh;
	m_pSampler = a_pSampler;

	// Creating the rasterizer.
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_FRONT;
	Graphics::GetDevice()->CreateRasterizerState(&rasterizerDesc, &m_pRasterizer);

	// Creating the depth stencil.
	D3D11_DEPTH_STENCIL_DESC depthStencilState = {};
	depthStencilState.DepthEnable = true;
	depthStencilState.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilState.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	Graphics::GetDevice()->CreateDepthStencilState(&depthStencilState, &m_pDepthStencil);

	// Initializing the special Shader program and the CBuffer.
	m_pShader = std::make_shared<Shader>(L"SkyVertex.cso", L"SkyPixel.cso");
	m_pCBuffer = std::make_shared<CBufferMapper<SkyCBuffer>>(SKY_CBUFFER_REGISTER);
}

void Sky::Draw(Matrix4 a_m4View, Matrix4 a_m4Projection)
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = Graphics::GetContext();

	// Setting the proper rasterizer and depth stencil.
	context->RSSetState(m_pRasterizer.Get());
	context->OMSetDepthStencilState(m_pDepthStencil.Get(), 0);

	// Setting the active shaders.
	m_pShader->SetShader();

	// Sending data to GPU with the constant buffer.
	SkyCBuffer dto{};
	dto.View = a_m4View;
	dto.Projection = a_m4Projection;
	m_pCBuffer->MapBufferData(dto);

	// Setting data for use within shaders.
	context->PSSetShaderResources(0, 1, m_pSRV.GetAddressOf());
	context->PSSetSamplers(0, 1, m_pSampler.GetAddressOf());

	// Rendering the skybox.
	m_pSkyMesh->Draw();

	// Reseting render states.
	context->RSSetState(nullptr);
	context->OMSetDepthStencilState(nullptr, 0);
}

void Sky::CreateCubemap(
	const wchar_t* right,
	const wchar_t* left,
	const wchar_t* up,
	const wchar_t* down,
	const wchar_t* front,
	const wchar_t* back)
{
	const int CUBE_SIZE = 6;
	Microsoft::WRL::ComPtr<ID3D11Device> device = Graphics::GetDevice();

	// Loading all 6 textures into an array together.
	Microsoft::WRL::ComPtr<ID3D11Texture2D> textures[CUBE_SIZE] = {};

	CreateWICTextureFromFile(device.Get(), right, (ID3D11Resource**)textures[0].GetAddressOf(), 0);
	CreateWICTextureFromFile(device.Get(), left, (ID3D11Resource**)textures[1].GetAddressOf(), 0);
	CreateWICTextureFromFile(device.Get(), up, (ID3D11Resource**)textures[2].GetAddressOf(), 0);
	CreateWICTextureFromFile(device.Get(), down, (ID3D11Resource**)textures[3].GetAddressOf(), 0);
	CreateWICTextureFromFile(device.Get(), front, (ID3D11Resource**)textures[4].GetAddressOf(), 0);
	CreateWICTextureFromFile(device.Get(), back, (ID3D11Resource**)textures[5].GetAddressOf(), 0);

	// Using the description of the first texture for all textures.
	D3D11_TEXTURE2D_DESC faceDesc = {};
	textures[0]->GetDesc(&faceDesc);

	// Creating the cube map description.
	D3D11_TEXTURE2D_DESC cubeDesc = {};
	cubeDesc.ArraySize = 6;
	cubeDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	cubeDesc.CPUAccessFlags = 0;       
	cubeDesc.Format = faceDesc.Format; 
	cubeDesc.Width = faceDesc.Width;   
	cubeDesc.Height = faceDesc.Height; 
	cubeDesc.MipLevels = 1;            
	cubeDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE; 
	cubeDesc.Usage = D3D11_USAGE_DEFAULT;
	cubeDesc.SampleDesc.Count = 1;
	cubeDesc.SampleDesc.Quality = 0;

	// Creating the cube map texture more directly.
	Microsoft::WRL::ComPtr<ID3D11Texture2D> cubeMap;
	device->CreateTexture2D(&cubeDesc, 0, cubeMap.GetAddressOf());

	// Looping through each texture and copying them to the cube map texture.
	for (int i = 0; i < CUBE_SIZE; i++)
	{
		// Calculating the subresource position to copy into.
		unsigned int subresource = D3D11CalcSubresource(
			0, 
			i, 
			1);

		// Copying from one resource to another.
		Graphics::GetContext()->CopySubresourceRegion(
			cubeMap.Get(),
			subresource,
			0, 0, 0,
			textures[i].Get(), 
			0, 
			0);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = cubeDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MipLevels = 1;
	srvDesc.TextureCube.MostDetailedMip = 0;

	// Making the SRV.
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cubeSRV;
	device->CreateShaderResourceView(cubeMap.Get(), &srvDesc, cubeSRV.GetAddressOf());
	m_pSRV = cubeSRV;
}