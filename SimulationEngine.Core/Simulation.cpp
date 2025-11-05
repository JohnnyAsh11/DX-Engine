#include "Simulation.h"

// Internal code.
#include "Graphics.h"
#include "Vectors.h"
#include "Application.h"
#include "Input.h"
#include "Transform.h"
#include "Logger.h"
#include "Utils.h"

// External code.
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

// C++ base libs.
#include <vector>
#include <WICTextureLoader.h>

#define MODEL_DIRECTORY "../SimulationEngine.Assets/Models/"
#define CUBE_FILE "cube.graphics_obj"
#define SPHERE_FILE "sphere.graphics_obj"
#define CYLINDER_FILE "cylinder.graphics_obj"

void Simulation::Init()
{
	int width = Application::GetInstance()->GetWidth();
	int height = Application::GetInstance()->GetHeight();
	float fAspectRatio = (float)width / height;
	m_pCamera = std::make_shared<Camera>(fAspectRatio, Vector3(0.0f, 0.0f, -5.0f), 45.0f);

	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	D3D11_SAMPLER_DESC sampleDesc{};
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampleDesc.MaxAnisotropy = 8;
	sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;
	sampleDesc.MipLODBias = 0;
	sampleDesc.MinLOD = 0;
	sampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	Graphics::GetDevice()->CreateSamplerState(&sampleDesc, &pSampler);

	m_pEntityManager = std::make_shared<EntityManager>();

	Light light{};
	light.Type = LIGHT_TYPE_DIRECTIONAL;
	light.Color = Vector3(1.0f, 1.0f, 1.0f);
	light.Range = 10.0f;
	light.Intensity = 10.0f;
	light.Position = Vector3(0.0f, 20.0f, 0.0f);
	light.Direction = Vector3(-1.0f, -1.0f, 0.0f);
	m_pEntityManager->AddLight(light, LightIndex::MainLight);
	light.Type = LIGHT_TYPE_POINT;
	light.Color = Vector3(1.0f, 1.0f, 1.0f);
	light.Range = 10.0f;
	light.Intensity = 2.0f;
	light.Position = Vector3(0.0f, -1.0f, 0.0f);
	m_pEntityManager->AddLight(light, LightIndex::Light1);


	m_pShader = std::make_shared<Shader>();
	std::shared_ptr<Material> mat = std::make_shared<Material>(
		m_pShader, 
		Vector4(0.0f, 0.0f, 0.0f, 1.0f),
		0.5f);

	m_pTestEntity = std::make_shared<AnimatedEntity>("../SimulationEngine.Assets/Advanced/safe.fbx", m_pShader, pSampler);
	/*m_pTestEntity->GetTransform()->Rotate(Vector3(
		DirectX::XMConvertToRadians(270.0f),
		DirectX::XMConvertToRadians(180.0f), 
		DirectX::XMConvertToRadians(0.0f)));*/
	m_pTestEntity->GetTransform()->Scale(Vector3(0.02f, 0.02f, 0.02f));

	TextureSet cobblestone = Utils::LoadTextureSet(L"cobblestone");
	mat->AddTexturesSRV(0, cobblestone.Albedo);		// Albedo
	mat->AddTexturesSRV(1, cobblestone.Normal);		// Normals
	mat->AddTexturesSRV(2, cobblestone.Roughness);	// Roughness
	mat->AddTexturesSRV(3, cobblestone.Metal);		// Metal
	mat->AddSampler(0, pSampler);					// Sampler

	std::shared_ptr<Mesh> sphere = std::make_shared<Mesh>(MODEL_DIRECTORY, SPHERE_FILE);
	std::shared_ptr<Mesh> cylinder = std::make_shared<Mesh>(MODEL_DIRECTORY, CYLINDER_FILE);
	std::shared_ptr<Mesh> cube = std::make_shared<Mesh>(MODEL_DIRECTORY, CUBE_FILE);
	std::shared_ptr<Mesh> helix = std::make_shared<Mesh>("../SimulationEngine.Assets/Models/", "helix.graphics_obj");

	//std::shared_ptr<Entity> pCar = std::make_shared<Entity>(m_pShader, pSampler, "../SimulationEngine.Assets/TexturedModels/mcl35m_2.graphics_obj");

	m_pEntityManager->AddEntity(sphere, mat);
	m_pEntityManager->AddEntity(cylinder, mat);
	m_pEntityManager->AddEntity(cube, mat);
	m_pEntityManager->AddEntity(helix, mat);

	//m_pEntityManager->AddEntity(pCar);

	EntityPtrCollection& entities = m_pEntityManager->GetEntities();
	for (UINT i = 0; i < entities.size(); i++)
	{
		std::shared_ptr<Transform> t = entities[i]->GetTransform();
		float fXPos = static_cast<float>(i) * 3.0f - 4.5f;

		t->Scale(Vector3(0.25f, 0.25f, 0.25f));
		t->SetPosition(Vector3(fXPos, 0.0f, 0.0f));
	}

	m_pSky = std::make_shared<Sky>(cube, pSampler);
	m_pSky->CreateCubemap(
		L"../SimulationEngine.Assets/Textures/Skies/right.png",
		L"../SimulationEngine.Assets/Textures/Skies/left.png",
		L"../SimulationEngine.Assets/Textures/Skies/up.png",
		L"../SimulationEngine.Assets/Textures/Skies/down.png",
		L"../SimulationEngine.Assets/Textures/Skies/front.png",
		L"../SimulationEngine.Assets/Textures/Skies/back.png"
	);

#if defined(DEBUG) | defined(_DEBUG)
	// Initialization of ImGui.
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(Application::GetInstance()->GetHandle());
	ImGui_ImplDX11_Init(Graphics::GetDevice().Get(), Graphics::GetContext().Get());
	ImGui::StyleColorsDark();
	Graphics::GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
#endif
}

void Simulation::Update(float a_fDeltaTime)
{
	m_pCamera->UpdateMovement(a_fDeltaTime);

	EntityPtrCollection entities = m_pEntityManager->GetEntities();
	for (UINT i = 0; i < entities.size(); i++)
	{
		std::shared_ptr<Transform> t = entities[i]->GetTransform();
		//t->Rotate(Vector3(0.0f, 0.5f * a_fDeltaTime, 0.0f));
	}

#if defined(DEBUG) | defined(_DEBUG)
	std::map<std::shared_ptr<Mesh>, std::shared_ptr<Outliner>> mOutliners = LineManager::GetInstance()->GetOutliners();
	int i = 0;
	for (const auto& kvp : mOutliners)
	{
		kvp.second->SetTransform(*entities[i]->GetTransform());
		i++;
	}
#endif

	if (Input::KeyDown(VK_ESCAPE))
	{
		Application::GetInstance()->Quit();
	}
}

void Simulation::Draw(float a_fDeltaTime)
{
	float background[4] = { 0.1f, 0.2f, 0.5f, 0.0f };
	// Clearing the screen.
	Graphics::GetContext()->ClearRenderTargetView(
		Graphics::GetBackBufferRTV().Get(),
		background);
	Graphics::GetContext()->ClearDepthStencilView(
		Graphics::GetDepthBufferDSV().Get(),
		D3D11_CLEAR_DEPTH, 1.0f, 0);

	Matrix4 m4View = m_pCamera->GetView();
	Matrix4 m4Proj = m_pCamera->GetProjection();

	// Setting the shader and rendering the entities.
	m_pShader->SetShader();
	//m_pEntityManager->Draw(m_pCamera);

	m_pTestEntity->Draw(
		m_pEntityManager->GetVertexCBufferMapper(),
		m_pEntityManager->GetPixelCBufferMapper(),
		m_pCamera,
		*m_pEntityManager->GetLights());

	if (m_bDebugRendering)
	{
		LineManager::GetInstance()->Draw(m_pCamera, Vector4(0.0f, 1.0f, 1.0f, 1.0f));
	}

	// Rendering the skybox last since last is slightly more efficient.
	m_pSky->Draw(m4View, m4Proj);

	// Rendering ImGui.
#if defined(DEBUG) | defined(_DEBUG)
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif

	// Present at the end of the frame
	bool vsync = Graphics::VsyncState();
	Graphics::GetSwapChain()->Present(
		vsync ? TRUE : FALSE,
		vsync ? FALSE : DXGI_PRESENT_ALLOW_TEARING);

	// Re-bind back buffer and depth buffer after presenting
	Graphics::GetContext()->OMSetRenderTargets(
		1,
		Graphics::GetBackBufferRTV().GetAddressOf(),
		Graphics::GetDepthBufferDSV().Get());
}

void Simulation::UpdateImGui(float a_fDeltaTime)
{
	// Providing new data to ImGui.
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = a_fDeltaTime;
	io.DisplaySize.x = (float)Application::GetInstance()->GetWidth();
	io.DisplaySize.y = (float)Application::GetInstance()->GetHeight();
	io.MousePos = ImVec2((float)Input::GetMouseX(), (float)Input::GetMouseY());
	io.MouseDown[0] = Input::MouseLeftDown();
	io.MouseDown[1] = Input::MouseRightDown();
	io.MouseWheel = Input::GetMouseWheel();

	// Resetting the frame.
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Setting the input capture variables.
	Input::SetKeyboardCapture(false);
	Input::SetMouseCapture(false);

	// Setting what is inside of the Gui.
	ImGui::Begin("Application Settings");

	// Only setting the framerate view for the UI every second.
	static float fTimer = 1.0f;
	fTimer -= a_fDeltaTime;
	if (fTimer <= 0.0f)
	{
		m_fUIFramerate = ImGui::GetIO().Framerate;
		fTimer = 1.0f;
	}

	ImGui::Text("Framerate: %f fps", m_fUIFramerate);
	// Framerate text in the UI:
	ImGui::Text("Delta Time: %f", a_fDeltaTime);

	if (ImGui::TreeNode("Debug"))
	{
		if (ImGui::Button("Enable Debug Rendering"))
		{
			m_bDebugRendering = !m_bDebugRendering;
		}

		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Entities"))
	{
		EntityPtrCollection entities = m_pEntityManager->GetEntities();
		// Looping through the entities in the list.s
		for (unsigned int i = 0; i < entities.size(); i++)
		{
			// Specific interface naming for ImGui.
			std::string sNum = std::to_string(i);
			std::string sInterface = "Entity " + sNum + "##";
			sInterface += sNum;

			// Creating the nodes for the individual entities.
			if (ImGui::TreeNode(sInterface.c_str()))
			{
				std::shared_ptr<Transform> current = entities[i]->GetTransform();

				// Creating the drag floats.
				ImGui::DragFloat3(
					("Position##" + sNum).c_str(),
					&current->GetPosition().x,
					0.025f);
				ImGui::DragFloat3(
					("Rotation##" + sNum).c_str(),
					&current->GetRotation().x,
					0.025f);
				ImGui::DragFloat3(
					("Scale##" + sNum).c_str(),
					&current->GetScale().x,
					0.025f);
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Lights"))
	{
		Light* lights = m_pEntityManager->GetLights();

		for (unsigned int i = 0; i < MAX_LIGHT_COUNT; i++)
		{
			// Ignoring undeclared lights.
			if (lights[i].Type == LIGHT_TYPE_NONE)
			{
				continue;
			}

			std::string sNum = std::to_string(i);
			std::string sInterface = "Light " + sNum + "##";
			sInterface += sNum;

			if (ImGui::TreeNode(sInterface.c_str()))
			{
				ImGui::DragFloat3(
					("Position##" + sNum).c_str(), 
					&lights[i].Position.x, 
					0.025f);
				ImGui::DragFloat3(
					("Color##" + sNum).c_str(), 
					&lights[i].Color.x, 
					0.025f);
				ImGui::DragFloat(
					("Intensity##" + sNum).c_str(), 
					&lights[i].Intensity, 
					0.025f);
				ImGui::DragFloat(
					("Range##" + sNum).c_str(), 
					&lights[i].Range, 
					0.025f);
				
				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}

	// Closing the sub window.
	ImGui::End();
}

void Simulation::OnResize()
{
	if (m_pCamera != nullptr) 
	{
		int width = Application::GetInstance()->GetWidth();
		int height = Application::GetInstance()->GetHeight();
		float fAspectRatio = (float)width / height;
		m_pCamera->UpdateProjection(fAspectRatio);
	}
}

Simulation::~Simulation()
{
	LineManager::Release();

#if defined(DEBUG) | defined(_DEBUG)
	// ImGui clean up
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif
}