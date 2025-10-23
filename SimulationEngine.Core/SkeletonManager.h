#ifndef __SKELETONMANAGER_H_
#define __SKELETONMANAGER_H_

#include <map>

#include "Skeleton.h"

/// <summary>
/// Manages a map of skeletons to IDs. 
/// </summary>
class SkeletonManager
{
private:
	std::map<unsigned int, Skeleton> m_mSkeletons;

public:
	/// <summary>
	/// Constructs an empty SkeletonManager.
	/// </summary>
	SkeletonManager(void);

	/// <summary>
	/// Destructor implementation for the SkeletonManager.
	/// </summary>
	~SkeletonManager(void);
	/// <summary>
	/// Copy operator implementation for the SkeletonManager.
	/// </summary>
	SkeletonManager& operator =(const SkeletonManager& a_Other);
	/// <summary>
	/// Copy constructor implementation for the SkeletonManager.
	/// </summary>
	SkeletonManager(const SkeletonManager& a_Other);

	/// <summary>
	/// Adds a skeleton to the map of skeletons.
	/// </summary>
	void AddSkeleton(unsigned int a_uKey, Skeleton a_Skeleton);

	/// <summary>
	/// Gets the map of skeletons and their unique keys.
	/// </summary>
	std::map<unsigned int, Skeleton> GetSkeletons(void);
};

#endif //__SKELETONMANAGER_H_
