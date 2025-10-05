#ifndef __MATERIAL_H_
#define __MATERIAL_H_

#include <memory>
#include <unordered_map>

#include "Vectors.h"
#include "Shader.h"

typedef Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResourcePtr;
typedef Microsoft::WRL::ComPtr<ID3D11SamplerState> SamplerPtr;

/// <summary>
/// Defines a material for models within the simulation.
/// </summary>
class Material
{
private:
	// The shader for this material itself.
	std::shared_ptr<Shader> m_pShader = nullptr;

	// Texture/sampler hash tables.
	std::unordered_map<std::string, ShaderResourcePtr> m_mTextureSRVs;
	std::unordered_map<std::string, SamplerPtr> m_mSamplers;

	// Material editing fields.
	Vector4 m_v4ColorTint;
	Vector2 m_v2Scale = Vector2(1.0f, 1.0f);
	Vector2 m_v2Offset = Vector2(0.0f, 0.0f);
	float m_fRoughness;

public:
	/// <summary>
	/// Constructs an instance of a Material for applying shaders to specific Meshes.
	/// </summary>
	Material(
		std::shared_ptr<Shader> a_pShader,
		Vector4 a_v4ColorTint,
		float a_fRoughness);

	/// <summary>
	/// Destructor for the Material class.
	/// </summary>
	~Material(void);

	/// <summary>
	/// Copy constructor for the Material class.
	/// </summary>
	Material(const Material& a_mOther);

	/// <summary>
	/// Copy operator for the Material class.
	/// </summary>
	Material& operator=(const Material& a_mOther);

	/// <summary>
	/// Gets the currently set shader program.
	/// </summary>
	std::shared_ptr<Shader> GetShader(void);

	/// <summary>
	/// Gets the color tint of the material.
	/// </summary>
	Vector4 GetColor(void);

	/// <summary>
	/// Gets the roughness of the material.
	/// </summary>
	float GetRoughness(void);

	/// <summary>
	/// Gets the scale of the material's textures.
	/// </summary>
	Vector2 GetScale(void);

	/// <summary>
	/// Gets the offset of the material's textures.
	/// </summary>
	Vector2 GetOffset(void);

	/// <summary>
	/// Sets the scale field of the material.
	/// </summary>
	void SetScale(Vector2 a_v2Scale);

	/// <summary>
	/// Sets the offset field of the material.
	/// </summary>
	void SetOffset(Vector2 a_v2Offset);

	/// <summary>
	/// Gets the unordered_map of textures for use outside of the material.
	/// </summary>
	std::unordered_map<std::string, ShaderResourcePtr> GetTextures();

	/// <summary>
	/// Sets the shader program pair.
	/// </summary>
	void SetShader(std::shared_ptr<Shader> a_pShader);

	/// <summary>
	/// Sets the color tint.
	/// </summary>
	void SetColor(Vector4 a_v4ColorTint);

	/// <summary>
	/// Adds a key value pair to the unordered map of texture SRVs.
	/// </summary>
	/// <param name="a_sTextureName">The name of the texture in the shaders.</param>
	/// <param name="a_pSRV">The Shader Resource View object pointer.</param>
	void AddTexturesSRV(std::string a_sTextureName, ShaderResourcePtr a_pSRV);

	/// <summary>
	/// Adds a key value pair to the unordered map of sampler states.
	/// </summary>
	/// <param name="a_sSamplerName">he name of the sampler in the shaders.</param>
	/// <param name="a_pSampler">The sampler object pointer.</param>
	void AddSampler(std::string a_sSamplerName, SamplerPtr a_pSampler);

	/// <summary>
	/// Sets all members from the unordered_maps for texture rendering.
	/// </summary>
	void PrepMaterialForDraw(void);
};

#endif //__MATERIAL_H_
