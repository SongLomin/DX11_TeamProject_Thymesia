#include "stdafx.h"
#include "UI_Loading.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "CustomUI.h"
#include "Player_ProgressBar.h"
#include "HUD_Hover.h"
#include "Fader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "HUD_Hover.h"
#include "FadeMask.h"
#include "DissolveUI.h"
#include "LoadingTextUI.h"

GAMECLASS_C(CUI_Loading)
CLONE_C(CUI_Loading, CGameObject)

HRESULT CUI_Loading::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CUI_Loading::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pLoadingBG = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_LOADING);
	m_pLoadingBG.lock()->Set_Depth(0.2f);

	UI_DESC	tIconDesc;
	tIconDesc.fX = 1550.f;
	tIconDesc.fY = 790.f;
	tIconDesc.fDepth = 0.1f;
	tIconDesc.fSizeX = 51.f;
	tIconDesc.fSizeY = 145.f;
	


	FaderDesc faderDesc;
	faderDesc.eFaderType = FADER_INOUTLOOPING;
	faderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
	faderDesc.fDelayTime = 0.f;
	faderDesc.fFadeMaxTime = 1.f;
	faderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 0.3f);

	CHUD_Hover::HUDHOVERDESC hoverDesc;

	hoverDesc.bSizeChange = false;
	hoverDesc.eType = CHUD_Hover::HUD_HOVER_ANIMATION_END;

	m_pIcon = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_LOADING, &tIconDesc);
	m_pIcon.lock()->Init_Fader(faderDesc, hoverDesc);
	m_pIcon.lock()->Set_Texture("Loading_Icon");

	tIconDesc.fX = 1400.f;
	tIconDesc.fY = 850.f;
	tIconDesc.fDepth = 0.1f;
	tIconDesc.fSizeX = 400.f;
	tIconDesc.fSizeY = 75.f;

	m_pLoadComplete = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_LOADING, &tIconDesc);
	m_pLoadComplete.lock()->Init_Fader(faderDesc, hoverDesc);
	m_pLoadComplete.lock()->Set_Texture("Loading_Complete");
	m_pLoadComplete.lock()->Set_Enable(false);



	tIconDesc.fX = 175.f;
	tIconDesc.fY = 695.f;
	tIconDesc.fDepth = 0.1f;
	tIconDesc.fSizeX = 174.;
	tIconDesc.fSizeY = 32.f;


	m_pStageTitle = GAMEINSTANCE->Add_GameObject<CLoadingTextUI>(LEVEL_LOADING, &tIconDesc);
	m_pStageTitle.lock()->Set_Texture("Loading_Font_Fortress_Title");
	m_pStageTitle.lock()->Set_DelayAlpha(1.f, 1.f);//1초뒤에, 1초걸리면 1.찍히는 알파 넣음
	//563 900
	tIconDesc.fX = 597.f;
	tIconDesc.fY = 748.f;
	tIconDesc.fDepth = 0.1f;
	tIconDesc.fSizeX = 1000.f;
	tIconDesc.fSizeY = 64.f;

	m_pStageDesc= GAMEINSTANCE->Add_GameObject<CLoadingTextUI>(LEVEL_LOADING, &tIconDesc);
	m_pStageDesc.lock()->Set_Texture("Loading_Font_Fortress_Desc");
	m_pStageDesc.lock()->Set_DelayAlpha(3.f, 2.f);
	m_bLoadComplete = false;
	m_bCallFadeOut = false;
	return S_OK;
}

HRESULT CUI_Loading::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_Loading::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);




}

void CUI_Loading::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

void CUI_Loading::Call_FadeEnd()
{
	m_bCallFadeOut = true;
}

void CUI_Loading::SetUp_LoadingUI(LEVEL eLevel)
{
	switch (eLevel)
	{
	case Client::LEVEL_STATIC:
		break;
	case Client::LEVEL_LOADING:
		break;
	case Client::LEVEL_LOGO:
		break;
	case Client::LEVEL_LOBBY:
		break;
	case Client::LEVEL_GAMEPLAY:
		m_pLoadingBG.lock()->Set_Texture("Loading_Tutorial");
		break;
	case Client::LEVEL_STAGE2:
		m_pLoadingBG.lock()->Set_Texture("Loading_Tutorial");
		break;
	case Client::LEVEL_STAGE3:
		m_pLoadingBG.lock()->Set_Texture("Loading_Tutorial");
		break;
	case Client::LEVEL_EDIT:
		break;
	case Client::LEVEL_END:
		break;
	default:
		break;
	}
}

void CUI_Loading::Set_Complete()
{
	m_bLoadComplete = true;
	m_pIcon.lock()->Set_Enable(false);
	m_pLoadComplete.lock()->Set_Enable(true);
}

_bool CUI_Loading::Get_Finish()
{
	if (m_bLoadComplete)
	{
		if (KEY_INPUT(KEY::ENTER, KEY_STATE::TAP))
		{
			FaderDesc tFaderDesc;
			tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
			tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
			tFaderDesc.fFadeMaxTime = 1.f;
			tFaderDesc.fDelayTime = 0.5f;
			tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);

			weak_ptr<CFadeMask> pFadeMask;

			pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();
			pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
			pFadeMask.lock()->CallBack_FadeEnd += bind(&CUI_Loading::Call_FadeEnd, this);

		}
		return m_bCallFadeOut;
	}
	return false;
}




void CUI_Loading::Free()
{
	__super::Free();

	
	
}
