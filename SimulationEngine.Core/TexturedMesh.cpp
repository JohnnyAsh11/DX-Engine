#include "TexturedMesh.h"
#include "Utils.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "TinyObjLoader/tiny_obj_loader.h"

// Temp macro directory for testing this class.
#define TEXTURE_DIRECTORY L"../SimulationEngine.Assets/TexturedModels/"

//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//
//TexturedMesh::TexturedMesh(void)
//{
//	Assimp::Importer importer;
//
//	const aiScene* scene = importer.ReadFile("assets/model.obj",
//		aiProcess_Triangulate |
//		aiProcess_JoinIdenticalVertices |
//		aiProcess_CalcTangentSpace |
//		aiProcess_GenSmoothNormals |
//		aiProcess_ImproveCacheLocality);
//}

TexturedMesh::TexturedMesh(
	std::shared_ptr<Shader> a_pShader, 
	Microsoft::WRL::ComPtr<ID3D11SamplerState> a_pSampler,
	std::string a_sObjFile)
{
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

	for (auto& submesh : mMeshData)
	{
		VertexPack VertexPack{};
		IndexPack indexPack{};

		VertexPack.Vertices = &submesh.second.Vertices[0];
		VertexPack.VertexCount = (int)submesh.second.Vertices.size();

		indexPack.Indices = &submesh.second.Indices[0];
		indexPack.IndexCount = (int)submesh.second.Indices.size();

		std::shared_ptr<Mesh> pNewMesh = std::make_shared<Mesh>(VertexPack, indexPack, TangentType::Inverted);

		m_mSubMesh[pNewMesh] = submesh.second.Material;
	}

}

TexturedMesh::~TexturedMesh()
{
}

TexturedMesh& TexturedMesh::operator=(const TexturedMesh& a_Other)
{
	return *this;
}

TexturedMesh::TexturedMesh(const TexturedMesh& a_Other)
{
}

std::vector<std::shared_ptr<Entity>> TexturedMesh::ToEntity()
{
	std::vector<std::shared_ptr<Entity>> entities;

	for (auto& matMeshPair : m_mSubMesh)
	{
		entities.push_back(std::make_shared<Entity>(matMeshPair.first, matMeshPair.second));
	}

	return entities;
}
