#include "stdafx.h"


GAMECLASS_C(CUI_EvolveMenu_TalentButton)
CLONE_C(CUI_EvolveMenu_TalentButton, CGameObject)

HRESULT CUI_EvolveMenu_TalentButton::Initialize(void* pArg)
{
	
}

HRESULT CUI_EvolveMenu_TalentButton::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_EvolveMenu_TalentButton::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Update_Child();
}

void CUI_EvolveMenu_TalentButton::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CUI_EvolveMenu_TalentButton::Update_Child()
{
	for (auto& elem : m_vecChildUI)//자식들은 버튼의 알파를 따라감.
	{
		elem.lock()->Set_UIPosition
		(
			m_tUIDesc.fX,	
			m_tUIDesc.fY
		);
		elem.lock()->Set_AlphaColor(m_fAlphaColor);
	}
}

void CUI_EvolveMenu_TalentButton::OnMouseOver()
{
	if (!m_bActive)
	{
	

		m_pButtonActive.lock()->Set_Texture("EvolveMenu_PW_Frame_Hover");
	}
	else
	{
		m_pButtonActive.lock()->Set_Texture("EvolveMenu_PW_Active_Hover");
	}

}

void CUI_EvolveMenu_TalentButton::OnMouseOut()
{
	if (!m_bActive)
	{
		m_pButtonActive.lock()->Set_Texture("EvolveMenu_PW_None");
	}
	else
	{
		m_pButtonActive.lock()->Set_Texture("EvolveMenu_PW_Active");
	}
}

void CUI_EvolveMenu_TalentButton::OnLButtonDown()
{	

}

void CUI_EvolveMenu_TalentButton::OnLButtonClick()
{
	
	m_bActive = Get_Togle();
	if (m_bActive)
	{
		m_pButtonFrame.lock()->Set_Texture("EvolveMenu_PW_Frame_Active");
		m_pButtonActive.lock()->Set_Texture("EvolveMenu_PW_Active");
	}
	else
	{
		m_pButtonFrame.lock()->Set_Texture("EvolveMenu_PW_Frame");
		m_pButtonActive.lock()->Set_Texture("EvolveMenu_PW_None");
	}
}

void CUI_EvolveMenu_TalentButton::OnEnable(void* pArg)
{
	__super::OnEnable(pArg);
}
