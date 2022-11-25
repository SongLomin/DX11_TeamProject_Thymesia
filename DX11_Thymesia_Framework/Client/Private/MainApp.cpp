#include "stdafx.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "GameManager.h"
#include "FadeMask.h"



CMainApp::CMainApp()
{

}

CMainApp::~CMainApp()
{
	Free();
}

HRESULT CMainApp::Initialize()
{
	GRAPHICDESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof(GRAPHICDESC));

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinCX = g_iWinCX;
	GraphicDesc.iWinCY = g_iWinCY;
	GraphicDesc.isWindowMode = GRAPHICDESC::MODE_WIN;


	CGameInstance::Create_Instance();
	CGameManager::Create_Instance();

	if (FAILED(GAMEINSTANCE->Initialize_Engine(g_hInst, LEVEL_END, (_uint)TIMESCALE_LAYER::LAYER_END, (_uint)COLLISION_LAYER::LAYER_END, GraphicDesc)))
		return E_FAIL;	


	GAMEINSTANCE->Reserve_Event((_uint)EVENT_TYPE::EVENT_END);

	//GAMEINSTANCE->Check_Group((_uint)COLLISION_LAYER::PLAYER, (_uint)COLLISION_LAYER::MONSTER);
	GAMEINSTANCE->Check_Group((_uint)COLLISION_LAYER::PLAYER_ATTACK, (_uint)COLLISION_LAYER::MONSTER);
	GAMEINSTANCE->Check_Group((_uint)COLLISION_LAYER::MONSTER_ATTACK, (_uint)COLLISION_LAYER::PLAYER);
	GAMEINSTANCE->Check_Group((_uint)COLLISION_LAYER::TRIGGER, (_uint)COLLISION_LAYER::PLAYER);


	
	GAMEINSTANCE->Check_PhysXFilterGroup((_uint)COLLISION_LAYER::PLAYER, (_uint)COLLISION_LAYER::MONSTER);



	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;


	GAMEINSTANCE->Load_Shader(TEXT("Shader_VtxColor"), TEXT("../Bin/ShaderFiles/Shader_VtxColor.hlsl"));
	GAMEINSTANCE->Add_GameObject<CFadeMask>(LEVEL_STATIC);

	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;


	return S_OK;
}

void CMainApp::Tick(float fTimeDelta)
{
	if (GetFocus())
	{
		if (KEY_INPUT(KEY::TAB, KEY_STATE::TAP))
		{
			m_bClip = !m_bClip;
		}

		if (m_bClip)
		{
			POINT WinSize{ g_iWinCX , g_iWinCY };
			ClientToScreen(g_hWnd, &WinSize);
			RECT ClientRect = { (_long)WinSize.x - g_iWinCX, (_long)WinSize.y - g_iWinCY, (_long)WinSize.x, (_long)WinSize.y };
			ClipCursor(&ClientRect);
		}
		else
		{
			RECT ClientRect = { 0, 0, 99999, 99999 };
			ClipCursor(&ClientRect);
		}
	}

	if (nullptr == GAMEINSTANCE)
		return;

	//GAMEINSTANCE->Add_MotionBlurScale(-0.2f * fTimeDelta);
	GAMEINSTANCE->Add_Chromatic(-0.2f * fTimeDelta);

	GAMEINSTANCE->Tick_Engine(fTimeDelta);

	GET_SINGLE(CGameManager)->LateTick(fTimeDelta);
	
	m_fTimeAcc += fTimeDelta;
}

HRESULT CMainApp::Render()
{
	if (nullptr == GAMEINSTANCE/* ||
		nullptr == m_pRenderer*/)
		return E_FAIL;

	GAMEINSTANCE->Clear_BackBuffer_View(_float4(0.f, 128.f / 255.f, 1.f, 1.f));
	GAMEINSTANCE->Clear_DepthStencil_View();
	
	GAMEINSTANCE->Draw_RenderGroup();

	GAMEINSTANCE->Render_Engine();

	//GAMEINSTANCE->Render_Font((_uint)FONT_INDEX::DREAM);


	if (m_fTimeAcc >= 1.f)
	{
		wsprintf(m_szFPS, TEXT("FPS : %d"), m_iNumRender);
		m_fTimeAcc = 0.f;
		m_iNumRender = 0;
	}

	++m_iNumRender;

	GAMEINSTANCE->Render_Font((_uint)FONT_INDEX::DREAM, m_szFPS, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));

	

	// MakeSpriteFont "폰트이름" /FontSize:32 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 출력파일이름.spritefont
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

HRESULT CMainApp::Ready_Prototype_Component()
{
	if (nullptr == GAMEINSTANCE)
		return E_FAIL;

	///* For.Prototype_Component_Renderer */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
	//	m_pRenderer = CRenderer::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	///* For.Prototype_Component_VIBuffer_Rect*/
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
	//	CVIBuffer_Rect::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	///* For.Prototype_Component_Transform */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
	//	CTransform::Create(m_pGraphic_Device))))
	//	return E_FAIL;

	///* For.Prototype_Component_Shader_Rect */
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Rect"),
	//	CShader::Create(m_pGraphic_Device, TEXT("..'Shader_Rect.hlsl")))))
	//	return E_FAIL;

	//Safe_AddRef(m_pRenderer);

	return S_OK;
}

unique_ptr<CMainApp> CMainApp::Create()
{
	unique_ptr<CMainApp>		pInstance = make_unique<CMainApp>();
	pInstance->Initialize();
	return pInstance;
}

void CMainApp::Free()
{
	CGameInstance::Release_Engine();
	CGameInstance::Destroy_Instance();
	CGameManager::Destroy_Instance();
}
