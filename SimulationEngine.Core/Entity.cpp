#include "Entity.h"

Entity::Entity(std::shared_ptr<Mesh> a_pMesh)
{
	m_pMesh = a_pMesh;
	m_pTransform = std::make_shared<Transform>();
}

Entity::~Entity()
{
	m_pTransform.reset();
	m_pMesh.reset();
}

Entity& Entity::operator=(const Entity& a_eOther)
{
	m_pTransform = a_eOther.m_pTransform;
	m_pMesh = a_eOther.m_pMesh;

	return *this;
}

Entity::Entity(const Entity& a_eOther)
{
	m_pTransform = a_eOther.m_pTransform;
	m_pMesh = a_eOther.m_pMesh;
}

std::shared_ptr<Transform> Entity::GetTransform(void)
{
	return m_pTransform;
}

std::shared_ptr<Mesh> Entity::GetMesh(void)
{
	return m_pMesh;
}

void Entity::Draw(
	std::shared_ptr<CBufferMapper<CBufferData>> a_pCBufferMapper, 
	Matrix4 a_m4View, 
	Matrix4 a_m4Projection)
{
	CBufferData cbuffer{};
	cbuffer.World = m_pTransform->GetWorld();
	cbuffer.WorldInvTranspose = m_pTransform->GetWorldInvTra();
	cbuffer.View = a_m4View;
	cbuffer.Projection = a_m4Projection;

	a_pCBufferMapper->MapBufferData(cbuffer);

	m_pMesh->Draw();
}
