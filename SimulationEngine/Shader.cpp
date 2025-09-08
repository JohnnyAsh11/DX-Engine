#include "Shader.h"

#include <Windows.h>
#include <d3dcompiler.h>

Shader::Shader(void)
{
	FindExecutableLocation();

	// Default shaders when using the default constructor.
	CompileShaders(L"VertexShader.cso", L"PixelShader.cso");
}

void Shader::FindExecutableLocation(void)
{
	// Default to the current directory.
	std::string path = ".\\";

	// Reading the file path to the simulation's executable.
	char currentDir[1024] = {};
	GetModuleFileNameA(0, currentDir, 1024);

	// Finding the location of the last slash charaacter.
	char* lastSlash = strrchr(currentDir, '\\');
	if (lastSlash)
	{
		// Removing the executable from the end of the filepath.
		*lastSlash = 0;

		// Setting the remainder as the file path.
		path = currentDir;
	}

	// Setting the path that has been found.
	m_sExecutablePath = path;
}

std::wstring Shader::CreateShaderPath(std::wstring a_sShaderFileName)
{
	return std::wstring(m_sExecutablePath.begin(), m_sExecutablePath.end()) + a_sShaderFileName;
}

void Shader::CompileShaders(std::wstring a_sVertexShader, std::wstring a_sPixelShader)
{
	ID3DBlob* pixelShaderBlob;
	ID3DBlob* vertexShaderBlob;

	// Loading the data stored within the cso files.
	D3DReadFileToBlob(CreateShaderPath(L"PixelShader.cso").c_str(), &pixelShaderBlob);
	D3DReadFileToBlob(CreateShaderPath(L"VertexShader.cso").c_str(), &vertexShaderBlob);

	// Creating the pixel shader with the blob.
	Graphics::GetDevice()->CreatePixelShader(
		pixelShaderBlob->GetBufferPointer(),
		pixelShaderBlob->GetBufferSize(),	
		0,									
		m_pPixelShader.GetAddressOf());		

	// Creating the vertex shader with the blob.
	Graphics::GetDevice()->CreateVertexShader(
		vertexShaderBlob->GetBufferPointer(),
		vertexShaderBlob->GetBufferSize(),	
		0,									
		m_pVertexShader.GetAddressOf());		

	// Creating the semantic input elements for the shader program.
	D3D11_INPUT_ELEMENT_DESC inputElements[4] = {};

	// POSITION
	inputElements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;			
	inputElements[0].SemanticName = "POSITION";						
	inputElements[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// NORMAL
	inputElements[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElements[1].SemanticName = "NORMAL";
	inputElements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// UV
	inputElements[2].Format = DXGI_FORMAT_R32G32_FLOAT;		
	inputElements[2].SemanticName = "UV";						
	inputElements[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// TANGENT
	inputElements[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElements[3].SemanticName = "TANGENT";
	inputElements[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	// Creating the input layout with the semantic descriptions.
	Graphics::GetDevice()->CreateInputLayout(
		inputElements,							
		2,										
		vertexShaderBlob->GetBufferPointer(),	
		vertexShaderBlob->GetBufferSize(),		
		m_pInputLayout.GetAddressOf());
}
