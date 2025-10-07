#include "Outliner.h"
#include <fstream>

Outliner::Outliner(void)
{
	m_pTransform = std::make_shared<Transform>();
}

Outliner::Outliner(std::vector<LineVertex> a_lVertices) : Outliner()
{
	m_lVertices = a_lVertices;

	// Collecting the indices together into the vector.
	for (unsigned int i = 0; i < m_lVertices.size(); i++)
	{
		m_lIndices.push_back(i);
	}

	m_uVertexCount = m_lVertices.size();
	m_uIndexCount = m_lIndices.size();

	CompileBuffers();
}

Outliner::Outliner(std::string a_sObjFile)
{
	std::ifstream obj(a_sObjFile);

	// Ensure that the file was properly found.
	if (!obj.is_open())
	{
		throw std::invalid_argument(
			"Error opening file: Invalid file path or file is inaccessible due to access levels."
		);
	}

	std::vector<Vector3> positions;
	std::vector<LineVertex> verts;
	std::vector<unsigned int> indices;

	// Count of each index and vertex.
	int vertCounter = 0;
	int indexCounter = 0;

	// For loading lines of the obj files.
	char chars[256];

	while (obj.good())
	{
		// Getting a line from the file.
		obj.getline(chars, 256);

		if (chars[0] == 'v' && 
			chars[1] != 'n' && 
			chars[1] != 't')
		{
			// Read the 3 numbers directly into an Vector3
			Vector3 position;
			sscanf_s(
				chars,
				"v %f %f %f",
				&position.x,
				&position.y,
				&position.z
			);

			// Add to the collection of positions.
			positions.push_back(position);
		}
		else if (chars[0] == 'f')
		{
			// Reading the indices from the obj file.
			// This assumes the obj contains vertex positions/UVs/normals.
			unsigned int i[12];
			int numbersRead = sscanf_s(
				chars,
				"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
				&i[0], &i[1], &i[2],
				&i[3], &i[4], &i[5],
				&i[6], &i[7], &i[8],
				&i[9], &i[10], &i[11]);

			// If there was only one number then the obj has no UV coords.
			// Continue loading without crashing so re-reading in a different way.
			if (numbersRead == 1)
			{
				// Re-read with a different pattern.
				numbersRead = sscanf_s(
					chars,
					"f %d//%d %d//%d %d//%d %d//%d",
					&i[0], &i[2],
					&i[3], &i[5],
					&i[6], &i[8],
					&i[9], &i[11]);

				// The following indices are where the UVs coords should 
				// have been, so giving them a valid value.
				i[1] = 1;
				i[4] = 1;
				i[7] = 1;
				i[10] = 1;
			}

			LineVertex v1_1;
			v1_1.Position = positions[i[0] - 1];
			LineVertex v1_2;
			v1_2.Position = positions[i[3] - 1];

			LineVertex v2_1;
			v2_1.Position = positions[i[3] - 1];
			LineVertex v2_2;
			v2_1.Position = positions[i[6] - 1];

			LineVertex v3_1;
			v3_1.Position = positions[i[6] - 1];
			LineVertex v3_2;
			v3_2.Position = positions[i[0] - 1];

			verts.push_back(v1_1);
			verts.push_back(v1_2);
			verts.push_back(v3_1);
			verts.push_back(v3_2);
			verts.push_back(v2_1);
			verts.push_back(v2_2);
			vertCounter += 6;

			for (int i = 0; i < 6; i++)
			{
				indices.push_back(indexCounter); indexCounter += 1;
			}
		}
	}
}

Outliner::~Outliner(void)
{
	m_pVertexBuffer.Reset();
	m_pIndexBuffer.Reset();
}

Outliner& Outliner::operator=(const Outliner& a_Other)
{
	m_pVertexBuffer.Reset();
	m_pIndexBuffer.Reset();

	m_pVertexBuffer = a_Other.m_pVertexBuffer;
	m_pIndexBuffer = a_Other.m_pIndexBuffer;

	m_uIndexCount = a_Other.m_uIndexCount;
	m_uVertexCount = a_Other.m_uVertexCount;

	m_lVertices = a_Other.m_lVertices;
	m_lIndices = a_Other.m_lIndices;

	return *this;
}

Outliner::Outliner(const Outliner& a_Other)
{
	m_pVertexBuffer.Reset();
	m_pIndexBuffer.Reset();

	m_pVertexBuffer = a_Other.m_pVertexBuffer;
	m_pIndexBuffer = a_Other.m_pIndexBuffer;

	m_uIndexCount = a_Other.m_uIndexCount;
	m_uVertexCount = a_Other.m_uVertexCount;

	m_lVertices = a_Other.m_lVertices;
	m_lIndices = a_Other.m_lIndices;
}

void Outliner::AddLine(LineVertex a_Vertex1, LineVertex a_Vertex2)
{
	const int AddedVertexCount = 2;
	m_lVertices.push_back(a_Vertex1);
	m_lVertices.push_back(a_Vertex2);
	m_uVertexCount += AddedVertexCount;

	for (unsigned int i = 0; i < AddedVertexCount; i++)
	{
		m_lIndices.push_back(m_uIndexCount);
		m_uIndexCount++;
	}
}

void Outliner::CompileBuffers(void)
{
	if (m_bIsCompiled)
	{
		return;
	}

	// Setting up the vertex buffer setup struct object.
	D3D11_BUFFER_DESC vbd = {};
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(LineVertex) * m_uVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Setting up the index buffer setup struct object.
	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * m_uIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Creating the structs that actually hold the buffer data
	D3D11_SUBRESOURCE_DATA initialVertexData = {};
	D3D11_SUBRESOURCE_DATA initialIndexData = {};

	// Setting the system memory to hold the buffer data.
	initialVertexData.pSysMem = &m_lVertices[0];
	initialIndexData.pSysMem = &m_lIndices[0];

	// Creating the buffers.
	Graphics::GetDevice()->CreateBuffer(&vbd, &initialVertexData, m_pVertexBuffer.GetAddressOf());
	Graphics::GetDevice()->CreateBuffer(&ibd, &initialIndexData, m_pIndexBuffer.GetAddressOf());
	m_bIsCompiled = true;
}

void Outliner::Draw()
{
	// Setting the stride to be the memory size of a Vertex
	UINT stride = sizeof(LineVertex);

	// The offset starts at the first piece of data.
	UINT offset = 0;

	// Setting this Mesh's buffers as the next thing to draw.
	Graphics::GetContext()->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	Graphics::GetContext()->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Starting up the render pipeline and drawing the currently set Index and Vertex buffers.
	Graphics::GetContext()->DrawIndexed(
		m_uIndexCount,		// The number of indices to use.
		0,					// Offset from the first index to use.
		0);					// Offset to add to each index.
}

std::shared_ptr<Transform> Outliner::GetTransform(void)
{
	return m_pTransform;
}

bool Outliner::IsCompiled(void)
{
	return m_bIsCompiled;
}
