#include "stdafx.h"
#include "Imgui_Manager.h"
#include <shobjidl.h> 

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

//Model
#include "Window_ModelContantView.h"
#include "Window_TextureEditerView.h"

//Editer Camera
#include "Camera_Free.h"
#include "Terrain.h"
#include "PipeLine.h"

#include "Transform.h"

#include "Client_GameObjects.h"
#include "GameManager.h"

IMPLEMENT_SINGLETON(CImGui_Manager)

HRESULT CImGui_Manager::Initialize()
{
	// Setup Dear ImGui context
	
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

	if (KEY_INPUT(KEY::DELETEKEY, KEY_STATE::TAP))
	{
		Toggle_PhysXInfo();
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
					this->Save_EffectJson();
				}

				if (EDITER_TYPE::MODEL == m_eCurrentEditerType)
				{
					GET_SINGLE(CWindow_ModelContantView)->Save_Model();
				}
				
			}

			if (EDITER_TYPE::SCENE == m_eCurrentEditerType)
			{
				/*if (ImGui::MenuItem("Load"))
				{
					Load_FromJson(m_szJsonPath + m_szCurrentLocalPath);
					
				}*/
			}

			else if (EDITER_TYPE::EFFECT == m_eCurrentEditerType)
			{

			}

			if (EDITER_TYPE::EFFECT == m_eCurrentEditerType)
			{
				if (ImGui::MenuItem("Load All & Save All Effects"))
				{
					vector<string> AllEffects = GET_SINGLE(CWindow_EffectResourceView)->Get_AllEffectGroupNames();
					for (auto& elem : AllEffects)
					{
						GET_SINGLE(CWindow_EffectHierarchyView)->Call_LoadEffectGroup(elem.c_str());
						this->Save_EffectJson();
					}
				}
			}
			
			if (EDITER_TYPE::MODEL == m_eCurrentEditerType)
			{
				if (ImGui::MenuItem("Load Anim Model"))
				{
					GET_SINGLE(CWindow_ModelContantView)->Open_ModelFile(MODEL_TYPE::ANIM);
				}

				if (ImGui::MenuItem("Load Model"))
				{
					GET_SINGLE(CWindow_ModelContantView)->Open_ModelFile(MODEL_TYPE::NONANIM);
				}

				
			}


			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("PhysX_Collider", "PgUp")) 
			{
				Toggle_PhysXInfo();


			}
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
			}

			if (ImGui::MenuItem("04. Stage3"))
			{
				m_szCurrentLocalPath = "Stage3.json";
				Load_FromJson(m_szJsonPath + m_szCurrentLocalPath);
			}

			if (ImGui::MenuItem("05. Stage1_sub"))
			{
				m_szCurrentLocalPath = "Stage1_sub.json";
				Load_FromJson(m_szJsonPath + m_szCurrentLocalPath);
			}

			if (ImGui::MenuItem("06. Stage2-2"))
			{
				m_szCurrentLocalPath = "Stage2-2.json";
				Load_FromJson(m_szJsonPath + m_szCurrentLocalPath);
			}

			if (ImGui::MenuItem("07. Stage_Lv3-1"))
			{
				m_szCurrentLocalPath = "Stage_Lv3-1.json";
				Load_FromJson(m_szJsonPath + m_szCurrentLocalPath);
			}

			if (ImGui::MenuItem("08. Stage_Home"))
			{
				m_szCurrentLocalPath = "Stage_Home.json";
				Load_FromJson(m_szJsonPath + m_szCurrentLocalPath);
			}

			if (ImGui::MenuItem("97. Test Level"))
			{
				m_szCurrentLocalPath = "Test_Level.json";
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
				GET_SINGLE(CGameManager)->OnLevelExit();
				Init_SceneEditer();
			}

			if (ImGui::MenuItem("Effect Editer"))
			{
				GET_SINGLE(CGameManager)->OnLevelExit();
				Init_EffectEditer();
			}

			if (ImGui::MenuItem("Model Editer"))
			{
				GET_SINGLE(CGameManager)->OnLevelExit();
				Init_ModelEditer();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	ID3D11DeviceContext* pDeviceContext = DEVICECONTEXT;

	for (_uint i = 0; i < m_arrWindows.size(); ++i)
	{
		if (m_arrWindows[i]->Is_Enable())
		{
			if (FAILED(m_arrWindows[i]->Render(pDeviceContext)))
				return E_FAIL;
		}
	}

	ImGui::Render();

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return S_OK;
}

void CImGui_Manager::Save_EffectJson()
{
	json DummuJson;

	GET_SINGLE(CWindow_EffectHierarchyView)->Write_Json(DummuJson);
	//GET_SINGLE(CWindow_EffectHierarchyView)->Write_FromBinary();
	GET_SINGLE(CWindow_EffectResourceView)->Load_EffectGroup();
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
	CameraDesc.fNear = 0.01f;
	CameraDesc.fFar = 300.f;

	m_pEditerCamera = GAMEINSTANCE->Add_GameObject<CCamera_Free>(LEVEL_EDIT, &CameraDesc);
	m_pTerrain = GAMEINSTANCE->Add_GameObject<CTerrain>(LEVEL_EDIT);
	m_pTerrain.lock()->Get_Component<Engine::CTransform>().lock()->Add_Position(XMVectorSet(-30.f, 0.f, -30.f, 1.f));
	
	for (auto& elem : m_arrWindows)
	{
		elem->Initialize();
	}

	for (auto& elem : m_arrWindows)
	{
		elem->Start();
	}

	CClientLevel::Loading_AllEffectGroup("..\\Bin\\EffectData\\", LEVEL::LEVEL_EDIT);
	m_eCurrentEditerType = EDITER_TYPE::EFFECT;
}

void CImGui_Manager::Init_ModelEditer()
{
	Release_CurrentEditer();

	m_arrWindows.emplace_back(CWindow_ModelContantView::Create_Instance());
	m_arrWindows.emplace_back(CWindow_TextureEditerView::Create_Instance());

	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));
	CameraDesc.vEye = _float4(0.0f, 4.f, 7.5f, 1.f);
	CameraDesc.vAt = _float4(0.f, 1.5f, 0.f, 1.f);
	CameraDesc.fFovy = XMConvertToRadians(65.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.01f;
	CameraDesc.fFar = 300.f;

	m_pEditerCamera = GAMEINSTANCE->Add_GameObject<CCamera_Free>(LEVEL_EDIT, &CameraDesc);
	m_pTerrain = GAMEINSTANCE->Add_GameObject<CTerrain>(LEVEL_EDIT);
	m_pTerrain.lock()->Get_Component<Engine::CTransform>().lock()->Add_Position(XMVectorSet(-30.f, 0.f, -30.f, 1.f));

	for (auto& elem : m_arrWindows)
	{
		elem->Initialize();
	}

	for (auto& elem : m_arrWindows)
	{
		elem->Start();
	}

	m_eCurrentEditerType = EDITER_TYPE::MODEL;
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

	CWindow_ModelContantView::Destroy_Instance();
	CWindow_TextureEditerView::Destroy_Instance();

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

	if (FAILED(CJson_Utility::Load_Json(In_szPath.c_str(), m_CurrentLevelJson)))
	{
#ifdef _DEBUG
		#ifdef _DEBUG_COUT_
		cout << In_szPath << " : 해당 경로에 Json 파일이 없음." << endl;
#endif
#endif // _DEBUG

		return;
	}

	for (_uint i = 0; i < m_arrWindows.size(); ++i)
	{
		m_arrWindows[i]->Load_FromJson(m_CurrentLevelJson);
	}
}

void CImGui_Manager::Toggle_PhysXInfo()
{
	m_bViewPhysXInfo = !m_bViewPhysXInfo;

	list<weak_ptr<CEditInstanceProp>>    Props = GAMEINSTANCE->Get_GameObjects<CEditInstanceProp>(LEVEL_EDIT);
	list<weak_ptr<CPhysXColliderObject>> ColliderObjects = GAMEINSTANCE->Get_GameObjects<CPhysXColliderObject>(LEVEL_EDIT);
	list<weak_ptr<CEditMapCollider>>     ColliderGroup = GAMEINSTANCE->Get_GameObjects<CEditMapCollider>(LEVEL_EDIT);

	for (auto& elem : Props)
	{
		if (elem.lock())
		{
			elem.lock()->OnEventMessage
			(
				(m_bViewPhysXInfo)
				? ((_uint)EVENT_TYPE::ON_EDIT_PHYSXINFO)
				: ((_uint)EVENT_TYPE::ON_EDIT_PHYSXINFO_N)
			);
		}
	}

	for (auto& elem : ColliderObjects)
	{
		if (elem.lock())
		{
			elem.lock()->OnEventMessage
			(
				(m_bViewPhysXInfo)
				? ((_uint)EVENT_TYPE::ON_EDIT_PHYSXINFO)
				: ((_uint)EVENT_TYPE::ON_EDIT_PHYSXINFO_N)
			);
		}
	}

	for (auto& elem : ColliderGroup)
	{
		if (elem.lock())
		{
			elem.lock()->OnEventMessage
			(
				(m_bViewPhysXInfo)
				? ((_uint)EVENT_TYPE::ON_EDIT_PHYSXINFO)
				: ((_uint)EVENT_TYPE::ON_EDIT_PHYSXINFO_N)
			);
		}
	}
}

_bool CImGui_Manager::Open_File(string& Out_szSelectedFile, string& Out_szFilePath)
{
	//  CREATE FILE OBJECT INSTANCE
	HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(f_SysHr))
		return FALSE;

	// CREATE FileOpenDialog OBJECT
	IFileOpenDialog* f_FileSystem;
	f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
	if (FAILED(f_SysHr)) {
		CoUninitialize();
		return FALSE;
	}

	try
	{
		//  SHOW OPEN FILE DIALOG WINDOW
		f_SysHr = f_FileSystem->Show(NULL);
		if (FAILED(f_SysHr)) {
			f_FileSystem->Release();
			CoUninitialize();
			return FALSE;
		}
	}
	catch(exception e)
	{
		return false;
	}
	

	//  RETRIEVE FILE NAME FROM THE SELECTED ITEM
	IShellItem* f_Files;
	f_SysHr = f_FileSystem->GetResult(&f_Files);
	if (FAILED(f_SysHr)) {
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}

	//  STORE AND CONVERT THE FILE NAME
	PWSTR f_Path;
	f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
	if (FAILED(f_SysHr)) {
		f_Files->Release();
		f_FileSystem->Release();
		CoUninitialize(); 
		return FALSE;
	}

	//  FORMAT AND STORE THE FILE PATH
	std::wstring path(f_Path);
	std::string c(path.begin(), path.end());
	Out_szFilePath = c;

	//  FORMAT STRING FOR EXECUTABLE NAME
	const size_t slash = Out_szFilePath.find_last_of("/\\");
	Out_szSelectedFile = Out_szFilePath.substr(slash + 1);

	//  SUCCESS, CLEAN UP
	CoTaskMemFree(f_Path);
	f_Files->Release();
	f_FileSystem->Release();
	CoUninitialize();
	return TRUE;
}

void CImGui_Manager::Free()
{
	Release_CurrentEditer();
}
