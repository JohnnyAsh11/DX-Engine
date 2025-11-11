#include "AnimEntityManager.h"

AnimEntityManager::AnimEntityManager(std::shared_ptr<Shader> a_pShader)
{
	m_pShader = a_pShader;

	// Creating the CBuffer mappers.
	m_pVertexCBuffer = std::make_shared<CBufferMapper<AnimCBufferVS>>(
		DEFAULT_REGISTER,
		ShaderType::VertexShader);
	m_pPixelCBuffer = std::make_shared<CBufferMapper<MaterialCBufferData>>(
		DEFAULT_REGISTER,
		ShaderType::PixelShader);
}

AnimEntityManager::~AnimEntityManager(void) { }
AnimEntityManager::AnimEntityManager(const AnimEntityManager& a_other)
{
	m_pVertexCBuffer = a_other.m_pVertexCBuffer;
	m_pPixelCBuffer = a_other.m_pPixelCBuffer;
	m_lEntities = a_other.m_lEntities;

	for (unsigned int i = 0; i < MAX_LIGHT_COUNT; i++)
	{
		m_Lights[i] = a_other.m_Lights[i];
	}
}
AnimEntityManager& AnimEntityManager::operator=(const AnimEntityManager& a_other)
{
	m_pVertexCBuffer = a_other.m_pVertexCBuffer;
	m_pPixelCBuffer = a_other.m_pPixelCBuffer;
	m_lEntities = a_other.m_lEntities;

	for (unsigned int i = 0; i < MAX_LIGHT_COUNT; i++)
	{
		m_Lights[i] = a_other.m_Lights[i];
	}

	return *this;
}

void AnimEntityManager::AddLight(Light a_LightDesc, LightIndex a_LightIndex)
{
	// Adds the light description to the passed in index.
	m_Lights[a_LightIndex] = a_LightDesc;
}

void AnimEntityManager::AddAnimEntity(std::shared_ptr<AnimatedEntity> a_pAnimEntity)
{
	m_lEntities.push_back(a_pAnimEntity);
}


void AnimEntityManager::Draw(std::shared_ptr<Camera> a_pCamera)
{
	m_pShader->SetShader();
	for (unsigned int i = 0; i < m_lEntities.size(); i++)
	{
		m_lEntities[i]->Draw(
			m_pVertexCBuffer, 
			m_pPixelCBuffer, 
			a_pCamera, 
			m_Lights);
	}
}

std::vector<std::shared_ptr<AnimatedEntity>> AnimEntityManager::GetEntities(void) { return m_lEntities; }
