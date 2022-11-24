#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"
#include "GameInstance.h"
#include "Client_GameObjects.h"
#include "GameManager.h"
#include "FadeMask.h"
#include "Fader.h"
#include <UI_Landing.h>
#include "Static_Instancing_Prop.h"
#include "HUD_PlagueWeapon.h"
#include "UI_PauseMenu.h"
#include "MonsterHPBar_Base.h"
#include "MonsterHPBar_Elite.h"
#include "MonsterHPBar_Boss.h"
#include "MonsterParryingBar.h"


CLevel_GamePlay::CLevel_GamePlay()
	//: CLevel(pDevice, pContext) ID3D11Device* pDevice, ID3D11DeviceContext* pContext
{

}

CLevel_GamePlay::~CLevel_GamePlay()
{
	Free();
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	ShowCursor(false);

//주석지워라

#ifndef _ONLY_UI_
	Loading_AllEffectGroup("..\\Bin\\EffectData\\", LEVEL::LEVEL_GAMEPLAY);
#pragma region GAMEOBJECT

	/*future<void> ThreadResult = async(launch::async, 
		bind(&CLevel_GamePlay::Load_FromJson, this,
			placeholders::_1, placeholders::_2), 
		m_szDefaultJsonPath + "Stage1.json", 
		LEVEL::LEVEL_GAMEPLAY);*/

	//Load_FromJson(m_szDefaultJsonPath + "Stage1.json", LEVEL::LEVEL_GAMEPLAY);

#ifdef _STAGE_1_
/*future<void> ThreadResult2 = async(launch::async,
	bind(&CLevel_GamePlay::Load_FromJson, this,
		placeholders::_1, placeholders::_2),
	m_szDefaultJsonPath + "Stage2.json",
	LEVEL::LEVEL_GAMEPLAY);*/

	Load_FromJson(m_szDefaultJsonPath + "Stage1.json", LEVEL::LEVEL_GAMEPLAY);
#endif // _STAGE_1_

#ifdef _STAGE_2_
	/*future<void> ThreadResult2 = async(launch::async,
		bind(&CLevel_GamePlay::Load_FromJson, this,
			placeholders::_1, placeholders::_2),
		m_szDefaultJsonPath + "Stage2.json",
		LEVEL::LEVEL_GAMEPLAY);*/

	Load_FromJson(m_szDefaultJsonPath + "Stage2.json", LEVEL::LEVEL_GAMEPLAY);
#endif // _STAGE_2_

	

	//Load_FromJson(m_szDefaultJsonPath + "Stage1_sub.json", LEVEL::LEVEL_GAMEPLAY);
	CCamera::CAMERADESC			CameraDesc;
	ZeroMemory(&CameraDesc, sizeof(CCamera::CAMERADESC));
	CameraDesc.vEye = _float4(0.0f, 2.5f, -2.5f, 1.f);
	CameraDesc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.fFovy = XMConvertToRadians(65.0f);
	CameraDesc.fAspect = (_float)g_iWinCX / g_iWinCY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 300.f;

	weak_ptr<CCamera_Target> TargetCamera = GAMEINSTANCE->Add_GameObject<CCamera_Target>(LEVEL::LEVEL_GAMEPLAY, &CameraDesc);
	GET_SINGLE(CGameManager)->Set_TargetCamera(TargetCamera);

	weak_ptr<CCorvus> pCorvus = GAMEINSTANCE->Add_GameObject<CCorvus>(LEVEL_GAMEPLAY);
	GET_SINGLE(CGameManager)->Set_CurrentPlayer(pCorvus);
	
	//TODO 야매에요
#ifdef _STAGE_1_MONSTER_
	CMonster::STATE_LINK_MONSTER_DESC NORMONSTER;
	CBossMonster::STATE_LINK_BOSS_DESC BOSSMONSTER;

	ZeroMemory(&NORMONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	NORMONSTER.eMonType = MONSTERTYPE::AXEMAN;
	NORMONSTER.eNorMonIdleType = NORMONSTERIDLETYPE::NORIDLE;
	NORMONSTER.vYame.x = 13.f;
	NORMONSTER.vYame.y = 4.6f;
	NORMONSTER.vYame.z = 36.f;
	NORMONSTER.m_iAtkCounterGauge = 10;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_GAMEPLAY, &NORMONSTER);

	ZeroMemory(&NORMONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	NORMONSTER.eMonType = MONSTERTYPE::KNIFEWOMAN;
	NORMONSTER.eNorMonIdleType = NORMONSTERIDLETYPE::SITIDLE;
	NORMONSTER.vYame.x = 23.f;
	NORMONSTER.vYame.y = 4.6f;
	NORMONSTER.vYame.z = 43.f;
	NORMONSTER.m_iAtkCounterGauge = 10;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_GAMEPLAY, &NORMONSTER);
	
	ZeroMemory(&NORMONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	NORMONSTER.eMonType = MONSTERTYPE::GARDENER;
	NORMONSTER.eNorMonIdleType = NORMONSTERIDLETYPE::NORIDLE;
	NORMONSTER.vYame.x = 15.f;
	NORMONSTER.vYame.y = 4.9f;
	NORMONSTER.vYame.z = 10.5f;
	NORMONSTER.m_iAtkCounterGauge = 10;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_GAMEPLAY, &NORMONSTER);
#endif // _STAGE_1_MONSTER_
	
	/*ZeroMemory(&NORMONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	NORMONSTER.eMonType = MONSTERTYPE::KNIFEWOMAN;
	NORMONSTER.eNorMonIdleType = NORMONSTERIDLETYPE::NORIDLE;
	NORMONSTER.vYame.x = 20.f;
	NORMONSTER.vYame.z = 25.f;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_GAMEPLAY, &NORMONSTER);*/

	//ZeroMemory(&NORMONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	//NORMONSTER.vYame.x = 30.f;
	//NORMONSTER.vYame.z = 15.f;
	//NORMONSTER.eBossStartType = BOSSSTARTTYPE::BEGINSTART;
	//NORMONSTER.eBossType = BOSSTYPE::BOSSVARG;
	//GAMEINSTANCE->Add_GameObject<CVarg>(LEVEL_GAMEPLAY, &NORMONSTER);
	//

	ZeroMemory(&BOSSMONSTER, sizeof(CBossMonster::STATE_LINK_BOSS_DESC));
	BOSSMONSTER.vYame.x = 55.f;
	BOSSMONSTER.vYame.y = 4.7f;
	BOSSMONSTER.vYame.z = 6.3f;
	BOSSMONSTER.eBossStartType = BOSSSTARTTYPE::NORMALSTART;
	GAMEINSTANCE->Add_GameObject<CVarg>(LEVEL_GAMEPLAY, &BOSSMONSTER);

	/*ZeroMemory(&NORMONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	NORMONSTER.eMonType = MONSTERTYPE::KNIFEWOMAN;
	NORMONSTER.eNorMonIdleType = NORMONSTERIDLETYPE::FIDGETIDLE;
	NORMONSTER.vYame.x = 25.f;
	NORMONSTER.vYame.z = 30.f;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_GAMEPLAY, &NORMONSTER);
	
	ZeroMemory(&NORMONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	NORMONSTER.eMonType = MONSTERTYPE::KNIFEWOMAN;
	NORMONSTER.eNorMonIdleType = NORMONSTERIDLETYPE::SITIDLE;
	NORMONSTER.vYame.x = 30.f;
	NORMONSTER.vYame.z = 35.f;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_GAMEPLAY, &NORMONSTER);

	ZeroMemory(&NORMONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	NORMONSTER.eMonType = MONSTERTYPE::GARDENER;
	NORMONSTER.eNorMonIdleType = NORMONSTERIDLETYPE::NORIDLE;
	NORMONSTER.vYame.x = 40.f;
	NORMONSTER.vYame.z = 35.f;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_GAMEPLAY, &NORMONSTER);

	ZeroMemory(&NORMONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	NORMONSTER.eMonType = MONSTERTYPE::GARDENER;
	NORMONSTER.eNorMonIdleType = NORMONSTERIDLETYPE::SITIDLE;
	NORMONSTER.vYame.x = 40.f;
	NORMONSTER.vYame.z = 40.f;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_GAMEPLAY, &NORMONSTER);

	ZeroMemory(&NORMONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	NORMONSTER.eMonType = MONSTERTYPE::GARDENER;
	NORMONSTER.eNorMonIdleType = NORMONSTERIDLETYPE::FIDGETIDLE;
	NORMONSTER.vYame.x = 40.f;
	NORMONSTER.vYame.z = 45.f;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_GAMEPLAY, &NORMONSTER);*/

	//야매에요
	
	GAMEINSTANCE->Add_GameObject<CLight_Prop>(LEVEL_GAMEPLAY);

	//GAMEINSTANCE->Add_GameObject<CTerrain>(LEVEL_GAMEPLAY);
	/*weak_ptr<CPreViewAnimationModel> pPreviewModel = GAMEINSTANCE->Add_GameObject<CPreViewAnimationModel>(LEVEL_GAMEPLAY);
	pPreviewModel.lock()->Init_EditPreViewAnimationModel("Corvus");
	pPreviewModel.lock()->Change_AnimationFromIndex(3);

	pPreviewModel.lock()->Play_Animation(0.01f);
	pPreviewModel.lock()->Get_Component<CTransform>().lock()->Add_Position(XMVectorSet(10.f, 10.f, 10.f, 0.f));*/


	GAMEINSTANCE->Add_GameObject<CStatic_Instancing_Prop>(LEVEL_GAMEPLAY);
	GAMEINSTANCE->Add_GameObject<CSkyBox>(LEVEL_GAMEPLAY);

	GAMEINSTANCE->Set_ShadowLight({ -15.f, 30.f, -15.f }, { 0.f, 0.f, 0.f });
	
	
#pragma endregion GAMEOBJECT
#endif	
	SetUp_UI();

	m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();

	//ThreadResult.get();
#ifdef _STAGE_2_
	//ThreadResult2.get();
#endif // _STAGE_2_

	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

	if (KEY_INPUT(KEY::CTRL, KEY_STATE::TAP))
	{
		if (m_pPauseMenu.lock()->Get_Enable() == false)
		{
			FaderDesc tFaderDesc;
			tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
			tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
			tFaderDesc.fFadeMaxTime = 0.3f;
			tFaderDesc.fDelayTime = 0.f;
			tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);

			m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
			m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CLevel_GamePlay::Call_Enable_PauseMenu, this);
		}
	}

	if (!m_bFadeTrigger)
	{
		FaderDesc tFaderDesc;
		tFaderDesc.eFaderType = FADER_TYPE::FADER_IN;
		tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
		tFaderDesc.fFadeMaxTime = 3.f;
		tFaderDesc.fDelayTime = 0.f;
		tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 0.f);

		m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);

		m_bFadeTrigger = true;
	}
	/*if (m_bChangeNextLevel)
	{
		m_bChangeNextLevel = false;

		m_eNextLevel = LEVEL_STAGE2;

		FaderDesc tFaderDesc;
		tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
		tFaderDesc.eLinearType = LINEAR_TYPE::POW;
		tFaderDesc.fFadeMaxTime = 1.f;
		tFaderDesc.fDelayTime = 0.5f;
		tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);

		m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
		m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CClientLevel::Call_FadeOutToLevelChange, this);

	}*/

}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("게임프렐이레벨임. "));

	return S_OK;
}

void CLevel_GamePlay::SetUp_UI()
{
	weak_ptr<CGameManager>	pGameManager = GET_SINGLE(CGameManager);

	GAMEINSTANCE->Add_GameObject<CUI_Landing>(LEVEL_STATIC);//여기서 
	m_pPauseMenu = GAMEINSTANCE->Add_GameObject<CUI_PauseMenu>(LEVEL_STATIC);

	//GAMEINSTANCE->Add_GameObject<CMonsterParryingBar>(LEVEL_STATIC);


	pGameManager.lock()->Register_Layer(OBJECT_LAYER::BATTLEUI, GAMEINSTANCE->Add_GameObject<CPlayer_HPBar>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::BATTLEUI, GAMEINSTANCE->Add_GameObject<CPlayer_MPBar>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::BATTLEUI, GAMEINSTANCE->Add_GameObject<CPlayer_Memory>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::BATTLEUI, GAMEINSTANCE->Add_GameObject<CHUD_PlagueWeapon>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::BATTLEUI, GAMEINSTANCE->Add_GameObject<CPlayer_PotionUI>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::BATTLEUI, GAMEINSTANCE->Add_GameObject<CPlayer_FeatherUI>(LEVEL_STATIC));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::BATTLEUI, GAMEINSTANCE->Add_GameObject<CPlayer_HPBar>(LEVEL_STATIC));

	//TODO : MonsterHpBar TestCode
	/*
	CUI::UI_DESC tDesc;
	tDesc.fX = g_iWinCX / 2.f;
	tDesc.fY = g_iWinCY / 2.f;
	tDesc.fSizeX = 150.f;
	tDesc.fSizeY = 15.f;
	tDesc.fDepth = 0.f;

	pGameManager.lock()->Register_Layer(OBJECT_LAYER::BATTLEUI, GAMEINSTANCE->Add_GameObject<CMonsterHPBar_Elite>(LEVEL_STATIC, &tDesc));
	pGameManager.lock()->Register_Layer(OBJECT_LAYER::BATTLEUI, GAMEINSTANCE->Add_GameObject<CMonsterHPBar_Boss>(LEVEL_STATIC, &tDesc));
	*/
}

void CLevel_GamePlay::Call_Enable_PauseMenu()
{
		m_pPauseMenu.lock()->Set_Enable(true);
		m_pFadeMask.lock()->Set_Enable(false);
}

shared_ptr<CLevel_GamePlay> CLevel_GamePlay::Create()
{
	shared_ptr<CLevel_GamePlay>		pInstance = make_shared<CLevel_GamePlay>();
	pInstance->m_eMyLevel = LEVEL_GAMEPLAY;
	pInstance->Initialize();
	return pInstance;
}

void CLevel_GamePlay::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);
}

void CLevel_GamePlay::Free()
{

}

