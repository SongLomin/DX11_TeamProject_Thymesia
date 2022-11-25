#include "stdafx.h"
#include "UI_EvolveMenu_Level.h"
#include "UI_Containers.h"
#include "UI_Elements.h"
#include "GameManager.h"
#include "FadeMask.h"
#include "UI_EvolveMenu.h"

GAMECLASS_C(CUI_EvolveMenu_Level)
CLONE_C(CUI_EvolveMenu_Level, CGameObject)


HRESULT CUI_EvolveMenu_Level::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	Create_Background();
	Set_Enable(false);
    return S_OK;
}

void CUI_EvolveMenu_Level::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (KEY_INPUT(KEY::T, KEY_STATE::TAP))
	{
		FaderDesc tFaderDesc;
		tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
		tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
		tFaderDesc.fFadeMaxTime = 0.2f;
		tFaderDesc.fDelayTime = 0.f;
		tFaderDesc.vFadeColor = _float4(0.f, 0.f, 0.f, 1.f);

		m_pFadeMask.lock()->Init_Fader((void*)&tFaderDesc);
		m_pFadeMask.lock()->CallBack_FadeEnd += bind(&CUI_EvolveMenu_Level::Call_ReturnToEvolveMenu, this);
	}
}

void CUI_EvolveMenu_Level::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

}

void CUI_EvolveMenu_Level::Create_Background()
{
	m_pPauseMenuBackground = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pPauseMenuBackground.lock()->Set_Depth(1.f);
	m_pPauseMenuBackground.lock()->Set_Texture("PauseMenu_Background0");

	m_pPauseMenuBackground_Main = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pPauseMenuBackground_Main.lock()->Set_Depth(0.9f);
	m_pPauseMenuBackground_Main.lock()->Set_Texture("PauseMenu_Background1");

	m_pPauseMenuBackground_Top = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
	m_pPauseMenuBackground_Top.lock()->Set_Depth(0.3f);
	m_pPauseMenuBackground_Top.lock()->Set_Texture("PauseMenu_Background3");


	m_vecChildUI.push_back(m_pPauseMenuBackground);
	m_vecChildUI.push_back(m_pPauseMenuBackground_Main);
	m_vecChildUI.push_back(m_pPauseMenuBackground_Top);
}

void CUI_EvolveMenu_Level::OnEnable(void* pArg)
{
	__super::OnEnable(pArg);

	if (!m_pFadeMask.lock())
		m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();

	GET_SINGLE(CGameManager)->Disable_Layer(OBJECT_LAYER::BATTLEUI);

}

void CUI_EvolveMenu_Level::Call_ReturnToEvolveMenu()
{
	Set_Enable(false);
	GAMEINSTANCE->Get_GameObjects<CUI_EvolveMenu>(LEVEL_STATIC).front().lock()->Set_Enable(true);

}
