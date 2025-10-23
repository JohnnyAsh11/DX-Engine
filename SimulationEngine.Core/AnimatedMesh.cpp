#include "AnimatedMesh.h"
#include "Utils.h"
#include "Material.h"
#include "Shader.h"

AnimatedMesh::AnimatedMesh(
	SkinnedVertex* a_pVertices,
	unsigned int a_uVertexCount,
	unsigned int* a_pIndices,
	unsigned int a_uIndexCount,
	TangentType a_TangentType)
{
	// Saving the passed in values to the member fields.
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;

	m_dVertexCount = a_uVertexCount;
	m_dIndexCount = a_uIndexCount;

	// Calculating vertex tangents.
	/*Mesh::CalculateTangents(
		a_pVertices,
		m_dVertexCount,
		a_pIndices,
		m_dIndexCount,
		a_TangentType);*/

	// Setting up the vertex buffer setup struct object.
	D3D11_BUFFER_DESC vbd = {};
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(SkinnedVertex) * m_dVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Setting up the index buffer setup struct object.
	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * m_dIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Creating the structs that actually hold the buffer data
	D3D11_SUBRESOURCE_DATA initialVertexData = {};
	D3D11_SUBRESOURCE_DATA initialIndexData = {};

	// Setting the system memory to hold the buffer data.
	initialVertexData.pSysMem = a_pVertices;
	initialIndexData.pSysMem = a_pIndices;

	// Creating the buffers.
	Graphics::GetDevice()->CreateBuffer(&vbd, &initialVertexData, m_pVertexBuffer.GetAddressOf());
	Graphics::GetDevice()->CreateBuffer(&ibd, &initialIndexData, m_pIndexBuffer.GetAddressOf());
}

BufferPtr AnimatedMesh::GetVertexBuffer(void) { return m_pVertexBuffer; }
BufferPtr AnimatedMesh::GetIndexBuffer(void) { return m_pIndexBuffer; }
int AnimatedMesh::GetIndexCount(void) { return m_dIndexCount; }
int AnimatedMesh::GetVertexCount(void) { return m_dVertexCount; }

void AnimatedMesh::Draw(void)
{
	// Setting the stride to be the memory size of a Vertex
	UINT stride = sizeof(SkinnedVertex);

	// The offset starts at the first piece of data.
	UINT offset = 0;

	// Setting this Mesh's buffers as the next thing to draw.
	Graphics::GetContext()->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	Graphics::GetContext()->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Starting up the render pipeline and drawing the currently set Index and Vertex buffers.
	Graphics::GetContext()->DrawIndexed(
		m_dIndexCount,		// The number of indices to use.
		0,					// Offset from the first index to use.
		0);					// Offset to add to each index.
}