#ifndef __ANIMATEDMESH_H_
#define __ANIMATEDMESH_H_

#include "Vectors.h"
#include "SkinnedVertex.h"
#include "Mesh.h"

/// <summary>
/// Defines a Mesh that uses SkinnedVertices as opposed to the normal Vertex.
/// Essentially a hard copy of the Mesh object except using the SkinnedVertex.
/// </summary>
class AnimatedMesh
{
private:
	BufferPtr m_pVertexBuffer;
	BufferPtr m_pIndexBuffer;
	int m_dIndexCount;
	int m_dVertexCount;

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
};

#endif //__ANIMATEDMesh<>_H_
