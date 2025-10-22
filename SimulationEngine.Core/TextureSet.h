#ifndef __TEXTURESET_H_
#define __TEXTURESET_H_

#include <d3d11.h>
#include <wrl/client.h>

struct TextureSet
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Albedo;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Normal;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Roughness;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Metal;
};

#endif //__TEXTURESET_H_