#include "AnimatedEntity.h"
#include "Utils.h"

#include <queue>

AnimatedEntity::AnimatedEntity(
	std::string a_sFbxFile, 
	std::shared_ptr<Shader> a_pShader, 
	Microsoft::WRL::ComPtr<ID3D11SamplerState> a_pSampler)
{
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(a_sFbxFile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	ProcessAssimpScene(scene, a_pShader, a_pSampler);
}

AnimatedEntity::~AnimatedEntity(void)
{
}

AnimatedEntity& AnimatedEntity::operator=(const AnimatedEntity& a_Other)
{
	return *this;
}

AnimatedEntity::AnimatedEntity(const AnimatedEntity& a_Other)
{
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> AnimatedEntity::ProcessAssimpTexture(const aiTexture* texture)
{
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = texture->mWidth;
	desc.Height = texture->mHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = texture->pcData;
	initData.SysMemPitch = texture->mWidth * sizeof(uint32_t);
	initData.SysMemSlicePitch = 0;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex = nullptr;
	Graphics::GetDevice()->CreateTexture2D(&desc, &initData, &tex);

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv = nullptr;
	Graphics::GetDevice()->CreateShaderResourceView(tex.Get(), nullptr, &srv);

	return srv;
}

void AnimatedEntity::ProcessAssimpScene(
	const aiScene* scene,
	std::shared_ptr<Shader> a_pShader,
	Microsoft::WRL::ComPtr<ID3D11SamplerState> a_pSampler)
{
	unsigned int uCounter = 0;
	std::shared_ptr<Skeleton> pSkeleton = std::make_shared<Skeleton>();
	std::queue<aiNode*> bones;

	bones.push(scene->mRootNode);
	aiNode* current;
	aiNode* parent;

	while (bones.size() > 0)
	{
		// Dequeueing the first element.
		current = bones.front();
		bones.pop();

		// Creating a joint and adding it to the skeleton.
		Joint joint{};
		joint.Name = current->mName.C_Str();
		joint.ParentIndex = current->mParent ? -1 : uCounter;
		joint.InvBindPose = Utils::ConvertFromAssimpMatrix(current->mTransformation);
		pSkeleton->AddJoint(joint);

		// Pushing the child bones to the queue.
		for (int i = 0; i < current->mNumChildren; i++)
		{
			bones.push(current->mChildren[i]);
			parent = current;
		}
		uCounter++;
	}

	// Material loading:
	std::map<unsigned int, std::shared_ptr<Material>> modelMats;
	unsigned int uMaterialCount = scene->mNumMaterials;
	for (unsigned int i = 0; i < uMaterialCount; i++)
	{
		aiString str;
		aiMaterial* material = scene->mMaterials[i];

		// Creating the material and inserting the sampler.
		std::shared_ptr<Material> mat = std::make_shared<Material>(
			a_pShader, 
			Vector4(0.0f, 0.0f, 0.0f, 1.0f), 
			0.5f);
		mat->AddSampler(SAMPLER_REGISTER, a_pSampler);

		// Getting the diffuse texture map.
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &str) == AI_SUCCESS)
		{
			if (str.C_Str()[0] == '*')
			{
				int index = atoi(str.C_Str() + 1);
				mat->AddTexturesSRV(
					DIFFUSE_REGISTER,
					ProcessAssimpTexture(scene->mTextures[index]));
			}
		}
		// Getting the Normal texture map.
		if (material->GetTexture(aiTextureType_NORMALS, 0, &str) == AI_SUCCESS)
		{
			if (str.C_Str()[0] == '*')
			{
				int index = atoi(str.C_Str() + 1);
				mat->AddTexturesSRV(
					NORMAL_REGISTER,
					ProcessAssimpTexture(scene->mTextures[index]));
			}
		}
		// Getting the Shininess texture.
		if (material->GetTexture(aiTextureType_SHININESS, 0, &str) == AI_SUCCESS)
		{
			if (str.C_Str()[0] == '*')
			{
				int index = atoi(str.C_Str() + 1);
				mat->AddTexturesSRV(
					ROUGHNESS_REGISTER,
					ProcessAssimpTexture(scene->mTextures[index]));
			}
		}
		// Getting the Metalness texture.
		if (material->GetTexture(aiTextureType_METALNESS, 0, &str) == AI_SUCCESS)
		{
			if (str.C_Str()[0] == '*')
			{
				int index = atoi(str.C_Str() + 1);
				mat->AddTexturesSRV(
					METAL_REGISTER,
					ProcessAssimpTexture(scene->mTextures[index]));
			}
		}

		modelMats.insert({i, mat});
	}

	// Vertex/Mesh loading:
	unsigned int uMeshCount = scene->mNumMeshes;
	for (unsigned int i = 0; i < uMeshCount; i++)
	{
		SubEntity entity{};
		aiMesh* mesh = scene->mMeshes[i];
		unsigned int matIndex = mesh->mMaterialIndex;

		// Vertex Count.
		int uVertexCount = mesh->mNumVertices;
		// Index Count.
		int uIndexCount = uVertexCount;
		// Vertices.
		SkinnedVertex* pVertices = new SkinnedVertex[uVertexCount];
		// Indices.
		unsigned int* pIndices = new unsigned int[uIndexCount];

		// Populating the Index array.
		for (unsigned int j = 0; j < uIndexCount; j++)
		{
			pIndices[j] = j;
		}

		bool bHasNormals = mesh->HasNormals();
		bool bHasTangents = mesh->HasTangentsAndBitangents();
		bool bHasPositions = mesh->HasPositions();

		// Populating the Vertex array.
		for (unsigned int j = 0; j < uVertexCount; j++)
		{
			SkinnedVertex vertex{};

			// Vertex positions:
			if (bHasPositions)
			{
				aiVector3D position = mesh->mVertices[j];
				vertex.Position.x = position.x;
				vertex.Position.y = position.y;
				vertex.Position.z = position.z;
			}

			// Vertex Normals:
			if (bHasNormals)
			{
				aiVector3D normal = mesh->mNormals[j];
				vertex.Normal.x = normal.x;
				vertex.Normal.y = normal.y;
				vertex.Normal.z = normal.z;
			}

			// Vertex tangents:
			if (bHasTangents)
			{
				aiVector3D tangent = mesh->mTangents[j];
				vertex.Tangent.x = tangent.x;
				vertex.Tangent.y = tangent.y;
				vertex.Tangent.z = tangent.z;
			}

			// Vertex UVs:
			if (mesh->HasTextureCoords(j))
			{
				aiVector3D* UV = mesh->mTextureCoords[j];
				vertex.UV.x = UV->x;
				vertex.UV.y = UV->y;
			}

			pVertices[j] = vertex;
		}

		// Creating the skeleton for this SubEntity.
		std::shared_ptr<Skeleton> pSkeleton = std::make_shared<Skeleton>();
		unsigned int uBoneCount = mesh->mNumBones;
		for (unsigned int j = 0; j < uBoneCount; j++)
		{
			Joint joint{};
			aiBone* bone = mesh->mBones[j];
			unsigned int uWeightCount = bone->mNumWeights;
			unsigned int uBoneId = 0;

			for (unsigned int k = 0; k < uWeightCount; k++)
			{
				aiVertexWeight weight = bone->mWeights[k];
				float weightValue = weight.mWeight;

				SkinnedVertex& vertex = pVertices[weight.mVertexId];

				vertex.JointWeights = Vector3(weightValue, weightValue, weightValue);
				for (int boneIndex = 0; boneIndex < 4; boneIndex++)
				{
					vertex.JointIndices[boneIndex] = uBoneId;
				}
			}
		}

		// Creating the mesh.
		entity.Mesh = std::make_shared<AnimatedMesh>(
			pVertices,
			uVertexCount,
			pIndices,
			uIndexCount);
	}

	// Test code:
	// - - - - - - - - - - - - - - 
	/*
	unsigned int uSkeletonCount = scene->mNumSkeletons;
	for (unsigned int i = 0; i < uSkeletonCount; i++)
	{
		Skeleton skeleton{};
		unsigned int uBoneCount = scene->mSkeletons[i]->mNumBones;

		for (unsigned int j = 0; j < uBoneCount; j++)
		{
			aiSkeletonBone* assimpBone = scene->mSkeletons[i]->mBones[j];
			Joint joint{};
			joint.ParentIndex = assimpBone->mParent;
			joint.InvBindPose = Utils::ConvertFromAssimpMatrix(assimpBone->mLocalMatrix);

			skeleton.AddJoint(joint);
		}
	}
	*/
}