#include "Mesh.h"

Mesh::Mesh(Vertex* a_pVertices, int a_dVertexCount, unsigned int* a_pIndices, int a_dIndexCount)
{
}

Mesh::Mesh(const char* a_sFilepath)
{
}

Mesh::~Mesh()
{
}

Mesh::Mesh(const Mesh& a_pOther)
{
}

Mesh& Mesh::operator=(const Mesh& a_pOther)
{
	// TODO: insert return statement here
}

BufferPtr Mesh::GetVertexBuffer(void)
{
	return BufferPtr();
}

BufferPtr Mesh::GetIndexBuffer(void)
{
	return BufferPtr();
}

int Mesh::GetIndexCount(void)
{
	return 0;
}

int Mesh::GetVertexCount(void)
{
	return 0;
}

void Mesh::Draw(void)
{
}

void Mesh::CalculateTangents(Vertex* a_lVertices, int a_dVertexCount, unsigned int* a_lIndices, int a_dIndexCount)
{
}
