#include "Mesh.h"
#include "Graphics.h"

Mesh::Mesh(Vertex* a_pVertices, int a_dVertexCount, UINT* a_pIndices, int a_dIndexCount)
{
	// Saving the passed in values to the member fields.
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;

	m_dVertexCount = a_dVertexCount;
	m_dIndexCount = a_dIndexCount;

	// Calculating vertex tangents.
	CalculateTangents(a_pVertices, a_dVertexCount, a_pIndices, a_dIndexCount);

	// Setting up the vertex buffer setup struct object.
	D3D11_BUFFER_DESC vbd = {};
	vbd.Usage = D3D11_USAGE_IMMUTABLE;				
	vbd.ByteWidth = sizeof(Vertex) * a_dVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		
	vbd.CPUAccessFlags = 0;							
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Setting up the index buffer setup struct object.
	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_IMMUTABLE;					
	ibd.ByteWidth = sizeof(UINT) * a_dIndexCount;
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

Mesh::Mesh(const char* a_sFilepath)
{
}

Mesh::~Mesh()
{
	// Freeing memory.
	m_pVertexBuffer.Reset();
	m_pIndexBuffer.Reset();
}

Mesh::Mesh(const Mesh& a_pOther)
{
}

Mesh& Mesh::operator=(const Mesh& a_pOther)
{
	Mesh mesh = Mesh(a_pOther);
	return mesh;
}

BufferPtr Mesh::GetVertexBuffer(void)
{
	return m_pVertexBuffer;
}

BufferPtr Mesh::GetIndexBuffer(void)
{
	return m_pIndexBuffer;
}

int Mesh::GetIndexCount(void)
{
	return m_dIndexCount;
}

int Mesh::GetVertexCount(void)
{
	return m_dVertexCount;
}

void Mesh::Draw(void)
{
	// Setting the stride to be the memory size of a Vertex
	UINT stride = sizeof(Vertex);

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

void Mesh::CalculateTangents(Vertex* a_lVertices, int a_dVertexCount, UINT* a_lIndices, int a_dIndexCount)
{
}
