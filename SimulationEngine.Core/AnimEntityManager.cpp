#include "AnimEntityManager.h"

AnimEntityManager::AnimEntityManager(void)
{

}

AnimEntityManager::~AnimEntityManager(void)
{

}

AnimEntityManager::AnimEntityManager(const AnimEntityManager& a_other)
{

}

AnimEntityManager& AnimEntityManager::operator=(const AnimEntityManager& a_other)
{
	return *this;
}

void AnimEntityManager::Draw(void)
{

}

std::vector<std::shared_ptr<AnimatedEntity>> AnimEntityManager::GetEntities(void) { return m_lEntities; }
