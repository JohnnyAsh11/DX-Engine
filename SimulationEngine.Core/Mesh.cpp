#include "Mesh.h"
#include "Graphics.h"
#include "Vectors.h"
#include "LineManager.h"

#include <fstream>

#define VECTOR3_ZERO Vector3(0.0f, 0.0f, 0.0f)

using namespace DirectX;

Mesh::Mesh(VertexPack a_VertexData, IndexPack a_IndexData)
{
	// Saving the passed in values to the member fields.
	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;

	m_dVertexCount = a_VertexData.m_nVertexCount;
	m_dIndexCount = a_IndexData.m_nIndexCount;

	// Calculating vertex tangents.
	CalculateTangents(
		a_VertexData.m_pVertices, 
		m_dVertexCount, 
		a_IndexData.m_pIndices, 
		m_dIndexCount);

	// Setting up the vertex buffer setup struct object.
	D3D11_BUFFER_DESC vbd = {};
	vbd.Usage = D3D11_USAGE_IMMUTABLE;				
	vbd.ByteWidth = sizeof(Vertex) * m_dVertexCount;
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
	initialVertexData.pSysMem = a_VertexData.m_pVertices;
	initialIndexData.pSysMem = a_IndexData.m_pIndices;

	// Creating the buffers.
	Graphics::GetDevice()->CreateBuffer(&vbd, &initialVertexData, m_pVertexBuffer.GetAddressOf());
	Graphics::GetDevice()->CreateBuffer(&ibd, &initialIndexData, m_pIndexBuffer.GetAddressOf());
}

Mesh::Mesh(std::string a_sObjDirectory, std::string a_sObjName)
{
	LoadObj(a_sObjDirectory, a_sObjName);
}

Mesh::~Mesh()
{
	// Freeing memory.
	m_pVertexBuffer.Reset();
	m_pIndexBuffer.Reset();
}

Mesh::Mesh(const Mesh& a_pOther)
{
	m_pVertexBuffer = a_pOther.m_pVertexBuffer;
	m_dVertexCount = a_pOther.m_dVertexCount;
	m_pIndexBuffer = a_pOther.m_pIndexBuffer;
	m_dIndexCount = a_pOther.m_dIndexCount;
}

Mesh& Mesh::operator=(const Mesh& a_pOther)
{
	// Freeing the memory in the ComPtrs.
	m_pVertexBuffer.Reset();
	m_pIndexBuffer.Reset();

	// Setting values.
	m_pVertexBuffer = a_pOther.m_pVertexBuffer;
	m_dVertexCount = a_pOther.m_dVertexCount;
	m_pIndexBuffer = a_pOther.m_pIndexBuffer;
	m_dIndexCount = a_pOther.m_dIndexCount;

	return *this;
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

BufferPtr Mesh::GetVertexBuffer(void) { return m_pVertexBuffer; }
BufferPtr Mesh::GetIndexBuffer(void) { return m_pIndexBuffer; }
int Mesh::GetIndexCount(void) { return m_dIndexCount; }
int Mesh::GetVertexCount(void) { return m_dVertexCount; }

// --------------------------------------------------------
// Calculates the tangents of the vertices in a mesh
// - Code originally adapted from: http://www.terathon.com/code/tangent.html
// - Updated version found here: http://foundationsofgameenginedev.com/FGED2-sample.pdf
// --------------------------------------------------------
void Mesh::CalculateTangents(Vertex* a_lVertices, int a_dVertexCount, UINT* a_lIndices, int a_dIndexCount)
{
	// Reset tangents
	for (int i = 0; i < a_dVertexCount; i++)
	{
		a_lVertices[i].Tangent = Vector3(0, 0, 0);
	}
	// Calculate tangents one whole triangle at a time
	for (int i = 0; i < a_dIndexCount;)
	{
		// Grab indices and vertices of first triangle
		unsigned int i1 = a_lIndices[i++];
		unsigned int i2 = a_lIndices[i++];
		unsigned int i3 = a_lIndices[i++];
		Vertex* v1 = &a_lVertices[i1];
		Vertex* v2 = &a_lVertices[i2];
		Vertex* v3 = &a_lVertices[i3];
		// Calculate vectors relative to triangle positions
		float x1 = v2->Position.x - v1->Position.x;
		float y1 = v2->Position.y - v1->Position.y;
		float z1 = v2->Position.z - v1->Position.z;
		float x2 = v3->Position.x - v1->Position.x;
		float y2 = v3->Position.y - v1->Position.y;
		float z2 = v3->Position.z - v1->Position.z;
		// Do the same for vectors relative to triangle uv's
		float s1 = v2->UV.x - v1->UV.x;
		float t1 = v2->UV.y - v1->UV.y;
		float s2 = v3->UV.x - v1->UV.x;
		float t2 = v3->UV.y - v1->UV.y;
		// Create vectors for tangent calculation
		float r = 1.0f / (s1 * t2 - s2 * t1);
		float tx = (t2 * x1 - t1 * x2) * r;
		float ty = (t2 * y1 - t1 * y2) * r;
		float tz = (t2 * z1 - t1 * z2) * r;
		// Adjust tangents of each vert of the triangle
		v1->Tangent.x += tx;
		v1->Tangent.y += ty;
		v1->Tangent.z += tz;
		v2->Tangent.x += tx;
		v2->Tangent.y += ty;
		v2->Tangent.z += tz;
		v3->Tangent.x += tx;
		v3->Tangent.y += ty;
		v3->Tangent.z += tz;
	}
	// Ensure all of the tangents are orthogonal to the normals
	for (int i = 0; i < a_dVertexCount; i++)
	{
		// Grab the two vectors
		XMVector tangent = XMLoadFloat3(&a_lVertices[i].Tangent);
		XMVector normal = XMLoadFloat3(&a_lVertices[i].Normal);
		// Use Gram-Schmidt orthonormalize to ensure
		// the normal and tangent are exactly 90 degrees apart
		tangent = XMVector3Normalize(
			tangent - normal * XMVector3Dot(normal, tangent));
		// Store the tangent
		XMStoreFloat3(&a_lVertices[i].Tangent, tangent);
	}
}

void Mesh::LoadMtl(std::string a_sObjDirectory, std::string a_sMtlName)
{

}

void Mesh::LoadObj(std::string a_sObjDirectory, std::string a_sObjName)
{
	// ---------------------------------------
	//	 	This method takes parts of 
	//	 Professor Chris Cascioli's code of
	//	 Rochester Institute of Technology.
	// ---------------------------------------
	std::ifstream obj(a_sObjDirectory + a_sObjName);

	// Ensure that the file was properly found.
	if (!obj.is_open())
	{
		throw std::invalid_argument(
			"Error opening file: Invalid file path or file is inaccessible due to access levels."
		);
	}

	// Collected data for loading each individual vertex.
	std::vector<Vector3> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> uvs;
	std::vector<Vertex> verts;

	// Defines the index buffer for the vertices.
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

		std::string line = chars;
		if (strstr(chars, "mtllib"))
		{
			std::string matFileName = "";

			// Loading the mtl filename.
			int i = 7;
			while (chars[i] != '\0')
			{
				matFileName += chars[i];
				i++;
			}
		}

		// Check the type of line within the obj file.
		if (chars[0] == 'v' && chars[1] == 'n')
		{
			// Read the 3 numbers directly into an Vector3
			Vector3 normal;
			sscanf_s(
				chars,
				"vn %f %f %f",
				&normal.x,
				&normal.y,
				&normal.z
			);

			// Add the loaded normal to the list of normals.
			normals.push_back(normal);
		}
		else if (chars[0] == 'v' && chars[1] == 't')
		{
			// Reading the uv values into an Vector2.
			Vector2 uv;
			sscanf_s(
				chars,
				"vt %f %f",
				&uv.x,
				&uv.y
			);

			// Add to the list of UVs.
			uvs.push_back(uv);
		}
		else if (chars[0] == 'v')
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

				// If we have no UVs, create a single UV coordinate
				// that will be used for all vertices.
				if (uvs.size() == 0)
				{
					uvs.push_back(Vector2(0, 0));
				}
			}

			// Constructing the Vertex models.
			Vertex v1;
			v1.Position = positions[i[0] - 1];
			v1.UV = uvs[i[1] - 1];
			v1.Normal = normals[i[2] - 1];

			Vertex v2;
			v2.Position = positions[i[3] - 1];
			v2.UV = uvs[i[4] - 1];
			v2.Normal = normals[i[5] - 1];

			Vertex v3;
			v3.Position = positions[i[6] - 1];
			v3.UV = uvs[i[7] - 1];
			v3.Normal = normals[i[8] - 1];

			// ---------------------------------------------------------
			// Professor Chris Cascioli:
			// 
			// The model is most likely in a right-handed space,
			// especially if it came from Maya.  We want to convert
			// to a left-handed space for DirectX.  This means we 
			// need to:
			//  - Invert the Z position
			//  - Invert the normal's Z
			//  - Flip the winding order
			// We also need to flip the UV coordinate since DirectX
			// defines (0,0) as the top left of the texture, and many
			// 3D modeling packages use the bottom left as (0,0)
			// 
			// ---------------------------------------------------------

			// Flip the UV's since they're probably "upside down"
			v1.UV.y = 1.0f - v1.UV.y;
			v2.UV.y = 1.0f - v2.UV.y;
			v3.UV.y = 1.0f - v3.UV.y;

			// Flip Z (LH vs. RH)
			v1.Position.z *= -1.0f;
			v2.Position.z *= -1.0f;
			v3.Position.z *= -1.0f;

			// Flip normal's Z
			v1.Normal.z *= -1.0f;
			v2.Normal.z *= -1.0f;
			v3.Normal.z *= -1.0f;

			// Collecting the vertices into the vertex collection.
			verts.push_back(v1);
			verts.push_back(v3);
			verts.push_back(v2);
			vertCounter += 3;

			// Adding more indices according to the new Vertex additions.
			indices.push_back(indexCounter); indexCounter += 1;
			indices.push_back(indexCounter); indexCounter += 1;
			indices.push_back(indexCounter); indexCounter += 1;

			// Follow Professor Chris Cascioli's explanation:
			// Was there a 4th face?
			// - 12 numbers read means 4 faces WITH uv's
			// - 8 numbers read means 4 faces WITHOUT uv's
			if (numbersRead == 12 || numbersRead == 8)
			{
				// Make the last vertex
				Vertex v4;
				v4.Position = positions[i[9] - 1];
				v4.UV = uvs[i[10] - 1];
				v4.Normal = normals[i[11] - 1];

				// Flip the UV, Z pos and normal's Z
				v4.UV.y = 1.0f - v4.UV.y;
				v4.Position.z *= -1.0f;
				v4.Normal.z *= -1.0f;

				// Add a whole triangle (flipping the winding order)
				verts.push_back(v1);
				verts.push_back(v4);
				verts.push_back(v3);
				vertCounter += 3;

				// Add three more indices
				indices.push_back(indexCounter); indexCounter += 1;
				indices.push_back(indexCounter); indexCounter += 1;
				indices.push_back(indexCounter); indexCounter += 1;
			}
		}
	}

	// Close the file and creating the actual buffers.
	obj.close();

	m_dVertexCount = vertCounter;
	m_dIndexCount = indexCounter;

	// Copying the data into a heap allocated array for the tangent function.
	Vertex* lTempVertArr = new Vertex[verts.size()];
	unsigned int* lTempIndexArr = new unsigned int[indices.size()];

	// Copying the data from the vectors.
	std::memcpy(lTempVertArr, verts.data(), verts.size() * sizeof(Vertex));
	std::memcpy(lTempIndexArr, indices.data(), indices.size() * sizeof(unsigned int));

	// Calculate vertex tangents.
	CalculateTangents(lTempVertArr, m_dVertexCount, lTempIndexArr, m_dIndexCount);

	// Setting up the vertex buffer description struct object.
	D3D11_BUFFER_DESC vbd = {};
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * vertCounter;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Setting up the index buffer description struct object.
	D3D11_BUFFER_DESC ibd = {};
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int) * indexCounter;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Creating the structs that actually hold the buffer data.
	D3D11_SUBRESOURCE_DATA initialVertexData = {};
	D3D11_SUBRESOURCE_DATA initialIndexData = {};

	// Setting the system memory to hold the buffer data.
	initialVertexData.pSysMem = &lTempVertArr[0];
	initialIndexData.pSysMem = &lTempIndexArr[0];

	// Creating the buffers.
	Graphics::GetDevice()->CreateBuffer(&vbd, &initialVertexData, m_pVertexBuffer.GetAddressOf());
	Graphics::GetDevice()->CreateBuffer(&ibd, &initialIndexData, m_pIndexBuffer.GetAddressOf());

	// Only create the outliners in Debug.
#if defined(DEBUG) | defined(_DEBUG)
	CreateOutliner(positions);
#endif

	// Deleting the temporary arrays.
	delete[] lTempIndexArr;
	delete[] lTempVertArr;
}

void Mesh::CreateOutliner(std::vector<Vector3> a_lPositions)
{
	// X Values.
	Vector3 v3Leftmost = VECTOR3_ZERO;
	Vector3 v3Rightmost = VECTOR3_ZERO;

	// Y Values.
	Vector3 v3Lowest = VECTOR3_ZERO;
	Vector3 v3Highest = VECTOR3_ZERO;

	// Z Values.
	Vector3 v3Backmost = VECTOR3_ZERO;
	Vector3 v3Forwardmost = VECTOR3_ZERO;

	// Looping and getting the furthest vertices for each mesh.
	// Should be relatively efficient since it is mostly int comparisons.
	for (int i = 0; i < a_lPositions.size(); i++)
	{
		Vector3 v3Current = a_lPositions[i];

		if (v3Current.x < v3Leftmost.x)
		{
			v3Leftmost = v3Current;
		}

		if (v3Current.x > v3Rightmost.x)
		{
			v3Rightmost = v3Current;
		}

		if (v3Current.y > v3Highest.y)
		{
			v3Highest = v3Current;
		}

		if (v3Current.y < v3Lowest.y)
		{
			v3Lowest = v3Current;
		}

		if (v3Current.z > v3Backmost.z)
		{
			v3Backmost = v3Current;
		}

		if (v3Current.z < v3Forwardmost.z)
		{
			v3Forwardmost = v3Current;
		}
	}

	int uWidth = v3Rightmost.x - v3Leftmost.x;
	int uHeight = v3Highest.y - v3Lowest.y;
	int uLength = v3Backmost.z - v3Forwardmost.z;

	Vector3 v3BottomLeft = Vector3(v3Leftmost.x, v3Lowest.y, v3Forwardmost.z);
	Vector3 v3TopRight = Vector3(v3Rightmost.x, v3Highest.y, v3Backmost.z);

	std::vector<LineVertex> lLineVertices;

	lLineVertices.push_back({ v3BottomLeft });
	lLineVertices.push_back({ Vector3(v3BottomLeft.x + uWidth, v3BottomLeft.y, v3BottomLeft.z) });

	lLineVertices.push_back({ v3BottomLeft });
	lLineVertices.push_back({ Vector3(v3BottomLeft.x, v3BottomLeft.y, v3BottomLeft.z + uLength) });

	lLineVertices.push_back({ v3BottomLeft });
	lLineVertices.push_back({ Vector3(v3BottomLeft.x, v3BottomLeft.y + uHeight, v3BottomLeft.z) });

	lLineVertices.push_back({ v3TopRight });
	lLineVertices.push_back({ Vector3(v3TopRight.x - uWidth, v3TopRight.y, v3TopRight.z) });

	lLineVertices.push_back({ v3TopRight });
	lLineVertices.push_back({ Vector3(v3TopRight.x, v3TopRight.y - uHeight, v3TopRight.z) });

	lLineVertices.push_back({ v3TopRight });
	lLineVertices.push_back({ Vector3(v3TopRight.x, v3TopRight.y, v3TopRight.z - uLength) });

	std::shared_ptr<Outliner> pOutliner = std::make_shared<Outliner>(lLineVertices);
	LineManager::GetInstance()->AddOutliner(pOutliner);
}