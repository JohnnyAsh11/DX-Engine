#ifndef __VERTEX_H_
#define __VERTEX_H_

#include <DirectXMath.h>

/// <summary>
/// Defines the data for a single vertex in a mesh.
/// </summary>
struct Vertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT3 Tangent;
	DirectX::XMFLOAT4 Color;
};

#endif //__VERTEX_H_