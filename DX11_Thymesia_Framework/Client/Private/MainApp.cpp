#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "GameManager.h"
#include "FadeMask.h"
#include "DeveloperConsole_Manager.h"
#include "Camera_Target.h"
#include "UIManager.h"

#include <imgui_impl_dx11.h>
#include "imgui_impl_win32.h"

CMainApp::CMainApp()
{

}

CMainApp::~CMainApp()
{
	Free();
}

HRESULT CMainApp::Initialize()
{
	
	ShowCursor(false);

	GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;
	GraphicDesc.isWindowMode = GRAPHICDESC::MODE_WIN;

#ifdef _RENDER_FPS_
	m_TextInfo_FPS.bCenterAlign = false;
	m_TextInfo_FPS.bAlways		= false;
	m_TextInfo_FPS.fRotation = 0.f;
	XMStoreFloat4(&m_TextInfo_FPS.vColor, XMVectorSet(0.f, 0.8588f, 0.176f, 1.f));
	XMStoreFloat2(&m_TextInfo_FPS.vPosition, XMVectorSet(2.f, 2.f, 0.f, 0.f));
	XMStoreFloat2(&m_TextInfo_FPS.vScale, XMVectorSet(0.55f, 0.55f, 0.f, 0.f));
#endif // _RENDER_FPS_

	CGameInstance::Create_Instance();
	CUIManager::Create_Instance();

	GET_SINGLE(CUIManager)->Add_SoundType(UI_SOUND_TYPE::SOUND_BGM, 2.0f);
	GET_SINGLE(CUIManager)->Add_SoundType(UI_SOUND_TYPE::SOUND_CHANGE_SELECT, 1.f);
	GET_SINGLE(CUIManager)->Add_SoundType(UI_SOUND_TYPE::SOUND_CHOOSE_SELECT, 1.f);
	GET_SINGLE(CUIManager)->Add_SoundType(UI_SOUND_TYPE::SOUND_EFFECT, 1.f);

	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	DWORD dwNumberOfProcessors = sysinfo.dwNumberOfProcessors;
	//DWORD dwNumberOfProcessors = 1;

	if (FAILED(GAMEINSTANCE->Initialize_Engine(g_hInst, LEVEL_END, (_uint)TIMESCALE_LAYER::LAYER_END, (_uint)COLLISION_LAYER::LAYER_END, GraphicDesc, (_uint)dwNumberOfProcessors)))
		return E_FAIL;	

	CGameManager::Create_Instance()->Initialize((_uint)dwNumberOfProcessors);

	GAMEINSTANCE->Reserve_Event((_uint)EVENT_TYPE::EVENT_END);
	GAMEINSTANCE->Check_Group((_uint)COLLISION_LAYER::PLAYER_ATTACK , (_uint)COLLISION_LAYER::MONSTER);
	GAMEINSTANCE->Check_Group((_uint)COLLISION_LAYER::PLAYER_ATTACK , (_uint)COLLISION_LAYER::DYNAMIC_PROP);
	GAMEINSTANCE->Check_Group((_uint)COLLISION_LAYER::MONSTER_ATTACK, (_uint)COLLISION_LAYER::PLAYER);
	GAMEINSTANCE->Check_Group((_uint)COLLISION_LAYER::MONSTER_ATTACK, (_uint)COLLISION_LAYER::BOSS_DYNAMIC_PROP);
	GAMEINSTANCE->Check_Group((_uint)COLLISION_LAYER::TRIGGER       , (_uint)COLLISION_LAYER::PLAYER);
	GAMEINSTANCE->Check_Group((_uint)COLLISION_LAYER::LADDER_DOWN   , (_uint)COLLISION_LAYER::PLAYER);
	GAMEINSTANCE->Check_Group((_uint)COLLISION_LAYER::LADDER_UP     , (_uint)COLLISION_LAYER::PLAYER);
	GAMEINSTANCE->Check_Group((_uint)COLLISION_LAYER::ELEVATOR      , (_uint)COLLISION_LAYER::PLAYER);
	GAMEINSTANCE->Check_Group((_uint)COLLISION_LAYER::DOOR          , (_uint)COLLISION_LAYER::PLAYER);
	GAMEINSTANCE->Check_Group((_uint)COLLISION_LAYER::CHECKPOINT    , (_uint)COLLISION_LAYER::PLAYER);
	GAMEINSTANCE->Check_Group((_uint)COLLISION_LAYER::MONSTER       , (_uint)COLLISION_LAYER::PLAYER_BATCOL);
	GAMEINSTANCE->Check_Group((_uint)COLLISION_LAYER::CHECK_DIR     , (_uint)COLLISION_LAYER::PLAYER);
	GAMEINSTANCE->Check_Group((_uint)COLLISION_LAYER::INTERIOR      , (_uint)COLLISION_LAYER::INTERIOR);

	GAMEINSTANCE->Check_PhysXFilterGroup((_uint)PHYSX_COLLISION_LAYER::DYNAMIC_PEICE, (_uint)PHYSX_COLLISION_LAYER::GROUND);
	GAMEINSTANCE->Check_PhysXFilterGroup((_uint)PHYSX_COLLISION_LAYER::DYNAMIC_PEICE, (_uint)PHYSX_COLLISION_LAYER::DYNAMIC_PEICE);
	GAMEINSTANCE->Check_PhysXFilterGroup((_uint)PHYSX_COLLISION_LAYER::DYNAMIC_PEICE, (_uint)PHYSX_COLLISION_LAYER::STATIC_PROP);
	GAMEINSTANCE->Check_PhysXFilterGroup((_uint)PHYSX_COLLISION_LAYER::MONSTER		, (_uint)PHYSX_COLLISION_LAYER::STATIC_PROP);
	GAMEINSTANCE->Check_PhysXFilterGroup((_uint)PHYSX_COLLISION_LAYER::MONSTER		, (_uint)PHYSX_COLLISION_LAYER::GROUND);
	GAMEINSTANCE->Check_PhysXFilterGroup((_uint)PHYSX_COLLISION_LAYER::PLAYER		, (_uint)PHYSX_COLLISION_LAYER::STATIC_PROP);
	GAMEINSTANCE->Check_PhysXFilterGroup((_uint)PHYSX_COLLISION_LAYER::PLAYER		, (_uint)PHYSX_COLLISION_LAYER::DYNAMIC_PROP);
	GAMEINSTANCE->Check_PhysXFilterGroup((_uint)PHYSX_COLLISION_LAYER::PLAYER		, (_uint)PHYSX_COLLISION_LAYER::GROUND);
	GAMEINSTANCE->Check_PhysXFilterGroup((_uint)PHYSX_COLLISION_LAYER::PLAYER		, (_uint)PHYSX_COLLISION_LAYER::MONSTER);
	GAMEINSTANCE->Check_PhysXFilterGroup((_uint)PHYSX_COLLISION_LAYER::PLAYER		, (_uint)PHYSX_COLLISION_LAYER::INTERIOR);
	GAMEINSTANCE->Check_PhysXFilterGroup((_uint)PHYSX_COLLISION_LAYER::PLAYER_WEAPON, (_uint)PHYSX_COLLISION_LAYER::DYNAMIC_PROP);
	GAMEINSTANCE->Check_PhysXFilterGroup((_uint)PHYSX_COLLISION_LAYER::CAMERA		, (_uint)PHYSX_COLLISION_LAYER::STATIC_PROP);
	GAMEINSTANCE->Check_PhysXFilterGroup((_uint)PHYSX_COLLISION_LAYER::CAMERA		, (_uint)PHYSX_COLLISION_LAYER::GROUND);
	GAMEINSTANCE->Check_PhysXFilterGroup((_uint)PHYSX_COLLISION_LAYER::DYNAMIC_PROP , (_uint)PHYSX_COLLISION_LAYER::GROUND);
	GAMEINSTANCE->Check_PhysXFilterGroup((_uint)PHYSX_COLLISION_LAYER::DYNAMIC_PROP , (_uint)PHYSX_COLLISION_LAYER::STATIC_PROP);

	GAMEINSTANCE->Load_Shader(TEXT("Shader_VtxColor"), TEXT("../Bin/ShaderFiles/Shader_VtxColor.hlsl"));
	GAMEINSTANCE->Add_SingleGameObject<CFadeMask>(LEVEL_STATIC);

	//Bake_MipMaps_Recursive("..\\Bin\\Resources\\Meshes\\Corvus");
	//Bake_MipMaps_Recursive("..\\Bin\\Resources\\Meshes\\Boss\\Urd");
	//Bake_MipMaps_Recursive("..\\Bin\\Resources\\Meshes\\Map_Lv1_Circus\\Binary");
	//Bake_MipMaps_Recursive("..\\Bin\\Resources\\Meshes\\Map_Lv2_Fortress\\Binary");
	//Bake_MipMaps_Recursive("..\\Bin\\Resources\\Meshes\\Map_Lv3_Garden\\Binary");
	//Bake_MipMaps_Recursive("..\\Bin\\Resources\\Meshes\\Map_Else\\Binary");
	//Bake_MipMaps_Recursive("..\\Bin\\Resources\\Meshes\\Boss\\Varg");
	//Bake_MipMaps_Recursive("..\\Bin\\Resources\\Meshes\\Boss\\Urd");
	//Bake_MipMaps_Recursive("..\\Bin\\Resources\\Meshes\\Boss\\Bat");

#ifdef _BAKE_MIPMAPS_
	Bake_MipMaps();
	MSG_BOX("BAKE SUCCEEDED.");
	
#else
	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;
#endif // _BAKE_MIPMAPS_

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

//#ifdef _DEBUG
	m_pDeveloperConsole = CDeveloperConsole_Manager::Create_Instance();
	m_pDeveloperConsole->Initialize();
//#endif // _DEBUG

	GAMEINSTANCE->Load_Textures("IrradianceMap", TEXT("../Bin/Resources/Textures/IrradianceMap/IrradianceMap0.dds"), MEMORY_TYPE::MEMORY_DYNAMIC);
	GAMEINSTANCE->Set_IrradianceMap("IrradianceMap");
	GAMEINSTANCE->Load_Textures("PreFilter", TEXT("../Bin/Resources/Textures/PreFilterIrradiance/PreFilter%d.dds"));
	GAMEINSTANCE->Set_PreFilteredMap("PreFilter");
	return S_OK;
}

void CMainApp::Tick(float fTimeDelta)
{
#ifndef _EFFECT_TOOL_
	if (GetFocus())
	{
		if (KEY_INPUT(KEY::CTRL, KEY_STATE::HOLD) && KEY_INPUT(KEY::ALT, KEY_STATE::HOLD) && KEY_INPUT(KEY::NUM0, KEY_STATE::TAP))
		{
			m_bClip = !m_bClip;
		}

		if (m_bClip)
		{
			POINT WinSize{ g_iWinCX , g_iWinCY };
			ClientToScreen(g_hWnd, &WinSize);
			RECT ClientRect = { _long(WinSize.x - g_iWinCX), _long(WinSize.y - g_iWinCY), (_long)WinSize.x, (_long)WinSize.y };
			ClipCursor(&ClientRect);
		}
		else
		{
			RECT ClientRect = { -99999, -99999, 99999, 99999 };
			ClipCursor(&ClientRect);
		}
	}
#endif // _EFFECT_TOOL_

//#ifdef _DEBUG
	if (KEY_INPUT(KEY::GRAVE, KEY_STATE::TAP))
	{
		m_bEnableConsole = !m_bEnableConsole;
		m_pDeveloperConsole->OnEnableConsole(m_bEnableConsole);
		

		ShowCursor(m_bEnableConsole);
		weak_ptr<CCamera_Target> pTargetCamera = GET_SINGLE(CGameManager)->Get_TargetCamera();
		weak_ptr<CPlayer> pPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

		if (pTargetCamera.lock())
		{
			pTargetCamera.lock()->Set_StopCamera(m_bEnableConsole);
		}

		if (pPlayer.lock())
		{
			pPlayer.lock()->Set_Edit(m_bEnableConsole);
		}
	}
//#endif // _DEBUG

	if (nullptr == GAMEINSTANCE)
		return;

	GAMEINSTANCE->Add_Chromatic(-0.5f * fTimeDelta);
	GAMEINSTANCE->Add_RadialBlur(-0.5f * fTimeDelta);
	GAMEINSTANCE->Add_MotionBlur(-0.4f * fTimeDelta);

	GAMEINSTANCE->Tick_Engine(fTimeDelta);
	GET_SINGLE(CUIManager)->Tick(fTimeDelta);

	
//#ifdef _DEBUG
	if (m_pDeveloperConsole && m_bEnableConsole)
	{
		m_pDeveloperConsole->Tick(fTimeDelta);
	}
	
	if (m_pDeveloperConsole)
	{
		m_pDeveloperConsole->Background_Tick(fTimeDelta);
	}
//#endif // _DEBUG

	GET_SINGLE(CGameManager)->LateTick(fTimeDelta);
	GET_SINGLE(CUIManager)->LateTick(fTimeDelta);

#ifdef	_RENDER_FPS_
	m_fTimeAcc += fTimeDelta;

	if (m_fTimeAcc >= 1.f)
	{
		wsprintf(m_szFPS, TEXT("%d"), m_iNumRender);
		m_TextInfo_FPS.szText = m_szFPS;
		m_fTimeAcc = 0.f;
		m_iNumRender = 0;
		m_TextInfo_FPS.vScale = _float2(8.f, 8.f);
	}

	++m_iNumRender;

	GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::DREAM, m_TextInfo_FPS);
#endif // _RENDER_FPS_
}

HRESULT CMainApp::Render()
{
	if (nullptr == GAMEINSTANCE)
		return E_FAIL;

	GAMEINSTANCE->Clear_BackBuffer_View(_float4(0.f, 0.215686f, 0.588235f, 1.f));
	GAMEINSTANCE->Clear_DepthStencil_View();

	//DEVICECONTEXT->ExecuteCommandList();

	GAMEINSTANCE->Draw_RenderGroup();
	GAMEINSTANCE->Render_Engine();

//#ifdef _DEBUG
	if (m_pDeveloperConsole && m_bEnableConsole)
	{
		m_pDeveloperConsole->Render(DEVICECONTEXT);
	}
//#endif // _DEBUG

	GAMEINSTANCE->Present();
	return S_OK;
}


HRESULT CMainApp::Open_Level(LEVEL eLevelID)
{
	if (nullptr == GAMEINSTANCE)
		return E_FAIL;

	shared_ptr<CLevel_Loading>		pLevel_Loading = CLevel_Loading::Create(eLevelID);
	if (nullptr == pLevel_Loading.get())
		return E_FAIL;
	
	if (FAILED(GAMEINSTANCE->Open_Level(LEVEL_LOADING, pLevel_Loading)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Bake_MipMaps()
{
	fs::directory_iterator itr("..\\Bin\\Resources\\Meshes");
	tstring szPath;
	tstring szFileName;

	while (itr != fs::end(itr)) {
		const fs::directory_entry& entry = *itr;
		szFileName = entry.path().filename().wstring();

#ifdef _DEBUG_COUT_
		cout << entry.path().filename() << std::endl;
#endif

		Bake_MipMaps_Recursive(entry.path());

		itr++;
	}

	//GAMEINSTANCE->Load_TexturesGenerateMipMap(("WHYTHROWSIBAL"), TEXT("../Bin/T_C_CorvusBirdClaw01_C.dds"), MEMORY_TYPE::MEMORY_STATIC);
	//GAMEINSTANCE->Load_TexturesGenerateMipMap(("WHYTHROWSIBAL"), TEXT("../Bin/basket_01_wire_087225143_BaseColor.png"), MEMORY_TYPE::MEMORY_STATIC);
	//GAMEINSTANCE->Load_TexturesGenerateMipMap(("WHYTHROWSIBAL"), TEXT("../Bin/Resources/Meshes/Map_Lv1_Circus/Binary/TX_FF10_N.png"), MEMORY_TYPE::MEMORY_STATIC);

}

void CMainApp::Bake_MipMaps_Recursive(const filesystem::path& In_Path)
{
	if (!In_Path.filename().extension().string().empty())
		return;

	fs::directory_iterator itr(In_Path);
	tstring szPath;
	string szFileName;

	while (itr != fs::end(itr)) 
	{
		const fs::directory_entry& entry = *itr;
		

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
		cout << entry.path().filename() << std::endl;
#endif
#endif

		Bake_MipMaps_Recursive(entry.path());

		

		if (strcmp(entry.path().extension().string().c_str(), ".png") == 0)
		{
			szPath = entry.path().wstring();

			GAMEINSTANCE->Generate_MipMap(szPath.c_str());
		}

		itr++;
	}
}

unique_ptr<CMainApp> CMainApp::Create()
{
	unique_ptr<CMainApp>		pInstance = make_unique<CMainApp>();
	pInstance->Initialize();
	return pInstance;
}

void CMainApp::Free()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CGameInstance::Release_Engine();
	CGameInstance::Destroy_Instance();
	CGameManager::Destroy_Instance();

#ifdef _DEBUG
	m_pDeveloperConsole.reset();
	CDeveloperConsole_Manager::Destroy_Instance();
#endif // _DEBUG
}
