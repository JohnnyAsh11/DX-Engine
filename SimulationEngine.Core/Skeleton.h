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
	int Id;
	std::string Name;
	Matrix4 InvBindPose{};
	int ParentIndex = -1;
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
	unsigned int m_uSkeletonCapacity = 2;

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

	/// <summary>
	/// Adds a new joint to the skeleton structure and sorts everything by parent indices.
	/// Example: root > parent > child > child > sub child 1 > sub child 2
	/// </summary>
	void AddJoint(Joint a_NewJoint);

	/// <summary>
	/// Allows full access to the joint data held within the Skeleton.
	/// </summary>
	Joint* GetJoints(void);

	/// <summary>
	/// Gets the amount of joints in the skeleton.
	/// </summary>
	unsigned int GetJointCount(void);
};

#endif //__SKELETON_H_
