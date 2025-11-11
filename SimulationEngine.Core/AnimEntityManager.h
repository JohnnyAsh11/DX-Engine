#ifndef __ANIMENTITYMANAGER_H_
#define __ANIMENTITYMANAGER_H_

#include <memory>
#include <vector>

#include "EntityManager.h"
#include "CBufferMapper.h"
#include "AnimatedEntity.h"
#include "Camera.h"
#include "Lights.h"

// Setting the register location for the AnimEntity Shader program.
#define ANIM_CBUFFER_REGISTER 4

/// <summary>
/// Manages the Animated Entities within the simulation.
/// </summary>
class AnimEntityManager
{
private:
	Light m_Lights[MAX_LIGHT_COUNT] = {};
	
	std::shared_ptr<Shader> m_pShader;

	std::shared_ptr<CBufferMapper<AnimCBufferVS>> m_pVertexCBuffer;
	std::shared_ptr<CBufferMapper<MaterialCBufferData>> m_pPixelCBuffer;

	std::vector<std::shared_ptr<AnimatedEntity>> m_lEntities;

public:
	/// <summary>
	/// Constructs the Animated Entity Manager.
	/// </summary>
	AnimEntityManager(std::shared_ptr<Shader> a_pShader);

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
	/// Adds the light to the array of lights at the specified enum index.
	/// </summary>
	void AddLight(Light a_LightDesc, LightIndex a_LightIndex);

	/// <summary>
	/// Adds the passed in AnimEntity to the manager's collection.
	/// </summary>
	void AddAnimEntity(std::shared_ptr<AnimatedEntity> a_pAnimEntity);

	/// <summary>
	/// Renders all of the Animated Entities.
	/// </summary>
	void Draw(std::shared_ptr<Camera> a_pCamera);

	/// <summary>
	/// Gets the collection of Animated Entities.
	/// </summary>
	std::vector<std::shared_ptr<AnimatedEntity>> GetEntities(void);

};

#endif //__ANIMENTITYMANAGER_H_

