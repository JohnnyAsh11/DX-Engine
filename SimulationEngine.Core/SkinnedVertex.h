#ifndef __SKINNEDVERTEX_H_
#define __SKINNEDVERTEX_H_

#include "Vertex.h"

#define MAX_JOINT_COUNT 4

struct SkinnedVertex : Vertex
{
	Vector3 JointWeights;
	int JointIndices[MAX_JOINT_COUNT] = {-1, -1, -1, -1};
};

#endif //__SKINNEDVERTEX_H_