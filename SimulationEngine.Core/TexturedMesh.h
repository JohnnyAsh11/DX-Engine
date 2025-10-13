#ifndef __TEXTUREDMESH_H_
#define __TEXTUREDMESH_H_

#include <vector>
#include <map>
#include <string>

#include "Mesh.h"
#include "Camera.h"
#include "Material.h"
#include "CBufferMapper.h"
#include "Entity.h"

struct SubMesh
{
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	std::shared_ptr<Material> Material;
};

class TexturedMesh
{
private:
	std::map<std::shared_ptr<Mesh>, std::shared_ptr<Material>> m_mSubMesh;

public:
	//TexturedMesh(void);

	TexturedMesh(std::shared_ptr<Shader> a_pShader, Microsoft::WRL::ComPtr<ID3D11SamplerState> a_pSampler, std::string a_sObjFile);

	~TexturedMesh();
	TexturedMesh& operator=(const TexturedMesh& a_Other);
	TexturedMesh(const TexturedMesh& a_Other);

	void Draw(
		std::shared_ptr<CBufferMapper<VertexCBufferData>> a_pVertexCBufferMapper,
		std::shared_ptr<CBufferMapper<MaterialCBufferData>> a_pPixelCBufferMapper,
		std::shared_ptr<Camera> a_pCamera,
		Light a_Lights[MAX_LIGHT_COUNT]);

	std::vector<std::shared_ptr<Entity>> ToEntity();
};

#endif //__TEXTUREDMESH_H_
