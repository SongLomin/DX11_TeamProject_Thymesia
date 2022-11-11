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
