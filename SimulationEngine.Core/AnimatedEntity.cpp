#include "AnimatedEntity.h"
#include "Utils.h"

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

	unsigned int uMeshCount = scene->mNumMeshes;
	for (unsigned int i = 0; i < uMeshCount; i++)
	{
		SubEntity entity{};
		aiMesh* mesh = scene->mMeshes[i];

		int uVertexCount = mesh->mNumVertices;
		int uIndexCount = uVertexCount;
		SkinnedVertex* pVertices = new SkinnedVertex[uVertexCount];
		unsigned int* pIndices = new unsigned int[uIndexCount];
		for (unsigned int j = 0; j < uIndexCount; j++)
		{
			pIndices[j] = j;
		}

		for (unsigned int j = 0; j < uVertexCount; j++)
		{
			SkinnedVertex vertex{};

			aiVector3D position = mesh->mVertices[j];
			aiVector3D normal = mesh->mNormals[j];
			aiVector3D tangent = mesh->mTangents[j];
			aiVector3D* UV = mesh->mTextureCoords[j];
			aiVector3D* UV = mesh->[j];

			vertex.Position.x = position.x;
			vertex.Position.y = position.y;
			vertex.Position.z = position.z;

			vertex.Normal.x = normal.x;
			vertex.Normal.y = normal.y;
			vertex.Normal.z = normal.z;

			vertex.Tangent.x = tangent.x;
			vertex.Tangent.y = tangent.y;
			vertex.Tangent.z = tangent.z;

			vertex.UV.x = UV->x;
			vertex.UV.y = UV->y;

			pVertices[j] = vertex;
		}


	}

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
}