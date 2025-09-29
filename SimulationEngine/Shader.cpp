#include "Shader.h"
#include "Logger.h"

#include <Windows.h>

#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

Shader::Shader(std::wstring a_sVertexShaderFile,
	std::wstring a_sPixelShaderFile)
{
	FindExecutableLocation();

	// Default shaders when using the default constructor.
	CompileShaders(a_sVertexShaderFile, a_sPixelShaderFile);
}

void Shader::SetShader(void)
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = Graphics::GetContext();

	// Setting that the buffer vertices will be rendered as triangles.
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Setting the input layout.
	context->IASetInputLayout(m_pInputLayout.Get());

	// Setting the pixel and vertex shaders.
	context->VSSetShader(m_pVertexShader.Get(), 0, 0);
	context->PSSetShader(m_pPixelShader.Get(), 0, 0);
}

void Shader::FindExecutableLocation(void)
{
	// Default to the current directory.
	std::string path = ".\\";

	// Reading the file path to the simulation's executable.
	char currentDir[1024] = {};
	GetModuleFileNameA(0, currentDir, 1024);

	// Finding the location of the last slash character.
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
	m_sExecutablePath += '\\';
}

std::wstring Shader::CreateShaderFilePath(std::wstring a_sShaderFileName)
{
	return std::wstring(m_sExecutablePath.begin(), m_sExecutablePath.end()) + a_sShaderFileName;
}

void Shader::CompileShaders(std::wstring a_sVertexShader, std::wstring a_sPixelShader)
{
	ID3DBlob* pixelShaderBlob;
	ID3DBlob* vertexShaderBlob;

	// Loading the data stored within the cso files.
	D3DReadFileToBlob(CreateShaderFilePath(a_sPixelShader).c_str(), &pixelShaderBlob);
	D3DReadFileToBlob(CreateShaderFilePath(a_sVertexShader).c_str(), &vertexShaderBlob);

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
	const UINT uSize = 4;
	D3D11_INPUT_ELEMENT_DESC inputElements[uSize] = {};

	// Every Vertex Shader will use the same input layout.
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
		uSize,										
		vertexShaderBlob->GetBufferPointer(),	
		vertexShaderBlob->GetBufferSize(),		
		m_pInputLayout.GetAddressOf());
}
