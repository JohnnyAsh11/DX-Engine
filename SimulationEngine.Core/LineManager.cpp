#include "LineManager.h"

LineManager* LineManager::m_pInstance = nullptr;

LineManager::LineManager()
{
	m_pShader = std::make_shared<Shader>(
		L"LineVS.cso",
		L"LinePS.cso",
		ShaderTopology::LineList
	);
	m_pCBufferMapper = std::make_shared<CBufferMapper<LineCBufferData>>(LINE_VERTEX_CBUFFER);
}

LineManager::~LineManager()
{
	m_pCBufferMapper.reset();
	m_pShader.reset();
	m_mOutliners.clear();
}

LineManager* LineManager::GetInstance(void)
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new LineManager();
	}

	return m_pInstance;
}

void LineManager::Release(void)
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
	}
}

LineManager& LineManager::operator=(const LineManager& a_Other)
{
	m_pCBufferMapper.reset();
	m_pShader.reset();
	m_mOutliners.clear();

	m_pShader = a_Other.m_pShader;
	m_mOutliners = a_Other.m_mOutliners;
	m_pCBufferMapper = a_Other.m_pCBufferMapper;

	return *this;
}

LineManager::LineManager(const LineManager& a_Other)
{
	m_pCBufferMapper.reset();
	m_pShader.reset();
	m_mOutliners.clear();

	m_pShader = a_Other.m_pShader;
	m_mOutliners = a_Other.m_mOutliners;
	m_pCBufferMapper = a_Other.m_pCBufferMapper;
}

void LineManager::AddOutliner(std::shared_ptr<Mesh> a_pMesh, std::shared_ptr<Outliner> a_pOutliner)
{
	// Ensuring that the outliner has its buffers compiled.
	if (!a_pOutliner->IsCompiled())
	{
		a_pOutliner->CompileBuffers();
	}

	m_mOutliners.insert({ a_pMesh, a_pOutliner });
}

void LineManager::Draw(std::shared_ptr<Camera> a_pCamera, Vector4 a_v4Color)
{
	// Setting the line shader.
	m_pShader->SetShader(); 
	for (const auto& kvp : m_mOutliners)
	{
		// Setting the constant buffer values.
		LineCBufferData cbuffer{};
		cbuffer.World = kvp.second->GetTransform()->GetWorld();
		cbuffer.View = a_pCamera->GetView();
		cbuffer.Projection = a_pCamera->GetProjection();
		cbuffer.Color = a_v4Color;
		m_pCBufferMapper->MapBufferData(cbuffer);

		// Rendering the lines.
		kvp.second->Draw();
	}
}

std::map<std::shared_ptr<Mesh>, std::shared_ptr<Outliner>> LineManager::GetOutliners(void)
{
	return m_mOutliners;
}
