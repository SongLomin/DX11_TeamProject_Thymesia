#include "stdafx.h"
#include "Imgui_Manager.h"

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "GameInstance.h"

#include "Window_UI.h"
#include "Window_PrototypeView.h"
#include "Window_HierarchyView.h"
#include "Window_ComponentView.h"


//Effect
#include "Window_EffectResourceView.h"
#include "Window_EffectEditerView.h"
#include "Window_AnimationPlayerView.h"
#include "Window_AnimationModelView.h"
#include "Window_EffectHierarchyView.h"

//Editer Camera
#include "Camera_Free.h"
#include "Terrain.h"

IMPLEMENT_SINGLETON(CImGui_Manager)

HRESULT CImGui_Manager::Initialize()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows


	// Setup Dear ImGui style
	ImGui::StyleColorsLight();
	
	//ImGui::StyleColorsClassic();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(DEVICE, DEVICECONTEXT);

	Init_SceneEditer();
	

	return S_OK;
}

void CImGui_Manager::Tick(_float fTimeDelta)
{
	for (auto& elem : m_arrWindows)
	{
		if(elem->Get_Enable())
			elem->Tick(fTimeDelta);
	}
}

HRESULT CImGui_Manager::Render(void)
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();


	ImGui::NewFrame();

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;
	if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::SetNextWindowSize(ImVec2(g_iWinCX, g_iWinCY), ImGuiCond_Always);
	ImGui::Begin("Dock", nullptr, window_flags);
	ImGui::PopStyleVar(2);
	ImGuiID dockspaceID = ImGui::GetID("DockSpace");
	ImGui::DockSpace(dockspaceID, ImVec2(0, 0), dockspaceFlags);
	ImGui::End();


	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				if (EDITER_TYPE::SCENE == m_eCurrentEditerType)
				{
					Write_Json(m_szJsonPath + m_szCurrentLocalPath);
				}

				if (EDITER_TYPE::EFFECT == m_eCurrentEditerType)
				{
					json DummuJson;

					GET_SINGLE(CWindow_EffectHierarchyView)->Write_Json(DummuJson);
					//GET_SINGLE(CWindow_EffectHierarchyView)->Write_FromBinary();
					GET_SINGLE(CWindow_EffectResourceView)->Load_EffectGroup();
				}
				
			}

			if (EDITER_TYPE::SCENE == m_eCurrentEditerType)
			{
				if (ImGui::MenuItem("Load"))
				{
					Load_FromJson(m_szJsonPath + m_szCurrentLocalPath);
					
				}
			}

			else if (EDITER_TYPE::EFFECT == m_eCurrentEditerType)
			{

			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Level"))
		{
			if (ImGui::MenuItem("-1. Loading"))
			{
				m_szCurrentLocalPath = "Loading.json";
				Load_FromJson(m_szJsonPath + m_szCurrentLocalPath);
			}

			if (ImGui::MenuItem("00. Logo"))
			{
				m_szCurrentLocalPath = "Logo.json";
				Load_FromJson(m_szJsonPath + m_szCurrentLocalPath);
			}

			if (ImGui::MenuItem("01. Lobby"))
			{
				m_szCurrentLocalPath = "Lobby.json";
				Load_FromJson(m_szJsonPath + m_szCurrentLocalPath);
			}

			if (ImGui::MenuItem("02. Stage1"))
			{
				m_szCurrentLocalPath = "Stage1.json";
				Load_FromJson(m_szJsonPath + m_szCurrentLocalPath);
			}

			if (ImGui::MenuItem("03. Stage2"))
			{
				m_szCurrentLocalPath = "Stage2.json";
				Load_FromJson(m_szJsonPath + m_szCurrentLocalPath);

				LIGHTDESC			LightDesc;
				LightDesc = GAMEINSTANCE->Get_LightDesc(0);

				LightDesc.eType = tagLightDesc::TYPE_DIRECTIONAL;
				LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
				LightDesc.vDiffuse = _float4(0.2f, 0.2f, 0.2f, 1.f);
				LightDesc.vAmbient = _float4(0.7f, 0.7f, 0.7f, 1.f);
				LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 0.1f);
				LightDesc.vLightFlag = _float4(1.f, 1.f, 1.f, 1.f);
				LightDesc.bEnable = true;

				GAMEINSTANCE->Set_LightDesc(LightDesc);
			}

			if (ImGui::MenuItem("04. Stage3"))
			{
				m_szCurrentLocalPath = "Stage3.json";
				Load_FromJson(m_szJsonPath + m_szCurrentLocalPath);
			}

			if (ImGui::MenuItem("99. Static"))
			{
				m_szCurrentLocalPath = "Static.json";
				Load_FromJson(m_szJsonPath + m_szCurrentLocalPath);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Editer"))
		{
			if (ImGui::MenuItem("Scene Editer"))
			{
				Init_SceneEditer();
			}

			if (ImGui::MenuItem("Effect Editer"))
			{
				Init_EffectEditer();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}


	for (_uint i = 0; i < m_arrWindows.size(); ++i)
	{
		if (m_arrWindows[i]->Is_Enable())
		{
			if (FAILED(m_arrWindows[i]->Render()))
				return E_FAIL;
		}
	}

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());



	return S_OK;
}

void CImGui_Manager::Init_SceneEditer()
{
	Release_CurrentEditer();

	m_arrWindows.emplace_back(CWindow_UI::Create_Instance());
	m_arrWindows.emplace_back(CWindow_PrototypeView::Create_Instance());
	m_arrWindows.emplace_back(CWindow_HierarchyView::Create_Instance());
	m_arrWindows.emplace_back(CWindow_ComponentView::Create_Instance());

	for (auto& elem : m_arrWindows)
	{
		elem->Initialize();
	}

	for (auto& elem : m_arrWindows)
	{
		elem->Start();
	}

	m_eCurrentEditerType = EDITER_TYPE::SCENE;

}

void CImGui_Manager::Init_EffectEditer()
{
	Release_CurrentEditer();

	m_arrWindows.emplace_back(CWindow_EffectResourceView::Create_Instance());
	m_arrWindows.emplace_back(CWindow_EffectEditerView::Create_Instance());
	m_arrWindows.emplace_back(CWindow_AnimationModelView::Create_Instance());
	m_arrWindows.emplace_back(CWindow_AnimationPlayerView::Create_Instance());
	m_arrWindows.emplace_back(CWindow_EffectHierarchyView::Create_Instance());

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));
	CameraDesc.vEye = _float4(0.0f, 4.f, 7.5f, 1.f);
	CameraDesc.vAt = _float4(0.f, 1.5f, 0.f, 1.f);
	CameraDesc.fFovy = XMConvertToRadians(65.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	m_pEditerCamera = GAMEINSTANCE->Add_GameObject<CCamera_Free>(LEVEL_EDIT, &CameraDesc);
	m_pTerrain = GAMEINSTANCE->Add_GameObject<CTerrain>(LEVEL_EDIT);
	m_pTerrain.lock()->Get_Component<CTransform>().lock()->Add_Position(XMVectorSet(-30.f, 0.f, -30.f, 1.f));

	for (auto& elem : m_arrWindows)
	{
		elem->Initialize();
	}

	for (auto& elem : m_arrWindows)
	{
		elem->Start();
	}

	m_eCurrentEditerType = EDITER_TYPE::EFFECT;
}

void CImGui_Manager::Release_CurrentEditer()
{
	for (_uint i = 0; i < m_arrWindows.size(); ++i)
	{
		m_arrWindows[i].reset();
	}

	m_arrWindows.clear();

	CWindow_UI::Destroy_Instance();
	CWindow_PrototypeView::Destroy_Instance();
	CWindow_HierarchyView::Destroy_Instance();
	CWindow_ComponentView::Destroy_Instance();

	CWindow_EffectResourceView::Destroy_Instance();
	CWindow_AnimationModelView::Destroy_Instance();
	CWindow_EffectEditerView::Destroy_Instance();
	CWindow_AnimationPlayerView::Destroy_Instance();
	CWindow_EffectHierarchyView::Destroy_Instance();

	if (m_pEditerCamera.lock())
	{
		m_pEditerCamera.lock()->Set_Dead();
		m_pEditerCamera = weak_ptr<CCamera_Free>();
	}

	if (m_pTerrain.lock())
	{
		m_pTerrain.lock()->Set_Dead();
		m_pTerrain = weak_ptr<CTerrain>();
	}
}

void CImGui_Manager::Write_Json(const string& In_szPath)
{
	json NewJson;

	for (_uint i = 0; i < m_arrWindows.size(); ++i)
	{
		m_arrWindows[i]->Write_Json(NewJson);
		
	}

	if (FAILED(CJson_Utility::Save_Json(In_szPath.c_str(), NewJson)))
		DEBUG_ASSERT;

}

void CImGui_Manager::Load_FromJson(const string& In_szPath)
{
	for (_uint i = 0; i < m_arrWindows.size(); ++i)
	{
		m_arrWindows[i]->OnLevelLoad();
	}

	//잘못된 경로
	if (FAILED(CJson_Utility::Load_Json(In_szPath.c_str(), m_CurrentLevelJson)))
	{
#ifdef _DEBUG
		cout << In_szPath << " : 해당 경로에 Json 파일이 없음." << endl;
#endif // _DEBUG

		return;
	}

	for (_uint i = 0; i < m_arrWindows.size(); ++i)
	{
		m_arrWindows[i]->Load_FromJson(m_CurrentLevelJson);
	}

}


void CImGui_Manager::Free()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	Release_CurrentEditer();
}
