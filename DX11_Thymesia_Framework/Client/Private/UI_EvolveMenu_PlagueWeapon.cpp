#include "stdafx.h"
#include "UI_EvolveMenu_PlagueWeapon.h"
#include "PlayerSkillHeader.h"
#include "UI_Utils.h"
#include "CustomUI.h"
#include "UIManager.h"
#include "FadeMask.h"
#include "UI_EvolveMenu_PlagueWeapon_SkillView.h"

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
	Init_LeftSkillView();

	m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();
	//시작시 꺼준다.
	Set_Enable(false);

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
	m_pTitle =				ADD_STATIC_CUSTOMUI;

	m_pBackgroundBack.lock()->Set_Texture("PauseMenu_Background0");
	m_pBackgroundMain.lock()->Set_Texture("PauseMenu_Background1");
	m_pBackgroundMainFill.lock()->Set_Texture("PauseMenu_Background2");
	m_pBackgroundTop.lock()->Set_Texture("PauseMenu_Background3");
	m_pTitle.lock()->Set_Texture("EvolveMenu_Skill_Title");

	m_pBackgroundMain.lock()->Set_Size(g_iWinCX, g_iWinCY);
	m_pBackgroundMainFill.lock()->Set_Size(g_iWinCX, g_iWinCY);
	m_pBackgroundTop.lock()->Set_Size(g_iWinCX, g_iWinCY);
	
	m_pTitle.lock()->Set_UIPosition
	(
		84.f,
		24.f,
		220.f,
		79.f,
		CUI::ALIGN_LEFTTOP
	);
	
	m_pBackgroundBack.lock()->Set_Depth(0.9f);
	m_pBackgroundMain .lock()->Set_Depth(0.85f);
	m_pBackgroundMainFill.lock()->Set_Depth(0.8f);
	m_pBackgroundTop.lock()->Set_Depth(0.75f);
	m_pTitle.lock()->Set_Depth(0.70f);

	Add_Child(m_pBackgroundBack);
	Add_Child(m_pBackgroundMain);
	Add_Child(m_pBackgroundMainFill);
	Add_Child(m_pBackgroundTop);
	Add_Child(m_pTitle);
}

void CUI_EvolveMenu_PlagueWeapon::Init_LeftSkillView()
{
	m_pSkillView = GAMEINSTANCE->Add_GameObject<CUI_EvolveMenu_PlagueWeapon_SkillView>(LEVEL_STATIC);

	Add_Child(m_pSkillView);
}

void CUI_EvolveMenu_PlagueWeapon::OnEnable(void* pArg)
{
	__super::OnEnable(pArg);

	GET_SINGLE(CUIManager)->EnableCursor();
}

void CUI_EvolveMenu_PlagueWeapon::OnDisable()
{
	__super::OnDisable();

	GET_SINGLE(CUIManager)->DisableCursor();
}

void CUI_EvolveMenu_PlagueWeapon::Free()
{


}
