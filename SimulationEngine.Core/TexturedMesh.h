#ifndef __TEXTUREDMESH_H_
#define __TEXTUREDMESH_H_

#include <vector>
#include <map>
#include <string>

#include "Mesh.h"
#include "Camera.h"
#include "Material.h"
#include "CBufferMapper.h"
#include "Entity.h"

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
/// Loads in an obj mesh that has an associated mtl file.
/// </summary>
class TexturedMesh
{
private:
	std::map<std::shared_ptr<Mesh>, std::shared_ptr<Material>> m_mSubMesh;

public:
	/// <summary>
	/// Loads in the passed in obj file.
	/// </summary>
	TexturedMesh(
		std::shared_ptr<Shader> a_pShader, 
		Microsoft::WRL::ComPtr<ID3D11SamplerState> a_pSampler, 
		std::string a_sObjFile);

	/// <summary>
	/// Destructs the TexturedMesh meshes.
	/// </summary>
	~TexturedMesh();

	/// <summary>
	/// Copy operator implementation for the TexturedMesh.
	/// </summary>
	TexturedMesh& operator=(const TexturedMesh& a_Other);

	/// <summary>
	/// Copy constructor implementation for the TexturedMesh.
	/// </summary>
	TexturedMesh(const TexturedMesh& a_Other);

	/// <summary>
	/// Converts all of the submesh of the TexturedMesh as a collection of Entities.
	/// </summary>
	std::vector<std::shared_ptr<Entity>> ToEntity();
};

#endif //__TEXTUREDMESH_H_
