#ifndef __CBUFFERS_H_
#define __CBUFFERS_H_

#include "Vectors.h"

/// <summary>
/// Constant Buffer vertex for entities in the sim world.
/// </summary>
struct VertexCBufferData
{
	Matrix4 World;
	Matrix4 WorldInvTranspose;
	Matrix4 View;
	Matrix4 Projection;
};

#endif //__CBUFFERS_H_