#include "AnimatedEntity.h"
#include "Utils.h"

#include <queue>

AnimatedEntity::AnimatedEntity(
	std::string a_sFbxFile, 
	std::shared_ptr<Shader> a_pShader, 
	Microsoft::WRL::ComPtr<ID3D11SamplerState> a_pSampler)
{
	m_pTransform = std::make_shared<Transform>();
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(a_sFbxFile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate);

	ProcessAssimpScene(scene, a_pShader, a_pSampler);
}

AnimatedEntity::~AnimatedEntity(void)
{
	m_pRootSkeleton.reset();
	m_pTransform.reset();
	m_mSubEntities.clear();
}

AnimatedEntity& AnimatedEntity::operator=(const AnimatedEntity& a_Other)
{
	m_pRootSkeleton = a_Other.m_pRootSkeleton;
	m_mSubEntities = a_Other.m_mSubEntities;
	m_pTransform = a_Other.m_pTransform;

	return *this;
}

AnimatedEntity::AnimatedEntity(const AnimatedEntity& a_Other)
{
	m_pRootSkeleton = a_Other.m_pRootSkeleton;
	m_mSubEntities = a_Other.m_mSubEntities;
	m_pTransform = a_Other.m_pTransform;
}

void AnimatedEntity::Draw(
	std::shared_ptr<CBufferMapper<VertexCBufferData>> a_pVertexCBufferMapper, 
	std::shared_ptr<CBufferMapper<MaterialCBufferData>> a_pPixelCBufferMapper,
	std::shared_ptr<Camera> a_pCamera,
	Light a_Lights)
{
	// Setting constant buffer data.
	VertexCBufferData cbuffer{};
	cbuffer.World = m_pTransform->GetWorld();
	cbuffer.WorldInvTranspose = m_pTransform->GetWorldInvTra();
	cbuffer.View = a_pCamera->GetView();
	cbuffer.Projection = a_pCamera->GetProjection();

	// Sending constant buffer data to GPU.
	a_pVertexCBufferMapper->MapBufferData(cbuffer);

	for (auto& submesh : m_mSubEntities)
	{
		// Applying materials.
		submesh.first->PrepMaterialForDraw(
			a_pPixelCBufferMapper, 
			a_pCamera->GetTransform().GetPosition(), 
			&a_Lights
		);

		// Rendering the mesh.
		submesh.second->Draw();
	}
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
	// TODO: separate each individual loading process in this method into smaller helper methods.
	// ------------------------------------------------------------------------------
	// Skeleton loading: 
	unsigned int uCounter = 0;
	m_pRootSkeleton = std::make_shared<Skeleton>();
	std::queue<LoadedBone> bones;

	// Setting the starting (root) node.
	LoadedBone current{};
	current.Bone = scene->mRootNode;
	current.ParentIndex = -1;
	bones.push(current);

	// Performing a breadth-first traversal through the bone tree.
	while (bones.size() > 0)
	{
		// Dequeueing the first element.
		current = bones.front();
		bones.pop();

		// Creating a joint and adding it to the skeleton.
		Joint joint{};
		joint.Name = current.Bone->mName.C_Str();
		joint.ParentIndex = current.ParentIndex;
		joint.InvBindPose = Utils::ConvertFromAssimpMatrix(current.Bone->mTransformation);
		m_pRootSkeleton->AddJoint(joint);

		// Pushing the child bones to the queue.
		for (int i = 0; i < current.Bone->mNumChildren; i++)
		{
			LoadedBone child{};
			child.Bone = current.Bone->mChildren[i];
			child.ParentIndex = uCounter;
			bones.push(child);
		}
		uCounter++;
	}

	// ------------------------------------------------------------------------------
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

	// ------------------------------------------------------------------------------
	// Vertex/Mesh loading:
	unsigned int uMeshCount = scene->mNumMeshes;
	for (unsigned int i = 0; i < uMeshCount; i++)
	{
		std::shared_ptr<AnimatedMesh> pMesh;
		aiMesh* mesh = scene->mMeshes[i];
		unsigned int matIndex = mesh->mMaterialIndex;

		// Vertex Count.
		int uVertexCount = mesh->mNumVertices;
		// Index Count.
		int uIndexCount = uVertexCount;
		// Vertices.
		SkinnedVertex* pVertices = new SkinnedVertex[uVertexCount];
		// Indices.  Always uses trianglese so num of faces times three.
		unsigned int* pIndices = new unsigned int[mesh->mNumFaces * 3];

		// Populating the Index array.
		int nTotalIndices = 0;
		for (int j = 0; j < mesh->mNumFaces; j++)
		{
			const aiFace& face = mesh->mFaces[j];
			for (int k = 0; k < face.mNumIndices; k++)
			{
				pIndices[nTotalIndices] = face.mIndices[k];
				nTotalIndices++;
			}
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

		// Putting together the mini bone structure for this mesh.
		unsigned int uBoneCount = mesh->mNumBones;
		for (unsigned int j = 0; j < uBoneCount; j++)
		{
			aiBone* bone = mesh->mBones[j];
			unsigned int uWeightCount = bone->mNumWeights;
			unsigned int uBoneIdx = 0;

			// Getting the current bones index in the skeleton.
			Joint* joints = m_pRootSkeleton->GetJoints();
			for (unsigned int i = 0; i < m_pRootSkeleton->GetJointCount(); i++)
			{
				if (strcmp(joints[i].Name.c_str(), bone->mName.C_Str()) == 0)
				{
					uBoneIdx = i;
				}
			}
			
			for (unsigned int k = 0; k < uWeightCount; k++)
			{
				// Extracting the weight and actual weight values.
				aiVertexWeight weight = bone->mWeights[k];
				float weightValue = weight.mWeight;

				SkinnedVertex& vertex = pVertices[weight.mVertexId];

				// Setting the data in that vertex,
				vertex.JointWeights = Vector3(weightValue, weightValue, weightValue);
				for (int boneIndex = 0; boneIndex < MAX_JOINT_COUNT; boneIndex++)
				{
					// Only setting indices that have not been used yet.
					if (vertex.JointIndices[boneIndex] == -1)
					{
						vertex.JointIndices[boneIndex] = uBoneIdx;
						break;
					}
				}
			}
		}

		// Creating the mesh.
		pMesh = std::make_shared<AnimatedMesh>(
			pVertices,
			uVertexCount,
			pIndices,
			uIndexCount);

		m_mSubEntities.insert({ modelMats[matIndex], pMesh });
	}
}