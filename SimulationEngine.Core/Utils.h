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
}


#endif //__UTILS_H_