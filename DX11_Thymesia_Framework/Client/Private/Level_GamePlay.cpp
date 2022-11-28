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
#include "UI_Containers.h"
#include "UI_EvolveMenu.h"
#include "UI_EvolveMenu_Level.h"


CLevel_GamePlay::CLevel_GamePlay()
	//: CLevel(pDevice, pContext) ID3D11Device* pDevice, ID3D11DeviceContext* pContext
{
}

CLevel_GamePlay::~CLevel_GamePlay() { Free(); }

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


	
#ifdef _VARG_
	CBossMonster::STATE_LINK_BOSS_DESC BOSSMONSTER;

	ZeroMemory(&BOSSMONSTER, sizeof(CBossMonster::STATE_LINK_BOSS_DESC));
	BOSSMONSTER.vYame.x = 49.33f;
	BOSSMONSTER.vYame.y = 14.4f;
	BOSSMONSTER.vYame.z = 30.32f;
	BOSSMONSTER.eBossStartType = BOSSSTARTTYPE::NORMALSTART;
	GAMEINSTANCE->Add_GameObject<CVarg>(LEVEL_GAMEPLAY, &BOSSMONSTER);



	//ZeroMemory(&BOSSMONSTER, sizeof(CBossMonster::STATE_LINK_BOSS_DESC));
	//BOSSMONSTER.vYame.x = 33.f;
	//BOSSMONSTER.vYame.y = 4.7f;
	//BOSSMONSTER.vYame.z = 26.f;
	//GAMEINSTANCE->Add_GameObject<CUrd>(LEVEL_GAMEPLAY, &BOSSMONSTER);
#endif // _VARG_


	CMonster::STATE_LINK_MONSTER_DESC MONSTER;
	//TODO 야매에요
#ifdef _STAGE_1_MONSTER_
	CMonster::STATE_LINK_MONSTER_DESC MONSTER;
	//CBossMonster::STATE_LINK_MONSTER_DESC BOSSMONSTER;

	ZeroMemory(&MONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	MONSTER.eMonType = MONSTERTYPE::AXEMAN;
	MONSTER.eNorMonIdleType = NORMONSTERIDLETYPE::NORIDLE;
	MONSTER.vYame.x = 13.f;
	MONSTER.vYame.y = 4.6f;
	MONSTER.vYame.z = 36.f;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_GAMEPLAY, &MONSTER);
	
	ZeroMemory(&MONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	MONSTER.eMonType = MONSTERTYPE::KNIFEWOMAN;
	MONSTER.eNorMonIdleType = NORMONSTERIDLETYPE::NORIDLE;
	MONSTER.vYame.x = 13.f;
	MONSTER.vYame.y = 4.6f;
	MONSTER.vYame.z = 33.f;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_GAMEPLAY, &MONSTER);
	
	//ZeroMemory(&MONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	//MONSTER.eMonType = MONSTERTYPE::GARDENER;
	//MONSTER.eNorMonIdleType = NORMONSTERIDLETYPE::NORIDLE;
	//MONSTER.vYame.x = 15.f;
	//MONSTER.vYame.y = 4.9f;
	//MONSTER.vYame.z = 10.5f;
	//GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_GAMEPLAY, &MONSTER);

	ZeroMemory(&MONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	MONSTER.eMonType = MONSTERTYPE::ELITEGARDENER;
	MONSTER.eNorMonIdleType = NORMONSTERIDLETYPE::NORIDLE;
	MONSTER.vYame.x = 20.f;
	MONSTER.vYame.y = 4.6f;
	MONSTER.vYame.z = 15.f;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_GAMEPLAY, &MONSTER);

	ZeroMemory(&MONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	MONSTER.eMonType = MONSTERTYPE::SHIELDAXEMAN;
	MONSTER.eNorMonIdleType = NORMONSTERIDLETYPE::NORIDLE;
	MONSTER.vYame.x = 20.f;
	MONSTER.vYame.y = 4.6f;
	MONSTER.vYame.z = 36.f;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_GAMEPLAY, &MONSTER);
#endif // _STAGE_1_MONSTER_
	
	/*ZeroMemory(&MONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	MONSTER.eMonType = MONSTERTYPE::KNIFEWOMAN;
	MONSTER.eNorMonIdleType = NORMONSTERIDLETYPE::NORIDLE;
	MONSTER.vYame.x = 20.f;
	MONSTER.vYame.z = 25.f;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_GAMEPLAY, &MONSTER);*/

	//ZeroMemory(&MONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	//MONSTER.vYame.x = 30.f;
	//MONSTER.vYame.z = 15.f;
	//MONSTER.eBossStartType = BOSSSTARTTYPE::BEGINSTART;
	//MONSTER.eBossType = BOSSTYPE::BOSSVARG;
	//GAMEINSTANCE->Add_GameObject<CVarg>(LEVEL_GAMEPLAY, &MONSTER);
	//

	//ZeroMemory(&MONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	//MONSTER.vYame.x = 32.f;
	//MONSTER.vYame.y = 4.7f;
	//MONSTER.vYame.z = 28.f;
	//MONSTER.eMonType = MONSTERTYPE::VARG;
	//MONSTER.eBossStartType = BOSSSTARTTYPE::NORMALSTART;
	//GAMEINSTANCE->Add_GameObject<CVarg>(LEVEL_GAMEPLAY, &MONSTER);
	
	//ZeroMemory(&MONSTER, sizeof(CBossMonster::STATE_LINK_MONSTER_DESC));
	//MONSTER.vYame.x = 33.f;
	//MONSTER.vYame.y = 4.7f;
	//MONSTER.vYame.z = 26.f;
	//GAMEINSTANCE->Add_GameObject<CUrd>(LEVEL_GAMEPLAY, &MONSTER);

	/*ZeroMemory(&MONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	MONSTER.eMonType = MONSTERTYPE::KNIFEWOMAN;
	MONSTER.eNorMonIdleType = NORMONSTERIDLETYPE::FIDGETIDLE;
	MONSTER.vYame.x = 25.f;
	MONSTER.vYame.z = 30.f;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_GAMEPLAY, &MONSTER);
	
	ZeroMemory(&MONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	MONSTER.eMonType = MONSTERTYPE::KNIFEWOMAN;
	MONSTER.eNorMonIdleType = NORMONSTERIDLETYPE::SITIDLE;
	MONSTER.vYame.x = 30.f;
	MONSTER.vYame.z = 35.f;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_GAMEPLAY, &MONSTER);

	ZeroMemory(&MONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	MONSTER.eMonType = MONSTERTYPE::GARDENER;
	MONSTER.eNorMonIdleType = NORMONSTERIDLETYPE::NORIDLE;
	MONSTER.vYame.x = 40.f;
	MONSTER.vYame.z = 35.f;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_GAMEPLAY, &MONSTER);

	ZeroMemory(&MONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	MONSTER.eMonType = MONSTERTYPE::GARDENER;
	MONSTER.eNorMonIdleType = NORMONSTERIDLETYPE::SITIDLE;
	MONSTER.vYame.x = 40.f;
	MONSTER.vYame.z = 40.f;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_GAMEPLAY, &MONSTER);

	ZeroMemory(&MONSTER, sizeof(CMonster::STATE_LINK_MONSTER_DESC));
	MONSTER.eMonType = MONSTERTYPE::GARDENER;
	MONSTER.eNorMonIdleType = NORMONSTERIDLETYPE::FIDGETIDLE;
	MONSTER.vYame.x = 40.f;
	MONSTER.vYame.z = 45.f;
	GAMEINSTANCE->Add_GameObject<CNorMonster>(LEVEL_GAMEPLAY, &MONSTER);*/

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

	if (KEY_INPUT(KEY::T, KEY_STATE::TAP))
	{
		if (m_pEvolveMenu.lock()->Get_Enable() == false)
		{
			FaderDesc tFaderDesc;
			tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
			tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
			tFaderDesc.fFadeMaxTime = 0.2f;
			tFaderDesc.fDelayTime = 0.f;
			tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);

			m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
			m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CLevel_GamePlay::Call_Enable_EvolveMenu, this);
		}
	}
}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("게임프렐이레벨임. "));

	return S_OK;
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

