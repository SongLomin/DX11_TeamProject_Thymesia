#include "stdafx.h"
#include "UI_OptionElementBase.h"

GAMECLASS_C(CUI_OptionElementBase)
CLONE_C(CUI_OptionElementBase, CGameObject)

HRESULT CUI_OptionElementBase::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pLeftArrow = GAMEINSTANCE->Add_GameObject<CUI_Button>(LEVEL_STATIC);

	m_pLeftArrow.lock()->Set_Size(20.f, 50.f);
	m_pLeftArrow.lock()->Set_Depth(0.6f);
	m_pLeftArrow.lock()->Set_Texture("Keyboard_Arrow_Left");

	m_pRightArrow = GAMEINSTANCE->Add_GameObject<CUI_Button>(LEVEL_STATIC);
	m_pRightArrow.lock()->Set_Size(20.f, 50.f);
	m_pRightArrow.lock()->Set_Depth(0.6f);
	m_pRightArrow.lock()->Set_Texture("Keyboard_Arrow_Right");

	m_pLeftArrow.lock()->Callback_OnLButtonClicked += bind(&CUI_OptionElementBase::Call_OnClickLeftArrow, this);
	m_pRightArrow.lock()->Callback_OnLButtonClicked += bind(&CUI_OptionElementBase::Call_OnClickRightArrow, this);

	Add_Child(m_pLeftArrow);
	Add_Child(m_pRightArrow);


	return S_OK;
}

void CUI_OptionElementBase::Call_OnClickLeftArrow()
{

}

void CUI_OptionElementBase::Call_OnClickRightArrow()
{

}

void CUI_OptionElementBase::Set_Y(_float fY)
{
	for (auto& elem : m_vecChildUI)
	{
		elem.lock()->Set_Y(fY);
	}
}

void CUI_OptionElementBase::SetUp_ArrowPos()
{
}


void CUI_OptionElementBase::Free()
{
}
