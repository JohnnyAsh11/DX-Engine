#ifndef __ENTITYMANAGER_H_
#define __ENTITYMANAGER_H_

#include <vector>

#include "Lights.h"
#include "Entity.h"
#include "CBufferMapper.h"

typedef std::vector<std::shared_ptr<Entity>> EntityPtrCollection;

/// <summary>
/// Properly manages the index for Lights in the simulation.
/// </summary>
enum LightIndex
{
	MainLight = 0,
	Light1 = 1,
	Light2 = 2,
	Light3 = 3,
	Light4 = 4
};

/// <summary>
/// Manages everything related to entities in the simulation.
/// </summary>
class EntityManager
{
private:
	Light m_Lights[MAX_LIGHT_COUNT];
	EntityPtrCollection m_lEntities;

	std::shared_ptr<CBufferMapper<VertexCBufferData>> m_pVertexCBufferMapper = nullptr;
	std::shared_ptr<CBufferMapper<MaterialCBufferData>> m_pPixelCBufferMapper = nullptr;

public:
	/// <summary>
	/// Constructs the EntityManager object.
	/// </summary>
	EntityManager();

	/// <summary>
	/// Destructs the EntityManager.
	/// </summary>
	~EntityManager(void);

	/// <summary>
	/// Copy operator implementation for the EntityManager.
	/// </summary>
	EntityManager& operator=(const EntityManager& a_emOther);

	/// <summary>
	/// Copy constructor implementation for the EntityManager.
	/// </summary>
	EntityManager(const EntityManager& a_emOther);

	/// <summary>
	/// Adds the light to the array of lights at the specified enum index.
	/// </summary>
	void AddLight(Light a_LightDesc, LightIndex a_LightIndex);
	
	/// <summary>
	/// Constructs and adds an Entity to the collection of Entities.
	/// </summary>
	void AddEntity(std::shared_ptr<Mesh> a_pMesh, std::shared_ptr<Material> a_pMaterial);

	/// <summary>
	/// Gets the collection of Entities.
	/// </summary>
	EntityPtrCollection GetEntities(void);

	/// <summary>
	/// Renders the Entities of the EntityManager.
	/// </summary>
	/// <param name="a_pCamera">Provided primarily for View and Projection matrices.</param>
	void Draw(std::shared_ptr<Camera> a_pCamera);
};

#endif //__ENTITYMANAGER_H_

