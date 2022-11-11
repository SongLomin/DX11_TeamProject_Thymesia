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

GAMECLASS_C(CUI_Loading)
CLONE_C(CUI_Loading, CGameObject)

HRESULT CUI_Loading::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Loading::Initialize(void* pArg)
{

	m_pLoadingBG = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_LOADING);
	m_pLoadingBG.lock()->Set_Texture("Loading_Tutorial");
	m_pLoadingBG.lock()->Set_Depth(0.2f);

	UI_DESC	tIconDesc;
	tIconDesc.fX = 1500.f;
	tIconDesc.fY = 790.f;
	tIconDesc.fDepth = 0.1f;
	tIconDesc.fSizeX = 102.f;
	tIconDesc.fSizeY = 290.f;
	


	FaderDesc faderDesc;
	faderDesc.eFaderType = FADER_INOUTLOOPING;
	faderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
	faderDesc.fDelayTime = 0.f;
	faderDesc.fFadeMaxTime = 1.f;
	faderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 0.3f);

	CHUD_Hover::HUDHOVERDESC hoverDesc;

	hoverDesc.m_bSizeChange = false;


	m_pIcon = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_LOADING, &tIconDesc);
	m_pIcon.lock()->Init_Fader(faderDesc, hoverDesc, CHUD_Hover::HUD_HOVER_ANIMATION_END);
	m_pLoadingBG.lock()->Set_Texture("Loading_Icon");
	m_fRatio = 0;

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

void CUI_Loading::Free()
{
	__super::Free();
}
