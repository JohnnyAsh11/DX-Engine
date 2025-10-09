#include "LineManager.h"

LineManager::LineManager()
{
	m_pShader = std::make_shared<Shader>(
		L"LineVertexShader.cso",
		L"LinePixelShader.cso",
		ShaderTopology::LineList
	);
	m_pCBufferMapper = std::make_shared<CBufferMapper<LineCBufferData>>(LINE_VERTEX_CBUFFER);
}

LineManager::~LineManager()
{
	m_pCBufferMapper.reset();
	m_pShader.reset();
	m_lOutliners.clear();
}

LineManager& LineManager::operator=(const LineManager& a_Other)
{
	m_pCBufferMapper.reset();
	m_pShader.reset();
	m_lOutliners.clear();

	m_pShader = a_Other.m_pShader;
	m_lOutliners = a_Other.m_lOutliners;
	m_pCBufferMapper = a_Other.m_pCBufferMapper;

	return *this;
}

LineManager::LineManager(const LineManager& a_Other)
{
	m_pCBufferMapper.reset();
	m_pShader.reset();
	m_lOutliners.clear();

	m_pShader = a_Other.m_pShader;
	m_lOutliners = a_Other.m_lOutliners;
	m_pCBufferMapper = a_Other.m_pCBufferMapper;
}

void LineManager::AddOutliner(std::shared_ptr<Outliner> a_pOutliner)
{
	// Ensuring that the outliner has its buffers compiled.
	if (!a_pOutliner->IsCompiled())
	{
		a_pOutliner->CompileBuffers();
	}

	m_lOutliners.push_back(a_pOutliner);
}

void LineManager::Draw(std::shared_ptr<Camera> a_pCamera, Vector4 a_v4Color)
{
	// Setting the line shader.
	m_pShader->SetShader();
	for (unsigned int i = 0; i < m_lOutliners.size(); i++)
	{
		// Setting the constant buffer values.
		LineCBufferData cbuffer{};
		cbuffer.World = m_lOutliners[i]->GetTransform()->GetWorld();
		cbuffer.View = a_pCamera->GetView();
		cbuffer.Projection = a_pCamera->GetProjection();
		cbuffer.Color = a_v4Color;
		m_pCBufferMapper->MapBufferData(cbuffer);

		// Rendering the lines.
		m_lOutliners[i]->Draw();
	}
}

std::vector<std::shared_ptr<Outliner>> LineManager::GetOutliners(void)
{
	return m_lOutliners;
}
