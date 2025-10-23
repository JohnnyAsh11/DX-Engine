#include "Utils.h"
#include "Graphics.h"

#include <WICTextureLoader.h>

#define PBR_FILE_PATH L"../SimulationEngine.Assets/Textures/PBR/"

#define ALBEDO_ENDING L"_albedo.png"
#define NORMALS_ENDING L"_normals.png"
#define ROUGHNESS_ENDING L"_roughness.png"
#define METAL_ENDING L"_metal.png"

TextureSet Utils::LoadTextureSet(std::wstring a_sTextureName)
{
	Microsoft::WRL::ComPtr<ID3D11Device> device = Graphics::GetDevice().Get();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = Graphics::GetContext().Get();

	TextureSet result{};

	std::wstring sAlbedo = (PBR_FILE_PATH + a_sTextureName + ALBEDO_ENDING).c_str();
	std::wstring sNormal = (PBR_FILE_PATH + a_sTextureName + NORMALS_ENDING).c_str();
	std::wstring sRoughness = (PBR_FILE_PATH + a_sTextureName + ROUGHNESS_ENDING).c_str();
	std::wstring sMetal = (PBR_FILE_PATH + a_sTextureName + METAL_ENDING).c_str();

	DirectX::CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		sAlbedo.c_str(),
		nullptr,
		&result.Albedo);
	DirectX::CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		sNormal.c_str(),
		nullptr,
		&result.Normal);
	DirectX::CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		sRoughness.c_str(),
		nullptr,
		&result.Roughness);
	DirectX::CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		sMetal.c_str(),
		nullptr,
		&result.Metal);

	return result;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Utils::LoadTexture(std::wstring a_sFileName)
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> result;
	Microsoft::WRL::ComPtr<ID3D11Device> device = Graphics::GetDevice().Get();
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = Graphics::GetContext().Get();
	
	DirectX::CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		a_sFileName.c_str(),
		nullptr,
		&result);

	return result;
}

std::wstring Utils::SanitizeFileName(std::string a_sStringToChange)
{
	// Saving the inputed string as a wstring.
	std::wstring sTexturePath = std::wstring(
		a_sStringToChange.begin(),
		a_sStringToChange.end()
	);

	// Finding whether or not this contains the correct file ending.
	size_t pos = sTexturePath.rfind(L".tga");
	if (pos != std::wstring::npos)
	{
		// Replacing .tga with .png.
		sTexturePath.replace(pos, 4, L".png");
	}

	// Returning that file.
	return sTexturePath;
}

Matrix4 Utils::ConvertFromAssimpMatrix(const aiMatrix4x4& from)
{
	Matrix4 to{};

	// Transpose the matrix to convert from Assimp's convention to DirectX11's.
	to._11 = from.a1; to._12 = from.b1; to._13 = from.c1; to._14 = from.d1;
	to._21 = from.a2; to._22 = from.b2; to._23 = from.c2; to._24 = from.d2;
	to._31 = from.a3; to._32 = from.b3; to._33 = from.c3; to._34 = from.d3;
	to._41 = from.a4; to._42 = from.b4; to._43 = from.c4; to._44 = from.d4;

	return to;
}
