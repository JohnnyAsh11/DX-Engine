#ifndef __OUTLINER_H_
#define __OUTLINER_H_

#include <vector>
#include <memory>

#include "Shader.h"
#include "Vectors.h"
#include "Camera.h"
#include "CBufferMapper.h"

/// <summary>
/// Defines a single line vertex.  Only a Position now but could become more.
/// </summary>
struct LineVertex
{
	Vector3 Position;
};
struct LineCBufferData
{
	Matrix4 World;
	Matrix4 View;
	Matrix4 Projection;
	Vector4 Color;
};

/// <summary>
/// Defines a space for debug purposes using lines and special line shaders.
/// Functionally a Mesh but for collections of lines.
/// </summary>
class Outliner
{
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer = nullptr;

	std::shared_ptr<Transform> m_pTransform = nullptr;

	std::vector<LineVertex> m_lVertices;
	std::vector<unsigned int> m_lIndices;

	unsigned int m_uVertexCount = 0;
	unsigned int m_uIndexCount = 0;
	bool m_bIsCompiled = false;

public:
	/// <summary>
	/// Constructs the empty Outliner.
	/// </summary>
	Outliner(void);

	/// <summary>
	/// Constructs the Outliner with an existing vector of vertices.
	/// </summary>
	Outliner(std::vector<LineVertex> a_lVertices);

	/// <summary>
	/// Loads an outliner from an OBJ file.
	/// </summary>
	Outliner(std::string a_sObjFile);

	/// <summary>
	/// Destructs the Outliner.
	/// </summary>
	~Outliner(void);

	/// <summary>
	/// Copy operator implementation for the Outliner.
	/// </summary>
	Outliner& operator=(const Outliner& a_Other);

	/// <summary>
	/// Copy constructor implementation for the Outliner.
	/// </summary>
	Outliner(const Outliner& a_Other);

	/// <summary>
	/// Adds a line to the Outliner.
	/// </summary>
	void AddLine(LineVertex a_Vertex1, LineVertex a_Vertex2);

	/// <summary>
	/// Sets the Index and Vertex buffers for rendering.
	/// </summary>
	void CompileBuffers(void);

	/// <summary>
	/// Renders the line mesh.
	/// </summary>
	void Draw(void);

	/// <summary>
	/// Get accessor for the Transform.
	/// </summary>
	std::shared_ptr<Transform> GetTransform(void);

	/// <summary>
	/// Sets the saved transform object.
	/// </summary>
	void SetTransform(Transform a_Transform);

	/// <summary>
	/// Checks whether or not the Outliner has its buffers compiled.
	/// </summary>
	bool IsCompiled(void);
};

#endif //__OUTLINER_H_