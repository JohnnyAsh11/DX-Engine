#include "SkeletonManager.h"

SkeletonManager::SkeletonManager(void) { }
SkeletonManager::~SkeletonManager(void) { }

SkeletonManager& SkeletonManager::operator=(const SkeletonManager& a_Other)
{
	m_mSkeletons = a_Other.m_mSkeletons;
	return *this;
}

SkeletonManager::SkeletonManager(const SkeletonManager& a_Other)
{
	m_mSkeletons = a_Other.m_mSkeletons;
}

void SkeletonManager::AddSkeleton(unsigned int a_uKey, Skeleton a_Skeleton)
{
	m_mSkeletons.insert({ a_uKey, a_Skeleton });
}

std::map<unsigned int, Skeleton> SkeletonManager::GetSkeletons(void)
{
	return m_mSkeletons;
}
