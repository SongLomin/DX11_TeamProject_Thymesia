#include "stdafx.h"
#include "UI_PauseMenu_Page_Status.h"
#include "UI_PauseMenu_Tap_Status.h"
#include "CustomUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "GameObject.h"
#include "UI_PauseMenu_Tap_PlagueWeapon.h"

GAMECLASS_C(CUI_PauseMenu_Page_Status)
CLONE_C(CUI_PauseMenu_Page_Status, CGameObject)

HRESULT CUI_PauseMenu_Page_Status::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CUI_PauseMenu_Page_Status::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	m_pStatusTap = GAMEINSTANCE->Add_GameObject<CUI_PauseMenu_Tap_Status>(LEVEL_STATIC);
	m_pPlagueWeaponTap = GAMEINSTANCE->Add_GameObject<CUI_PauseMenu_Tap_PlagueWeapon>(LEVEL_STATIC);

	m_vecChildUI.push_back(m_pStatusTap);
	m_vecChildUI.push_back(m_pPlagueWeaponTap);
	return S_OK;
}

HRESULT CUI_PauseMenu_Page_Status::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_PauseMenu_Page_Status::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_PauseMenu_Page_Status::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}


HRESULT CUI_PauseMenu_Page_Status::Render()
{

	//

	return S_OK;
}
