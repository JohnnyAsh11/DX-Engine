#ifndef __ENTITY_H_
#define __ENTITY_H_

#include <memory>

#include "CBufferMapper.h"
#include "Mesh.h"
#include "Transform.h"

#define DEFAULT_REGISTER 0

/// <summary>
/// Constant Buffer vertex for entities in the sim world.
/// </summary>
struct CBufferData
{
	Matrix4 World;
	Matrix4 WorldInvTranspose;
	Matrix4 View;
	Matrix4 Projection;
};

/// <summary>
/// Container for Meshes, CBuffers, Materials and Transforms.  Defines a true object in the simulation.
/// </summary>
class Entity
{
private:
	std::shared_ptr<Mesh> m_pMesh = nullptr;
	std::shared_ptr<Transform> m_pTransform = nullptr;
	std::shared_ptr<CBufferMapper<CBufferData>> m_pCBuffer = nullptr;

public:
	/// <summary>
	/// Most basic constructor for the Entity class.
	/// Defines the mesh for the entity and nothing more.
	/// </summary>
	Entity(std::shared_ptr<Mesh> a_pMesh, int a_nCBufferRegister = DEFAULT_REGISTER);

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
	/// Renders the Entity to the simulation window.
	/// </summary>
	void Draw(Matrix4 a_m4View, Matrix4 a_m4Projection);
};

#endif //__ENTITY_H_
