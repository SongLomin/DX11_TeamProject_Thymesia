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
#include "UI_PauseMenu_Tap_Potion.h"


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

	m_eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;


	m_pStatusTap = GAMEINSTANCE->Add_GameObject<CUI_PauseMenu_Tap_Status>(LEVEL_STATIC);
	m_pPlagueWeaponTap = GAMEINSTANCE->Add_GameObject<CUI_PauseMenu_Tap_PlagueWeapon>(LEVEL_STATIC);
//	m_pPotionTap = GAMEINSTANCE->Add_GameObject<CUI_PauseMenu_Tap_Potion>(LEVEL_STATIC);

	Add_Child(m_pStatusTap);
	Add_Child(m_pPlagueWeaponTap);
//	Add_Child(m_pPotionTap);

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


HRESULT CUI_PauseMenu_Page_Status::Render(ID3D11DeviceContext* pDeviceContext)
{

	//

	return S_OK;
}
