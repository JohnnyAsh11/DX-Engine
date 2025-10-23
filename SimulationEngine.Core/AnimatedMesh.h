#ifndef __ANIMATEDMESH_H_
#define __ANIMATEDMESH_H_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Vectors.h"
#include "SkinnedVertex.h"
#include "Mesh.h"
#include "SkeletonManager.h"

/// <summary>
/// Defines a Mesh that uses SkinnedVertices as opposed to the normal Vertex.
/// </summary>
class AnimatedMesh
{
private:
	BufferPtr m_pVertexBuffer;
	BufferPtr m_pIndexBuffer;
	int m_dIndexCount;
	int m_dVertexCount;

	std::shared_ptr<SkeletonManager> m_pSkeletonMgr;

public:
	/// <summary>
	/// Constructs the AnimatedMesh with preexisting vertices/indices.
	/// </summary>
	AnimatedMesh(
		SkinnedVertex* a_pVertices, 
		unsigned int a_uVertexCount, 
		unsigned int* a_pIndices, 
		unsigned int a_uIndexCount,
		TangentType a_TangentType = TangentType::Normal);

	AnimatedMesh(std::string a_sFbxFile);

	// Accessors:
	/// <summary>
	/// Retrieves the Vertex Buffer ComPtr.
	/// </summary>
	/// <returns>The Vertex Buffer ptr.</returns>
	BufferPtr GetVertexBuffer(void);

	/// <summary>
	/// Retrieves the Index Buffer ComPtr.
	/// </summary>
	/// <returns>The Index Buffer ptr.</returns>
	BufferPtr GetIndexBuffer(void);

	/// <summary>
	/// Retrieves the amount of indices in the Index Buffer.
	/// </summary>
	/// <returns>The amount of indices in the Index Buffer.</returns>
	int GetIndexCount(void);

	/// <summary>
	/// Retrieves the amount of vertices in the Vertex Buffer.
	/// </summary>
	/// <returns>The amount of vertices in the Vertex Buffer.</returns>
	int GetVertexCount(void);

	/// <summary>
	/// Renders the AnimatedMesh to the simulation window.
	/// </summary>
	void Draw(void);

private:
	/// <summary>
	/// Processes all of the data loaded from the Assimp aiScene structure.
	/// </summary>
	void ProcessAssimpScene(const aiScene* scene);

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ProcessAssimpTexture(const aiTexture* texture);
};

#endif //__ANIMATEDMesh<>_H_
