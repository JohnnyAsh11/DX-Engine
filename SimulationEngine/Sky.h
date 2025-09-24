#ifndef __SKY_H_
#define __SKY_H_

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "CBufferMapper.h"

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

/// <summary>
/// Defines the Constant Buffer for the sky box.
/// </summary>
struct SkyCBuffer
{
	Matrix4 View;
	Matrix4 Projection;
};

/// <summary>
/// Creates a skybox for the world.
/// </summary>
class Sky
{
private:
	std::shared_ptr<CBufferMapper<SkyCBuffer>> m_pCBuffer;
	std::shared_ptr<Mesh> m_pSkyMesh;
	std::shared_ptr<Shader> m_pShader;

	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_pSampler;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pSRV;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_pDepthStencil;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>  m_pRasterizer;

public:
	/// <summary>
	/// The default constructor for the Sky box object.
	/// </summary>
	Sky(std::shared_ptr<Mesh> a_pMesh, Microsoft::WRL::ComPtr<ID3D11SamplerState> a_pSampler);

	/// <summary>
	/// Renders the skybox to the world.
	/// </summary>
	void Draw(Matrix4 a_m4View, Matrix4 a_m4Projection);

	/// <summary>
	/// Loads six individual textures (the six faces of a cube map), then
	/// creates a blank cube map and copies each of the six textures to
	/// another face.  Afterwards, creates a shader resource view for
	/// the cube map and cleans up all of the temporary resources.
	/// </summary>
	void CreateCubemap(
		const wchar_t* right,
		const wchar_t* left,
		const wchar_t* up,
		const wchar_t* down,
		const wchar_t* front,
		const wchar_t* back);
};

#endif //__SKY_H_

