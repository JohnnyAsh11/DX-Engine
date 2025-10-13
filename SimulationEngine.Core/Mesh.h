#ifndef __MESH_H_
#define __MESH_H_

#include <d3d11.h>
#include <memory>
#include <wrl/client.h>
#include <string>
#include <vector>

#include "Vertex.h"
#include "Shader.h"

typedef Microsoft::WRL::ComPtr<ID3D11Buffer> BufferPtr;

/// <summary>
/// Packs the data of the vertex buffer together
/// in memory for slight efficieny improvements.
/// </summary>
struct VertexPack
{
	Vertex* Vertices;
	int VertexCount;
};
/// <summary>
/// Packs the data of the index buffer together
/// in memory for slight efficieny improvements.
/// </summary>
struct IndexPack
{
	UINT* Indices;
	int IndexCount;
};

/// <summary>
/// Specifies the types of calculation changes for Tangents.
/// </summary>
enum TangentType
{
	Normal,
	Inverted
};

/// <summary>
/// Defines a collection of vertices into proper buffers for rendering.
/// </summary>
class Mesh
{
private:
	BufferPtr m_pVertexBuffer;
	BufferPtr m_pIndexBuffer;
	int m_dIndexCount;
	int m_dVertexCount;

public:

	// Construction / Rule of Three:
	/// <summary>
	/// Constructs an instance of the Mesh class.
	/// </summary>
	/// <param name="a_pVertices">The vertices inside of this instance of a Mesh object.</param>
	/// <param name="a_dVertexCount">The quantity of vertices in the array.</param>
	/// <param name="a_pIndices">The indices inside of this instance of a Mesh object.</param>
	/// <param name="a_dIndexCount">The amount of indices in the array.</param>
	Mesh(VertexPack a_VertexData, IndexPack a_IndexData, TangentType a_TangentType = TangentType::Normal);

	/// <summary>
	/// Loads in the vertices from an obj file.
	/// </summary>
	Mesh(std::string a_sObjDirectory, std::string a_sObjName);

	/// <summary>
	/// Destructs instances of the Mesh object.
	/// </summary>
	~Mesh();
	/// <summary>
	/// Copy constructor instantiates a copy of a passed in mesh object.
	/// </summary>
	/// <param name="a_pOther">The mesh object that this one is copying.</param>
	Mesh(const Mesh& a_pOther);
	/// <summary>
	/// Copy operator deep copies data from the assigned mesh object.
	/// </summary>
	/// <param name="a_pOther">The mesh that is having its data copied.</param>
	/// <returns>The new copy.</returns>
	Mesh& operator= (const Mesh& a_pOther);

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
	/// Sets the buffers and draws with the proper amount of indices.
	/// </summary>
	void Draw(void);

	/// <summary>
	/// Generates a renderable area surrounding the passed in collection of positions.
	/// </summary>
	static void CreateOutliner(std::shared_ptr<Mesh> a_pMesh, std::vector<Vector3> a_lPositions);

	/// <summary>
	/// Calculates the Tangents on a given mesh.
	/// </summary>
	static void CalculateTangents(
		Vertex* a_lVertices,
		int a_dVertexCount,
		unsigned int* a_lIndices,
		int a_dIndexCount,
		TangentType a_TangentType = TangentType::Normal);

private:

	/// <summary>
	/// Loads an obj file's vertices from file.
	/// </summary>
	void LoadObj(std::string a_sObjDirectory, std::string a_sObjName);
};

#endif //__MESH_H_
