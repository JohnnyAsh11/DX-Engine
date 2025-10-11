#include "EntityManager.h"

EntityManager::EntityManager()
{
	// Creating the CBuffer mappers.
	m_pVertexCBufferMapper = std::make_shared<CBufferMapper<VertexCBufferData>>(
		DEFAULT_REGISTER,
		ShaderType::VertexShader);
	m_pPixelCBufferMapper = std::make_shared<CBufferMapper<MaterialCBufferData>>(
		DEFAULT_REGISTER,
		ShaderType::PixelShader);
}

EntityManager::~EntityManager(void)
{
}

EntityManager& EntityManager::operator=(const EntityManager& a_emOther)
{
	for (int i = 0; i < MAX_LIGHT_COUNT; i++)
	{
		m_Lights[i] = a_emOther.m_Lights[i];
	}

	m_pVertexCBufferMapper.reset();
	m_pPixelCBufferMapper.reset();

	m_pVertexCBufferMapper = a_emOther.m_pVertexCBufferMapper;
	m_pPixelCBufferMapper = a_emOther.m_pPixelCBufferMapper;
	m_lEntities = a_emOther.m_lEntities;

	return *this;
}

EntityManager::EntityManager(const EntityManager& a_emOther)
{
	for (int i = 0; i < MAX_LIGHT_COUNT; i++)
	{
		m_Lights[i] = a_emOther.m_Lights[i];
	}

	m_pVertexCBufferMapper = a_emOther.m_pVertexCBufferMapper;
	m_pPixelCBufferMapper = a_emOther.m_pPixelCBufferMapper;
	m_lEntities = a_emOther.m_lEntities;
}

void EntityManager::AddLight(Light a_LightDesc, LightIndex a_LightIndex)
{
	// Adds the light description to the passed in index.
	m_Lights[a_LightIndex] = a_LightDesc;
}

void EntityManager::AddEntity(std::shared_ptr<Mesh> a_pMesh, std::shared_ptr<Material> a_pMaterial)
{
	// Creating the entity.
	std::shared_ptr<Entity> entity = std::make_shared<Entity>(a_pMesh, a_pMaterial);

	// Adding the new entity to the vector of Entities.
	m_lEntities.push_back(entity);
}

EntityPtrCollection EntityManager::GetEntities(void)
{
	return m_lEntities;
}

Light* EntityManager::GetLights(void)
{
	return &m_Lights[0];
}

void EntityManager::Draw(std::shared_ptr<Camera> a_pCamera)
{
	for (UINT i = 0; i < m_lEntities.size(); i++)
	{
		m_lEntities[i]->Draw(
			m_pVertexCBufferMapper,
			m_pPixelCBufferMapper,
			a_pCamera,
			m_Lights);
	}
}
