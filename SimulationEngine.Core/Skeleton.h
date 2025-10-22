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

/// <summary>
/// Defines the individual joints within the Skeleton data structure.
/// </summary>
struct Joint
{
	Matrix4 InvBindPose;
	std::string Name;
	unsigned int ParentIndex;
};

/// <summary>
/// Contains the array based tree structure used for a collection of Joints.
/// </summary>
class Skeleton
{
private:
	/// <summary>
	/// The Joint count tracks the number of joints actively in the collection.
	/// </summary>
	unsigned int m_uJointCount = 0;
	/// <summary>
	/// The Skeleton Capacity tracks the number of joints that can currently fit in the collection.
	/// </summary>
	unsigned int m_uSkeletonCapacity = 4;
	Joint* m_pJoints = nullptr;

public:
	/// <summary>
	/// Constructs an empty Skeleton structure.
	/// </summary>
	Skeleton();

	/// <summary>
	/// Destructs Skeletons.
	/// </summary>
	~Skeleton();
	/// <summary>
	/// Copy operator for the Skeleton structure.
	/// </summary>
	Skeleton& operator= (const Skeleton& a_Other);
	/// <summary>
	/// Copy constructor for the Skeleton structure.
	/// </summary>
	Skeleton(const Skeleton& a_Other);

	void AddJoint(Joint a_NewJoint);

private:
	void ResortJointsByParentIndex(void);
};

#endif //__SKELETON_H_
