#ifndef __SKINNEDVERTEX_H_
#define __SKINNEDVERTEX_H_

#include "Vertex.h"

struct SkinnedVertex : Vertex
{
	Vector3 JointWeights;
	unsigned int JointIndices[4];
};

#endif //__SKINNEDVERTEX_H_