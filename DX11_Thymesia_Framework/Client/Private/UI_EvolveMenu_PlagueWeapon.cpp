#include "stdafx.h"
#include "UI_EvolveMenu_PlagueWeapon.h"
#include "PlayerSkillHeader.h"
#include "UI_Utils.h"
#include "CustomUI.h"

GAMECLASS_C(CUI_EvolveMenu_PlagueWeapon);
CLONE_C(CUI_EvolveMenu_PlagueWeapon, CGameObject);

HRESULT CUI_EvolveMenu_PlagueWeapon::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CUI_EvolveMenu_PlagueWeapon::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_eRenderGroup = RENDERGROUP::RENDER_UI;

	Init_Backgrounds();

	return S_OK;
}

HRESULT CUI_EvolveMenu_PlagueWeapon::Start()
{
	__super::Start();


	return S_OK;
}

void CUI_EvolveMenu_PlagueWeapon::Tick(_float fTimeDelta)
{
	fTimeDelta = CUI_Utils::UI_TimeDelta();
	__super::Tick(fTimeDelta);

}

void CUI_EvolveMenu_PlagueWeapon::LateTick(_float fTimeDelta)
{
	fTimeDelta = CUI_Utils::UI_TimeDelta();
	__super::LateTick(fTimeDelta);

}

void CUI_EvolveMenu_PlagueWeapon::Init_Backgrounds()
{
	m_pBackgroundBack =		ADD_STATIC_CUSTOMUI;
	m_pBackgroundMain =		ADD_STATIC_CUSTOMUI;
	m_pBackgroundMainFill = ADD_STATIC_CUSTOMUI;
	m_pBackgroundTop =		ADD_STATIC_CUSTOMUI;


	m_pBackgroundBack.lock()->Set_Texture("PauseMenu_Background0");
	m_pBackgroundMain.lock()->Set_Texture("PauseMenu_Background1");
	m_pBackgroundMainFill.lock()->Set_Texture("PauseMenu_Background2");
	m_pBackgroundTop.lock()->Set_Texture("PauseMenu_Background3");


	m_pBackgroundBack.lock()->Set_Size(g_iWinCX, g_iWinCY);
	m_pBackgroundMain.lock()->Set_Size(g_iWinCX, g_iWinCY);
	m_pBackgroundMainFill.lock()->Set_Size(g_iWinCX, g_iWinCY);
	m_pBackgroundTop.lock()->Set_Size(g_iWinCX, g_iWinCY);


	m_pBackgroundBack.lock()->Set_AlphaColor(0.9f);
	m_pBackgroundMain .lock()->Set_AlphaColor(0.85f);
	m_pBackgroundMainFill.lock()->Set_AlphaColor(0.8f);
	m_pBackgroundTop.lock()->Set_AlphaColor(0.75f);


	Add_Child(m_pBackgroundBack);
	Add_Child(m_pBackgroundMain);
	Add_Child(m_pBackgroundMainFill);
	Add_Child(m_pBackgroundTop);

}
