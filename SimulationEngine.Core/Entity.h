#ifndef __ENTITY_H_
#define __ENTITY_H_

#include <memory>
#include <map>

#include "CBufferMapper.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "Camera.h"
#include "CBuffers.h"

#define DEFAULT_REGISTER 0

/// <summary>
/// Pairs a collection of vertices and indices with a material.
/// </summary>
struct SubMesh
{
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	std::shared_ptr<Material> Material;
};

/// <summary>
/// Container for Meshes, Materials and Transforms.  Defines a true object in the simulation.
/// </summary>
class Entity
{
private:
	std::map<std::shared_ptr<Mesh>, std::shared_ptr<Material>> m_mSubEntities;
	std::shared_ptr<Transform> m_pTransform = nullptr;

public:
	/// <summary>
	/// Most basic constructor for the Entity class.
	/// Defines the mesh for the entity and nothing more.
	/// </summary>
	Entity(std::shared_ptr<Mesh> a_pMesh, std::shared_ptr<Material> a_pMaterial);

	/// <summary>
	/// Contructs the Entity with the collection of mesh and materials.
	/// </summary>
	Entity(std::map<std::shared_ptr<Mesh>, std::shared_ptr<Material>> a_mSubEntities);

	Entity(
		std::shared_ptr<Shader> a_pShader,
		Microsoft::WRL::ComPtr<ID3D11SamplerState> a_pSampler,
		std::string a_sObjFile);

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
	std::vector<std::shared_ptr<Mesh>> GetMeshes(void);

	/// <summary>
	/// Gets the material for this Entity.
	/// </summary>
	std::vector<std::shared_ptr<Material>> GetMaterials(void);

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
