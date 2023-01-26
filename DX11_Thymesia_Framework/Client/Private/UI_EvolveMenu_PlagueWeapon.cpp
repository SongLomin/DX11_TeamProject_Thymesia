#include "stdafx.h"
#include "UI_EvolveMenu_PlagueWeapon.h"
#include "PlayerSkillHeader.h"
#include "UI_Utils.h"
#include "CustomUI.h"
#include "UIManager.h"
#include "FadeMask.h"
#include "UI_EvolveMenu_PlagueWeapon_SkillView.h"
#include "UI_EvolveMenu_PlagueWeapon_SkillInformation.h"
#include "UI_EvolveMenu_PlagueWeapon_SkillButton.h"
#include "UI_Effect_MagicCircle.h"
#include "UI_EffectGroup_SkillOpen.h"
#include "GameManager.h"


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
	Init_ChildUI();

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

void CUI_EvolveMenu_PlagueWeapon::Init_ChildUI()
{
	m_pSkillView = GAMEINSTANCE->Add_GameObject<CUI_EvolveMenu_PlagueWeapon_SkillView>(LEVEL_STATIC);
	m_pSkillInformation = GAMEINSTANCE->Add_GameObject<CUI_EvolveMenu_PlagueWeapon_SkillInformation>(LEVEL_STATIC);

	m_pSkillView.lock()->Callback_OnMouseOver += bind(&CUI_EvolveMenu_PlagueWeapon::Call_OnSkillButtonMouseOver, this, placeholders::_1);
	m_pSkillView.lock()->Callback_OnMouseOut += bind(&CUI_EvolveMenu_PlagueWeapon::Call_OnSkillButtonMouseOut, this);
	m_pSkillView.lock()->Callback_OnUnlockSkill += bind(&CUI_EvolveMenu_PlagueWeapon::Call_OnUnlockSkill, this, placeholders::_1);
	m_pSkillView.lock()->Callback_OnLButtonUp += bind(&CUI_EvolveMenu_PlagueWeapon::Call_OnLButtonUp, this, placeholders::_1);

	Add_Child(m_pSkillView);
	Add_Child(m_pSkillInformation);
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

void CUI_EvolveMenu_PlagueWeapon::Call_OnSkillButtonMouseOver(weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkillButton)
{
	m_pSkillInformation.lock()->View_Information(pSkillButton);
}

void CUI_EvolveMenu_PlagueWeapon::Call_OnSkillButtonMouseOut()
{
	m_pSkillInformation.lock()->Clear_Information();
}

void CUI_EvolveMenu_PlagueWeapon::Call_OnUnlockSkill(weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkillButton)
{
	m_pSkillInformation.lock()->Clear_Information();
	m_pSkillInformation.lock()->View_Information(pSkillButton);

	//해금 이펙트 추가
	weak_ptr<CUI_EffectGroup_SkillOpen> pEffectGroup = GET_SINGLE(CGameManager)->
		GetGameObject_SafetyUseMemoryPool< CUI_EffectGroup_SkillOpen>(LEVEL_STATIC);

	pEffectGroup.lock()->Play(pSkillButton.lock()->Get_SkillName());

	weak_ptr<CSkill_Base> pSkill = GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->
		Get_Component<CPlayerSkill_System>().lock()->Find_Skill(pSkillButton.lock()->Get_SkillName());

	if (pSkill.lock())
	{
		pSkill.lock()->Set_Unlock(true);
	}
}

void CUI_EvolveMenu_PlagueWeapon::Call_OnLButtonUp(weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkillButton)
{

}

void CUI_EvolveMenu_PlagueWeapon::Free()
{


}
