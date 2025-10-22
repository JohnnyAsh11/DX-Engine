#ifndef __SKELETON_H_
#define __SKELETON_H_

#include <string>

#include "Vectors.h"

// - - - - - - - - - - - - - - - - - - - -
//	 Code for these data structures
//   was heavily influenced by the 
//	 Game Engine Architecture textbook
//   by Jason Gregory.
// - - - - - - - - - - - - - - - - - - - -

struct Joint
{
	Matrix4 m_m4InvBindPose;
	std::string m_sName;
	unsigned int m_uParentIndex;
};

class Skeleton
{
private:
	unsigned int m_uJointCount;
	Joint* m_aJoints;

public:
};

#endif //__SKELETON_H_
