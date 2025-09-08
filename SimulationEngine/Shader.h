#ifndef __SHADER_H_
#define __SHADER_H_

#include<string>

#include "Graphics.h"

/// <summary>
/// Compiles and deploys shaders used by the simulation.
/// </summary>
class Shader
{
private:
	std::string m_sExecutablePath;
	
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer;
		
public:
	/// <summary>
	/// Constructs a pair of shaders (vertex/pixel) for use in the simulation.
	/// </summary>
	Shader(void);

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
	std::wstring CreateShaderPath(std::wstring a_sShaderFileName);

	/// <summary>
	/// Attempts to find the passed in shader (.cso) files and creates them with DirectX.
	/// </summary>
	/// <param name="a_sVertexShader">The vertex shader file name.</param>
	/// <param name="a_sPixelShader">The pixel shader file name.</param>
	void CompileShaders(std::wstring a_sVertexShader, std::wstring a_sPixelShader);

};

#endif //__SHADER_H_

