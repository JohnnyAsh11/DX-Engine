#ifndef __ANIMATEDENTITY_H_
#define __ANIMATEDENTITY_H_

#include <string>
#include <memory>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Material.h"
#include "AnimatedMesh.h"
#include "Skeleton.h"
#include "Transform.h"
#include "CBufferMapper.h"
#include "Camera.h"
#include "CBuffers.h"

// Combines an index with the assimp bone structure.
struct LoadedBone
{
	aiNode* Bone;
	unsigned int ParentIndex;
};

/// <summary>
/// Contains all of the AnimatedEntity data.
/// </summary>
class AnimatedEntity
{
private:
	std::shared_ptr<Skeleton> m_pRootSkeleton = nullptr;
	std::shared_ptr<Transform> m_pTransform = nullptr;
	std::map<std::shared_ptr<Material>, std::shared_ptr<AnimatedMesh>> m_mSubEntities;

public:
	/// <summary>
	/// Loads and constructs an fbx model containing animations.
	/// </summary>
	AnimatedEntity(
		std::string a_sFbxFile, 
		std::shared_ptr<Shader> a_pShader, 
		Microsoft::WRL::ComPtr<ID3D11SamplerState> a_pSampler);

	/// <summary>
	/// AnimatedEntity implementation of the destructor.
	/// </summary>
	~AnimatedEntity(void);
	/// <summary>
	/// AnimatedEntity implementation of the copy operator.
	/// </summary>
	AnimatedEntity& operator =(const AnimatedEntity& a_Other);
	/// <summary>
	/// AnimatedEntity implementation of the copy constructor.
	/// </summary>
	AnimatedEntity(const AnimatedEntity& a_Other);

	void Draw(
		std::shared_ptr<CBufferMapper<VertexCBufferData>> a_pVertexCBufferMapper,
		std::shared_ptr<CBufferMapper<MaterialCBufferData>> a_pPixelCBufferMapper, 
		std::shared_ptr<Camera> a_pCamera,
		Light a_Lights);

private:
	/// <summary>
	/// Processes the loaded Assimp scene into SimulationEngine data structures.
	/// </summary>
	void ProcessAssimpScene(
		const aiScene* scene,
		std::shared_ptr<Shader> a_pShader,
		Microsoft::WRL::ComPtr<ID3D11SamplerState> a_pSampler);

	/// <summary>
	/// Constructs a D3D11 Shader Resource View out of an Assimp aiTexture.
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ProcessAssimpTexture(const aiTexture* texture);
};

#endif //__ANIMATEDENTITY_H_
