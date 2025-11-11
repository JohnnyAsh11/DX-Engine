#include "Entity.h"
#include "SimulationUtils.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "TinyObjLoader/tiny_obj_loader.h"

// Temp macro directory for testing this class.
#define TEXTURE_DIRECTORY L"../SimulationEngine.Assets/TexturedModels/"

Entity::Entity(std::shared_ptr<Mesh> a_pMesh, std::shared_ptr<Material> a_pMaterial)
{
	m_mSubEntities.insert({a_pMesh, a_pMaterial});
	m_pTransform = std::make_shared<Transform>();
}

Entity::Entity(std::map<std::shared_ptr<Mesh>, std::shared_ptr<Material>> a_mSubEntities)
{
	m_mSubEntities = a_mSubEntities;
	m_pTransform = std::make_shared<Transform>();
}

Entity::Entity(std::shared_ptr<Shader> a_pShader, Microsoft::WRL::ComPtr<ID3D11SamplerState> a_pSampler, std::string a_sObjFile)
{
	m_pTransform = std::make_shared<Transform>();

	// Parsing the obj/mtl files.
	tinyobj::ObjReaderConfig config{};
	tinyobj::ObjReader reader = tinyobj::ObjReader();
	std::string fullpath = a_sObjFile.c_str();
	reader.ParseFromFile(fullpath, config);

	// Converting all of the material data to my Material class.
	std::vector<std::shared_ptr<Material>> lMeshMaterials;
	for (const auto& mat : reader.GetMaterials())
	{
		// Saving the diffuse values as a color tint.
		Vector4 colorTint(
			mat.diffuse[0],
			mat.diffuse[1],
			mat.diffuse[2],
			1.0f * (1.0f - mat.dissolve)
		);

		// Saving the roughness from the loaded shininess value.
		float roughness = 1.0f - min(mat.shininess / 1000.0f, 1.0f);

		// Instantiating a blank material.
		std::shared_ptr<Material> material = std::make_shared<Material>(a_pShader, colorTint, roughness);

		// Saving specific textures if provided:
		// Albedo textures.
		if (!mat.diffuse_texname.empty())
		{
			std::wstring sTexturePath = Utils::SanitizeFileName(mat.diffuse_texname);
			ShaderResourcePtr albedoTex = Utils::LoadTexture(TEXTURE_DIRECTORY + sTexturePath);
			material->AddTexturesSRV(0, albedoTex);
		}
		// Normal textures.
		if (!mat.normal_texname.empty())
		{
			std::wstring sTexturePath = Utils::SanitizeFileName(mat.normal_texname);
			ShaderResourcePtr normalTex = Utils::LoadTexture(TEXTURE_DIRECTORY + sTexturePath);
			material->AddTexturesSRV(1, normalTex);
		}
		// Roughness textures.
		if (!mat.roughness_texname.empty())
		{
			std::wstring sTexturePath = Utils::SanitizeFileName(mat.roughness_texname);
			ShaderResourcePtr roughTex = Utils::LoadTexture(TEXTURE_DIRECTORY + sTexturePath);
			material->AddTexturesSRV(2, roughTex);
		}
		// Metallic textures.
		if (!mat.metallic_texname.empty())
		{
			std::wstring sTexturePath = Utils::SanitizeFileName(mat.metallic_texname);
			ShaderResourcePtr metalTex = Utils::LoadTexture(TEXTURE_DIRECTORY + sTexturePath);
			material->AddTexturesSRV(3, metalTex);
		}

		// Adding the base sampler to the material.
		material->AddSampler(0, a_pSampler);

		// Adding the new material to this Mesh's collection of materials.
		lMeshMaterials.push_back(material);
	}

	// Collecting all of the submesh data together.
	std::map<int, SubMesh> mMeshData;
	const tinyobj::attrib_t& attrib = reader.GetAttrib();
	const std::vector<tinyobj::shape_t>& lShapes = reader.GetShapes();

	// For every shape in the loaded collection:
	for (const auto& shape : lShapes)
	{
		int nIndexOffset = 0;

		// For every face in the loaded collection:
		for (int f = 0; f < shape.mesh.num_face_vertices.size(); f++)
		{
			int nMaterialId = shape.mesh.material_ids[f];

			// If this material hasn't been seen before, create a new entry with this material.
			if (mMeshData.find(nMaterialId) == mMeshData.end())
			{
				SubMesh subset;
				subset.Material = lMeshMaterials[nMaterialId];
				mMeshData[nMaterialId] = subset;
			}

			unsigned int uVertexFaceCount = shape.mesh.num_face_vertices[f];
			SubMesh& subset = mMeshData[nMaterialId];

			// For each vertex in the face:
			for (int v = uVertexFaceCount - 1; v >= 0; v--)
			{
				Vertex vertex{};
				tinyobj::index_t index = shape.mesh.indices[nIndexOffset + v];

				// Vertex Positions:
				if (index.vertex_index >= 0)
				{
					vertex.Position.x = attrib.vertices[3 * index.vertex_index + 0];
					vertex.Position.y = attrib.vertices[3 * index.vertex_index + 1];
					vertex.Position.z = attrib.vertices[3 * index.vertex_index + 2] * -1.0f;
				}

				// Vertex Normals:
				if (index.normal_index >= 0)
				{
					vertex.Normal.x = attrib.normals[3 * index.normal_index + 0];
					vertex.Normal.y = attrib.normals[3 * index.normal_index + 1];
					vertex.Normal.z = attrib.normals[3 * index.normal_index + 2] * -1.0f;
				}

				// Vertex UVs:
				if (index.texcoord_index >= 0)
				{
					vertex.UV.x = attrib.texcoords[2 * index.texcoord_index + 0];
					vertex.UV.y = 1.0f - attrib.texcoords[2 * index.texcoord_index + 1];
				}

				// Tangents will be computed but the existing Mesh object.
				vertex.Tangent = Vector3{ 0.0f, 0.0f, 0.0f };

				// Adding the Vertex and setting the index.
				subset.Vertices.push_back(vertex);
				subset.Indices.push_back((unsigned int)subset.Vertices.size() - 1);
			}

			nIndexOffset += uVertexFaceCount;
		}
	}

	// Creating the actual Meshes and their DirectX buffer objects.
	for (auto& submesh : mMeshData)
	{
		VertexPack VertexPack{};
		IndexPack indexPack{};

		// Initializing the Vertex and Index packs.
		VertexPack.Vertices = &submesh.second.Vertices[0];
		VertexPack.VertexCount = (int)submesh.second.Vertices.size();
		indexPack.Indices = &submesh.second.Indices[0];
		indexPack.IndexCount = (int)submesh.second.Indices.size();

		// Creating the mesh with inverted tangents.
		std::shared_ptr<Mesh> pNewMesh = std::make_shared<Mesh>(VertexPack, indexPack, TangentType::Inverted);

		// Assigning the Mesh to its correct material.1
		m_mSubEntities[pNewMesh] = submesh.second.Material;
	}
}

Entity::~Entity()
{
	m_pTransform.reset();
}

Entity& Entity::operator=(const Entity& a_eOther)
{
	m_pTransform = a_eOther.m_pTransform;
	m_mSubEntities = a_eOther.m_mSubEntities;

	return *this;
}

Entity::Entity(const Entity& a_eOther)
{
	m_pTransform = a_eOther.m_pTransform;
	m_mSubEntities = a_eOther.m_mSubEntities;
}

std::shared_ptr<Transform> Entity::GetTransform(void)
{
	return m_pTransform;
}

std::vector<std::shared_ptr<Mesh>> Entity::GetMeshes(void)
{
	std::vector<std::shared_ptr<Mesh>> lMesh;
	for (auto& subMesh : m_mSubEntities)
	{
		lMesh.push_back(subMesh.first);
	}

	return lMesh;
}

std::vector<std::shared_ptr<Material>> Entity::GetMaterials(void)
{
	std::vector<std::shared_ptr<Material>> lMaterial;
	for (auto& subMesh : m_mSubEntities)
	{
		lMaterial.push_back(subMesh.second);
	}

	return lMaterial;
}

void Entity::Draw(
	std::shared_ptr<CBufferMapper<VertexCBufferData>> a_pVertexCBufferMapper,
	std::shared_ptr<CBufferMapper<MaterialCBufferData>> a_pPixelCBufferMapper,
	std::shared_ptr<Camera> a_pCamera,
	Light a_Lights[MAX_LIGHT_COUNT])
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
		submesh.second->PrepMaterialForDraw(
			a_pPixelCBufferMapper,
			a_pCamera->GetTransform().GetPosition(),
			a_Lights
		);

		// Rendering the mesh.
		submesh.first->Draw();
	}
}
