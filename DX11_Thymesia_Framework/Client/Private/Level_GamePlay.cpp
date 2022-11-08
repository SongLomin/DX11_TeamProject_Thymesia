#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"
#include "GameInstance.h"
#include "Client_GameObjects.h"
#include "GameManager.h"
#include "FadeMask.h"
#include "Fader.h"


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

	

	//Load_FromJson(m_szDefaultJsonPath + "Stage1.json", LEVEL::LEVEL_GAMEPLAY);

	

	//GAMEINSTANCE->Add_GameObject<CDummy_Monster>(LEVEL_GAMEPLAY).lock()->Get_Component<CTransform>().lock()->Add_Position(XMVectorSet(-2.5f, 0.f, 5.f, 1.f));
	//GAMEINSTANCE->Add_GameObject<CDummy_Monster>(LEVEL_GAMEPLAY).lock()->Get_Component<CTransform>().lock()->Add_Position(XMVectorSet(5.f, 0.f, 5.f, 1.f));
	//GAMEINSTANCE->Add_GameObject<CDummy_Monster>(LEVEL_GAMEPLAY).lock()->Get_Component<CTransform>().lock()->Add_Position(XMVectorSet(-5.f, 0.f, 5.f, 1.f));
	//GAMEINSTANCE->Add_GameObject<CDummy_Monster>(LEVEL_GAMEPLAY).lock()->Get_Component<CTransform>().lock()->Add_Position(XMVectorSet(2.5f, 0.f, 5.f, 1.f));

	//GAMEINSTANCE->Add_GameObject<CSkyBox>(LEVEL_GAMEPLAY);
	

	/*GAMEINSTANCE->Add_GameObject<CEffect_Point>(LEVEL_GAMEPLAY);
	GAMEINSTANCE->Add_GameObject<CEffect_Point>(LEVEL_GAMEPLAY);
	GAMEINSTANCE->Add_GameObject<CEffect_Point>(LEVEL_GAMEPLAY);
	GAMEINSTANCE->Add_GameObject<CEffect_Point>(LEVEL_GAMEPLAY);
	GAMEINSTANCE->Add_GameObject<CEffect_Point>(LEVEL_GAMEPLAY);
	GAMEINSTANCE->Add_GameObject<CEffect_Point>(LEVEL_GAMEPLAY);
	GAMEINSTANCE->Add_GameObject<CEffect_Point>(LEVEL_GAMEPLAY).lock()->Get_Component<CTransform>().lock()->Add_Position(XMVectorSet(-5.f, 0.f, 0.f, 1.f));*/
	
	//GAMEINSTANCE->Add_GameObject<CCustomEffectMesh>(LEVEL_GAMEPLAY);

	//GET_SINGLE(CGameManager)->Set_TargetForTargetCamera(Player.front());

	m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();


	CFader::FaderDesc tFaderDesc;
	tFaderDesc.eFaderType = FADER_TYPE::FADER_IN;
	tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
	tFaderDesc.fFadeMaxTime = 3.f;
	tFaderDesc.fDelayTime = 0.5f;
	tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 0.f);

	m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);

	//GAMEINSTANCE->Add_GameObject<CDubian>(LEVEL::LEVEL_GAMEPLAY).lock()->Set_Enable(false);
	//GAMEINSTANCE->Add_GameObject<CBianka>(LEVEL::LEVEL_GAMEPLAY).lock()->Set_Enable(false);

	//GAMEINSTANCE->Add_GameObject<CStage1>(LEVEL::LEVEL_GAMEPLAY);

	

	//m_iMonsterCount = GET_SINGLE(CGameManager)->Get_Layer(OBJECT_LAYER::MONSTER).size();

	m_iMonsterCount = 0;

	GAMEINSTANCE->Set_ShadowLight({ -15.f, 30.f, -15.f }, { 0.f, 0.f, 0.f });


	return S_OK;
}

void CLevel_GamePlay::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);		

	if (m_bChangeNextLevel)
	{
		m_bChangeNextLevel = false;

		m_eNextLevel = LEVEL_STAGE2;

		CFader::FaderDesc tFaderDesc;
		tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
		tFaderDesc.eLinearType = LINEAR_TYPE::POW;
		tFaderDesc.fFadeMaxTime = 1.f;
		tFaderDesc.fDelayTime = 0.5f;
		tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);

		m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
		m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CClientLevel::Call_FadeOutToLevelChange, this);

	}

}

HRESULT CLevel_GamePlay::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	SetWindowText(g_hWnd, TEXT("게임프렐이레벨임. "));

	return S_OK;
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

