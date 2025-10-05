#include "Material.h"

Material::Material(
	std::shared_ptr<Shader> a_pShader,
	Vector4 a_v4ColorTint,
	float a_fRoughness)
{
	m_pShader = a_pShader;
	m_v4ColorTint = a_v4ColorTint;
	m_mTextureSRVs = std::unordered_map<unsigned int, ShaderResourcePtr>();
	m_mSamplers = std::unordered_map<unsigned int, SamplerPtr>();
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
Vector4 Material::GetColor() const { return m_v4ColorTint; }
float Material::GetRoughness() const { return m_fRoughness; }
Vector2 Material::GetScale() const { return m_v2Scale; }
Vector2 Material::GetOffset() { return m_v2Offset; }
std::unordered_map<unsigned int, ShaderResourcePtr> Material::GetTextures() { return m_mTextureSRVs; }

void Material::SetScale(Vector2 a_fScale) { m_v2Scale = a_fScale; }
void Material::SetOffset(Vector2 a_fOffset) { m_v2Offset = a_fOffset; }
void Material::SetShader(std::shared_ptr<Shader> a_pShader) { m_pShader = a_pShader; }
void Material::SetColor(Vector4 a_v4ColorTint) { m_v4ColorTint = a_v4ColorTint; }

void Material::AddTexturesSRV(unsigned int a_nRegister, ShaderResourcePtr a_pSRV)
{
	m_mTextureSRVs.insert({ a_nRegister, a_pSRV });
}

void Material::AddSampler(unsigned int a_nRegister, SamplerPtr a_pSampler)
{
	m_mSamplers.insert({ a_nRegister, a_pSampler });
}

void Material::PrepMaterialForDraw(
	std::shared_ptr<CBufferMapper<MaterialCBufferData>> a_pCBufferMapper,
	Vector3 a_v3CameraPosition,
	Light a_Lights[MAX_LIGHT_COUNT])
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = Graphics::GetContext();

	// Setting the data of the cbuffer.
	MaterialCBufferData cbuffer{};
	cbuffer.CameraPosition = a_v3CameraPosition;
	for (unsigned int i = 0; i < MAX_LIGHT_COUNT; i++)
	{
		cbuffer.Lights[i] = a_Lights[i];
	}

	// Sending the data to the GPU.
	a_pCBufferMapper->MapBufferData(cbuffer);

	// Sending the resources to the GPU.
	for (const auto& t : m_mTextureSRVs) 
	{
		context->PSSetShaderResources(t.first, 1, t.second.GetAddressOf());
	}
	for (const auto& s : m_mSamplers) 
	{ 
		context->PSSetSamplers(s.first, 1, s.second.GetAddressOf()); 
	}
}
