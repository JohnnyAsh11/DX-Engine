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
