#ifndef __VERTEX_H_
#define __VERTEX_H_
#include "Vectors.h"

/// <summary>
/// Defines the data for a single vertex in a mesh.
/// </summary>
struct Vertex
{
	Vector3 Position;
	Vector3 Normal;
	Vector2 UV;
	Vector3 Tangent;
};

#endif //__VERTEX_H_