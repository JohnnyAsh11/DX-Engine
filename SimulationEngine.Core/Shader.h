#ifndef __SHADER_H_
#define __SHADER_H_

#include<string>

#include "Graphics.h"

/// <summary>
/// Less verbose versions of the D3D11 topology values.
/// </summary>
enum ShaderTopology
{
	TriangleList = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	LineList = D3D11_PRIMITIVE_TOPOLOGY_LINELIST
};

/// <summary>
/// Compiles and deploys shaders used by the simulation.
/// </summary>
class Shader
{
private:
	ShaderTopology m_ShaderTopology;
	std::string m_sExecutablePath;	
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader = nullptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader = nullptr;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer = nullptr;
		
public:
	/// <summary>
	/// Constructs a shader program with the passed in shader files.
	/// </summary>
	/// <param name="a_sVertexShaderFile">The vertex shader .cso file.</param>
	/// <param name="a_sPixelShaderFile">The pixel shader .cso file.</param>
	Shader(std::wstring a_sVertexShaderFile = L"VertexShader.cso", 
		   std::wstring a_sPixelShaderFile = L"PixelShader.cso",
		   ShaderTopology a_ShaderTopology = ShaderTopology::TriangleList);

	/// <summary>
	/// Sets this shader program as the active shader.
	/// </summary>
	void SetShader(void);

private:
	/// <summary>
	/// Finds the filepath containing the executable.  This is where HLSL/.cso files are put.
	/// </summary>
	void FindExecutableLocation(void);

	/// <summary>
	/// Sets the file path of the shader with the executable file location.
	/// </summary>
	/// <param name="a_sShaderFileName">The file name of a shader file.</param>
	/// <returns>The full filepath of the passed in shader file.</returns>
	std::wstring CreateShaderFilePath(std::wstring a_sShaderFileName);

	/// <summary>
	/// Attempts to find the passed in shader (.cso) files and creates them with DirectX.
	/// </summary>
	/// <param name="a_sVertexShader">The vertex shader file name.</param>
	/// <param name="a_sPixelShader">The pixel shader file name.</param>
	void CompileShaders(std::wstring a_sVertexShader, std::wstring a_sPixelShader);
};

#endif //__SHADER_H_

