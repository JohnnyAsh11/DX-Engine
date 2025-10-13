#ifndef __UTILS_H_
#define __UTILS_H_

#include <string>

#include "TextureSet.h"

namespace Utils
{
	/// <summary>
	/// Loads in the texture files associated with the passed in wide char name.
	/// Files loaded files must be in the SimulationEngine.Assets/Textures/PBR/ directory.
	/// Files also must have an _albedo, _normal, _roughness, and _metal png files.
	/// </summary>
	TextureSet LoadTextureSet(std::wstring a_sTextureName);

	/// <summary>
	/// Loads an individual resource (SRV) into the program.
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> LoadTexture(std::wstring a_sFileName);

	/// <summary>
	/// Cleans up a file prior to use in other texture loading functions.
	/// </summary>
	std::wstring SanitizeFileName(std::string a_sStringToChange);
}


#endif //__UTILS_H_