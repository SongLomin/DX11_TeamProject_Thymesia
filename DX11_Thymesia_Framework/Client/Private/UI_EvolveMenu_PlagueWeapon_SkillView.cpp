#include "stdafx.h"
#include "UI_EvolveMenu_PlagueWeapon_SkillView.h"
#include "PlayerSkillHeader.h"
#include "UIManager.h"
#include "UI_Utils.h"
#include "CustomUI.h"
#include "UI_EvolveMenu_PlagueWeapon_SkillButton.h"


GAMECLASS_C(CUI_EvolveMenu_PlagueWeapon_SkillView)
CLONE_C(CUI_EvolveMenu_PlagueWeapon_SkillView, CGameObject)


HRESULT CUI_EvolveMenu_PlagueWeapon_SkillView::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CUI_EvolveMenu_PlagueWeapon_SkillView::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_eRenderGroup = RENDERGROUP::RENDER_UI;

	SetUp_UI();
	SetUp_SkillPos();
	SetUp_SkillButtons();
	
	return S_OK;
}

HRESULT CUI_EvolveMenu_PlagueWeapon_SkillView::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_EvolveMenu_PlagueWeapon_SkillView::Tick(_float fTimeDelta)
{
	fTimeDelta = CUI_Utils::UI_TimeDelta();


	__super::Tick(fTimeDelta);

}

void CUI_EvolveMenu_PlagueWeapon_SkillView::LateTick(_float fTimeDelta)
{
	fTimeDelta = CUI_Utils::UI_TimeDelta();
	__super::LateTick(fTimeDelta);
}

void CUI_EvolveMenu_PlagueWeapon_SkillView::SetUp_SkillPos()
{
	_float2 fStartPos;
	_float	fOffsetX = 175.f;
	fStartPos = { 221.f, 267.f };

	for (_uint i = 0; i < 3; i++)
	{
		m_vecSkillPos.push_back(_float2(fStartPos.x + i * fOffsetX, fStartPos.y));
	}

	fStartPos = { 150.f, 354.f};
	for (_uint i = 0; i < 4; i++)
	{
		m_vecSkillPos.push_back(_float2(fStartPos.x + i * fOffsetX, fStartPos.y));
	}

	fStartPos = { 221.f, 441.f};
	for (_uint i = 0; i < 3; i++)
	{
		m_vecSkillPos.push_back(_float2(fStartPos.x + i * fOffsetX, fStartPos.y));
	}
	fStartPos = { 306.f, 528.f };
	for (_uint i = 0; i < 2; i++)
	{
		m_vecSkillPos.push_back(_float2(fStartPos.x + i * fOffsetX, fStartPos.y));
	}
}

void CUI_EvolveMenu_PlagueWeapon_SkillView::SetUp_UI()
{
	EvolveMenu_SkillView_Frame = ADD_STATIC_CUSTOMUI;

	EvolveMenu_SkillView_Frame.lock()->Set_UIPosition
	(
		84.f,
		150.f,
		624.f,
		600.f,
		CUI::ALIGN_LEFTTOP
	);
	EvolveMenu_SkillView_Frame.lock()->Set_Depth(0.3f);
	EvolveMenu_SkillView_Frame.lock()->Set_Texture("EvolveMenu_SkillView_Frame");

	Add_Child(EvolveMenu_SkillView_Frame);
}

void CUI_EvolveMenu_PlagueWeapon_SkillView::SetUp_SkillButtons()
{
	weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton>	pButton;

	for(_uint i = 0 ; i < (_uint)SKILL_NAME::SKILL_END;i++)
	{
		pButton = GAMEINSTANCE->Add_GameObject<CUI_EvolveMenu_PlagueWeapon_SkillButton>(LEVEL_STATIC);
		pButton.lock()->Set_Skill((SKILL_NAME)i);

		pButton.lock()->Set_UIPosition(m_vecSkillPos[i].x, m_vecSkillPos[i].y);

		pButton.lock();

		m_vecSkillButton.push_back(pButton);
	}
}

void CUI_EvolveMenu_PlagueWeapon_SkillView::OnEnable(void* pArg)
{
	__super::OnEnable(pArg);

	for (auto& elem : m_vecSkillButton)
	{
		elem.lock()->Set_Enable(true);
	}

}

void CUI_EvolveMenu_PlagueWeapon_SkillView::OnDisable()
{
	__super::OnDisable();

	for (auto& elem : m_vecSkillButton)
	{
		elem.lock()->Set_Enable(false);
	}
}

void CUI_EvolveMenu_PlagueWeapon_SkillView::Bind_Callback(weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton> pButton)
{
	pButton.lock()->Callback_MouseOver += bind(&CUI_EvolveMenu_PlagueWeapon_SkillView::Call_OnMouseOver, this,
		placeholders::_1);

	pButton.lock()->Callback_MouseOut += bind(&CUI_EvolveMenu_PlagueWeapon_SkillView::Call_OnMouseOut, this);

}

void CUI_EvolveMenu_PlagueWeapon_SkillView::Call_OnMouseOver(weak_ptr<CUI_EvolveMenu_PlagueWeapon_SkillButton> pSkillButton)
{
	Callback_OnMouseOver(pSkillButton);
}

void CUI_EvolveMenu_PlagueWeapon_SkillView::Call_OnMouseOut()
{
	Callback_OnMouseOut();
}

void CUI_EvolveMenu_PlagueWeapon_SkillView::Free()
{

}
