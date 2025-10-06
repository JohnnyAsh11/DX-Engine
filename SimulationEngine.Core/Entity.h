#ifndef __ENTITY_H_
#define __ENTITY_H_

#include <memory>

#include "CBufferMapper.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "Camera.h"

#define DEFAULT_REGISTER 0

/// <summary>
/// Constant Buffer vertex for entities in the sim world.
/// </summary>
struct VertexCBufferData
{
	Matrix4 World;
	Matrix4 WorldInvTranspose;
	Matrix4 View;
	Matrix4 Projection;
};

/// <summary>
/// Container for Meshes, Materials and Transforms.  Defines a true object in the simulation.
/// </summary>
class Entity
{
private:
	std::shared_ptr<Material> m_pMaterial = nullptr;
	std::shared_ptr<Mesh> m_pMesh = nullptr;
	std::shared_ptr<Transform> m_pTransform = nullptr;

public:
	/// <summary>
	/// Most basic constructor for the Entity class.
	/// Defines the mesh for the entity and nothing more.
	/// </summary>
	Entity(std::shared_ptr<Mesh> a_pMesh, std::shared_ptr<Material> a_pMaterial);

	/// <summary>
	/// Destructs Entity data allocated on the heap.
	/// </summary>
	~Entity();

	/// <summary>
	/// Copy operator for the Entity class. 
	/// </summary>
	Entity& operator =(const Entity& a_eOther);

	/// <summary>
	/// Copy constructor for the Entity class.
	/// </summary>
	Entity(const Entity& a_eOther);

	/// <summary>
	/// Gets a reference to the Entity's transform.
	/// </summary>
	std::shared_ptr<Transform> GetTransform(void);

	/// <summary>
	/// Gets a reference to the Entity's mesh.
	/// </summary>
	std::shared_ptr<Mesh> GetMesh(void);

	/// <summary>
	/// Gets the material for this Entity.
	/// </summary>
	std::shared_ptr<Material> GetMaterial(void);

	/// <summary>
	/// Renders the Entity to the simulation window.
	/// </summary>
	void Draw(
		std::shared_ptr<CBufferMapper<VertexCBufferData>> a_pVertexCBufferMapper,
		std::shared_ptr<CBufferMapper<MaterialCBufferData>> a_pPixelCBufferMapper,
		std::shared_ptr<Camera> a_pCamera,
		Light a_Lights[MAX_LIGHT_COUNT] = {});
};

#endif //__ENTITY_H_
