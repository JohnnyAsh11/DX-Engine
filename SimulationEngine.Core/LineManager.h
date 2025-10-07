#ifndef __LINEMANAGER_H_
#define __LINEMANAGER_H_

#include "CBufferMapper.h"
#include "Shader.h"
#include "Outliner.h"
#include "Camera.h"

/// <summary>
/// Manages lines rendered throughout the simulation.
/// </summary>
class LineManager
{
private:
	std::shared_ptr<CBufferMapper<LineCBufferData>> m_pCBufferMapper = nullptr;
	std::shared_ptr<Shader> m_pShader = nullptr;

	std::vector<std::shared_ptr<Outliner>> m_lOutliners;
public:
	/// <summary>
	/// Constructs the LineManager class.
	/// </summary>
	LineManager();

	/// <summary>
	/// Destructs the LineManager class.
	/// </summary>
	~LineManager();

	/// <summary>
	/// Implementation of Copy Operator.
	/// </summary>
	LineManager& operator=(const LineManager& a_Other);

	/// <summary>
	/// Implemenation of Copy Constructor.
	/// </summary>
	LineManager(const LineManager& a_Other);

	/// <summary>
	/// Adds an Outliner to the collection of Outliners.
	/// </summary>
	void AddOutliner(std::shared_ptr<Outliner> a_pOutliner);

	/// <summary>
	/// Renders all Outliners.
	/// </summary>
	void Draw(std::shared_ptr<Camera> a_pCamera, Vector4 a_v4Color);

	/// <summary>
	/// Get Accessor for the collection of Outliners.
	/// </summary>
	std::vector<std::shared_ptr<Outliner>> GetOutliners(void);
};

#endif //__LINEMANAGER_H_
