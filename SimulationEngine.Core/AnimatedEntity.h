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

struct AnimCBufferVS
{
	Matrix4 World;
	Matrix4 WorldInvTranspose;
	Matrix4 View;
	Matrix4 Projection;
};

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
	/// Loads and constructs a model from any file ending that can contain animations.
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

	/// <summary>
	/// Renders the Animated Entity.
	/// </summary>
	void Draw(
		std::shared_ptr<CBufferMapper<AnimCBufferVS>> a_pVertexCBufferMapper,
		std::shared_ptr<CBufferMapper<MaterialCBufferData>> a_pPixelCBufferMapper,
		std::shared_ptr<Camera> a_pCamera,
		Light a_Lights[MAX_LIGHT_COUNT] = {});

	/// <summary>
	/// Gets this Entity's Transform.
	/// </summary>
	std::shared_ptr<Transform> GetTransform(void);

private:
	/// <summary>
	/// Processes the loaded Assimp scene into SimulationEngine data structures.
	/// </summary>
	void ProcessAssimpScene(
		const aiScene* scene,
		std::shared_ptr<Shader> a_pShader,
		Microsoft::WRL::ComPtr<ID3D11SamplerState> a_pSampler);

	/// <summary>
	/// Processes the Assimp scene's skeleton data.
	/// </summary>
	void ProcessAssimpSkeleton(const aiScene* scene);

	/// <summary>
	/// Processes the Assimp scene's vertex/index data.
	/// </summary>
	void ProcessAssimpVertices(
		const aiScene* scene, 
		std::map<unsigned int, std::shared_ptr<Material>> a_mMaterials);
	
	/// <summary>
	/// Processes the Assimp scene's material data.
	/// </summary>
	std::map<unsigned int, std::shared_ptr<Material>> ProcessAssimpMaterials(
		const aiScene* scene,
		std::shared_ptr<Shader> a_pShader,
		Microsoft::WRL::ComPtr<ID3D11SamplerState> a_pSampler);

	/// <summary>
	/// Processes the Assimp animation data.
	/// </summary>
	void ProcessAssimpAnimations(const aiScene* scene);

	/// <summary>
	/// Constructs a D3D11 Shader Resource View out of an Assimp aiTexture.
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ProcessAssimpTexture(const aiTexture* texture);
};

#endif //__ANIMATEDENTITY_H_
