#include "Entity.h"

Entity::Entity(std::shared_ptr<Mesh> a_pMesh, std::shared_ptr<Material> a_pMaterial)
{
	m_pMesh = a_pMesh;
	m_pMaterial = a_pMaterial;
	m_pTransform = std::make_shared<Transform>();
}

Entity::~Entity()
{
	m_pMaterial.reset();
	m_pTransform.reset();
	m_pMesh.reset();
}

Entity& Entity::operator=(const Entity& a_eOther)
{
	m_pTransform = a_eOther.m_pTransform;
	m_pMaterial = a_eOther.m_pMaterial;
	m_pMesh = a_eOther.m_pMesh;

	return *this;
}

Entity::Entity(const Entity& a_eOther)
{
	m_pTransform = a_eOther.m_pTransform;
	m_pMaterial = a_eOther.m_pMaterial;
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

std::shared_ptr<Material> Entity::GetMaterial(void)
{
	return m_pMaterial;
}

void Entity::Draw(
	std::shared_ptr<CBufferMapper<VertexCBufferData>> a_pVertexCBufferMapper,
	std::shared_ptr<CBufferMapper<MaterialCBufferData>> a_pPixelCBufferMapper,
	std::shared_ptr<Camera> a_pCamera,
	Light a_Lights[MAX_LIGHT_COUNT])
{
	// Setting constant buffer data.
	VertexCBufferData cbuffer{};
	cbuffer.World = m_pTransform->GetWorld();
	cbuffer.WorldInvTranspose = m_pTransform->GetWorldInvTra();
	cbuffer.View = a_pCamera->GetView();
	cbuffer.Projection = a_pCamera->GetProjection();

	// Sending constant buffer data to GPU.
	a_pVertexCBufferMapper->MapBufferData(cbuffer);

	// Applying materials.
	m_pMaterial->PrepMaterialForDraw(
		a_pPixelCBufferMapper,
		a_pCamera->GetTransform().GetPosition(),
		a_Lights
	);

	// Rendering the mesh.
	m_pMesh->Draw();
}
