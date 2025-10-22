#include "Skeleton.h"

#include <vector>
#include <algorithm>

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

		// Create a new array of this capacity.
		Joint* higherCapacity = new Joint[m_uSkeletonCapacity];

		// Copy all of the data over to the new array.
		for (unsigned int i = 0; i < m_uJointCount; i++)
		{
			higherCapacity[i] = m_pJoints[i];
		}

		// Deleting the previous array and replacing it with the new one.
		delete[] m_pJoints;
		m_pJoints = higherCapacity;
	}

	// Assigning the new joint to the collection and incrementing the count.
	m_pJoints[m_uJointCount] = a_NewJoint;
	m_uJointCount++;

	// Sorting all of the joints by their parent index.
	std::sort(&m_pJoints[0], &m_pJoints[m_uJointCount], [](const Joint& j1, const Joint& j2)
		{
			return j1.ParentIndex < j2.ParentIndex;
		});
}

Joint* Skeleton::GetJoints(void) { return m_pJoints; }
