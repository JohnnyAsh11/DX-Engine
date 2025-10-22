#include "Skeleton.h"

#include <vector>

Skeleton::Skeleton()
{
	m_pJoints = new Joint[m_uSkeletonCapacity];
}

Skeleton::~Skeleton()
{
	if (m_pJoints != nullptr)
	{
		delete[] m_pJoints;
	}
}

Skeleton& Skeleton::operator=(const Skeleton& a_Other)
{
	if (m_pJoints != nullptr)
	{
		delete[] m_pJoints;
	}

	// Copying all of the Joint data over.
	m_pJoints = new Joint[a_Other.m_uSkeletonCapacity];
	for (unsigned int i = 0; i < a_Other.m_uJointCount; i++)
	{
		m_pJoints[i] = a_Other.m_pJoints[i];
	}

	m_uJointCount = a_Other.m_uJointCount;
	m_uSkeletonCapacity = a_Other.m_uSkeletonCapacity;

	return *this;
}

Skeleton::Skeleton(const Skeleton& a_Other)
{
	// Copying all of the Joint data over.
	m_pJoints = new Joint[m_uSkeletonCapacity];
	for (unsigned int i = 0; i < a_Other.m_uJointCount; i++)
	{
		m_pJoints[i] = a_Other.m_pJoints[i];
	}

	m_uJointCount = a_Other.m_uJointCount;
	m_uSkeletonCapacity = a_Other.m_uSkeletonCapacity;
}

void Skeleton::AddJoint(Joint a_NewJoint)
{
	if (m_uJointCount == m_uSkeletonCapacity)
	{
		// Increasing the size of the joints array.
		m_uSkeletonCapacity *= 2;
		Joint* higherCapacity = new Joint[m_uSkeletonCapacity];
		memcpy(higherCapacity, m_pJoints, m_uJointCount);
		delete[] m_pJoints;
		m_pJoints = new Joint[m_uSkeletonCapacity];
		m_pJoints = higherCapacity;
	}

	m_pJoints[m_uJointCount] = a_NewJoint;
	m_uJointCount++;
}

void Skeleton::ResortJointsByParentIndex(void)
{
	std::vector<Joint> lJoints;
	memcpy(&lJoints[0], m_pJoints, m_uJointCount);

	Joint* sortedArray = new Joint[m_uSkeletonCapacity];

	for (unsigned int i = 0; i < lJoints.size(); i++)
	{
		Joint currentJoint = lJoints[i];
		unsigned int uTargetIndex = i;

		for (unsigned int j = 0; j < lJoints.size(); j++)
		{
			if (currentJoint.ParentIndex < lJoints[j].ParentIndex)
			{
				sortedArray[i] = currentJoint;
			}
			else
			{
				currentJoint = lJoints[j];
				uTargetIndex = j;
			}
		}

		lJoints.erase(lJoints.begin() + uTargetIndex);
	}

	delete[] m_pJoints;
	m_pJoints = sortedArray;
}
