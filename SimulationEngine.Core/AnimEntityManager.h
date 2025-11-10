#ifndef __ANIMENTITYMANAGER_H_
#define __ANIMENTITYMANAGER_H_

#include <memory>
#include <vector>

#include "CBufferMapper.h"
#include "AnimatedEntity.h"

struct VertexCBuffer
{

};

struct PixelCBuffer
{

};

/// <summary>
/// Manages the Animated Entities within the simulation.
/// </summary>
class AnimEntityManager
{
private: 
	std::shared_ptr<CBufferMapper<VertexCBuffer>> m_pVertexCBuffer;
	std::shared_ptr<CBufferMapper<PixelCBuffer>> m_pPixelCBuffer;
	std::vector<std::shared_ptr<AnimatedEntity>> m_lEntities;

public:
	/// <summary>
	/// Constructs the Animated Entity Manager.
	/// </summary>
	AnimEntityManager(void);

	/// <summary>
	/// Destructs the Animated Entity Manager.
	/// </summary>
	~AnimEntityManager(void);

	/// <summary>
	/// Copy constructor for the Animated Entity Manager.
	/// </summary>
	AnimEntityManager(const AnimEntityManager& a_other);

	/// <summary>
	/// Copy operator for the Animated Entity Manager.
	/// </summary>
	AnimEntityManager& operator=(const AnimEntityManager& a_other);

	/// <summary>
	/// Renders all of the Animated Entities.
	/// </summary>
	void Draw(void);

	/// <summary>
	/// Gets the collection of Animated Entities.
	/// </summary>
	std::vector<std::shared_ptr<AnimatedEntity>> GetEntities(void);

};

#endif //__ANIMENTITYMANAGER_H_

