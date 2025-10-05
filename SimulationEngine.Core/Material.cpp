#include "Material.h"

Material::Material(
	std::shared_ptr<Shader> a_pShader,
	Vector4 a_v4ColorTint,
	float a_fRoughness)
{
	m_pShader = a_pShader;
	m_v4ColorTint = a_v4ColorTint;
	m_mTextureSRVs = std::unordered_map<std::string, ShaderResourcePtr>();
	m_mSamplers = std::unordered_map<std::string, SamplerPtr>();
	m_fRoughness = a_fRoughness;
}

Material::~Material()
{
	m_pShader.reset();
}

Material::Material(const Material& a_mOther)
{
	// Setting the fields.
	m_pShader = a_mOther.m_pShader;
	m_v4ColorTint = a_mOther.m_v4ColorTint;
	m_mTextureSRVs = a_mOther.m_mTextureSRVs;
	m_mSamplers = a_mOther.m_mSamplers;
	m_fRoughness = a_mOther.m_fRoughness;
	m_v2Scale = a_mOther.m_v2Scale;
	m_v2Offset = a_mOther.m_v2Offset;
}

Material& Material::operator=(const Material& a_mOther)
{
	// Reallocating memory where necessary.
	if (m_pShader != nullptr) m_pShader.reset();

	// Setting the fields.
	m_pShader = a_mOther.m_pShader;
	m_v4ColorTint = a_mOther.m_v4ColorTint;
	m_mTextureSRVs = a_mOther.m_mTextureSRVs;
	m_mSamplers = a_mOther.m_mSamplers;
	m_v2Scale = a_mOther.m_v2Scale;
	m_v2Offset = a_mOther.m_v2Offset;

	return *this;
}

std::shared_ptr<Shader> Material::GetShader() { return m_pShader; }
Vector4 Material::GetColor() { return m_v4ColorTint; }
float Material::GetRoughness() { return m_fRoughness; }
Vector2 Material::GetScale() { return m_v2Scale; }
Vector2 Material::GetOffset() { return m_v2Offset; }
std::unordered_map<std::string, ShaderResourcePtr> Material::GetTextures() { return m_mTextureSRVs; }

void Material::SetScale(Vector2 a_fScale) { m_v2Scale = a_fScale; }
void Material::SetOffset(Vector2 a_fOffset) { m_v2Offset = a_fOffset; }
void Material::SetShader(std::shared_ptr<Shader> a_pShader) { m_pShader = a_pShader; }
void Material::SetColor(DirectX::XMFLOAT4 a_v4ColorTint) { m_v4ColorTint = a_v4ColorTint; }

void Material::AddTexturesSRV(std::string a_sTextureName, ShaderResourcePtr a_pSRV)
{
	m_mTextureSRVs.insert({ a_sTextureName, a_pSRV });
}

void Material::AddSampler(std::string a_sSamplerName, SamplerPtr a_pSampler)
{
	m_mSamplers.insert({ a_sSamplerName, a_pSampler });
}

void Material::PrepMaterialForDraw()
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = Graphics::GetContext();
	// TODO: Implement edits to this class according to changes necessary to this method.
	for (const auto& t : m_mTextureSRVs) 
	{
		m_pPixelShader->SetShaderResourceView(t.first.c_str(), t.second); 
	}
	for (const auto& s : m_mSamplers) 
	{ 
		m_pPixelShader->SetSamplerState(s.first.c_str(), s.second); 
	}
}
