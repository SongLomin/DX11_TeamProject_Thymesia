#include "stdafx.h"
#include "EvolveMenu_TalentButton.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "FadeMask.h"
#include "Fader.h"
#include "GameManager.h"
#include "Client_GameObjects.h"
#include "CustomUI.h"

GAMECLASS_C(CEvolveMenu_TalentButton)
CLONE_C(CEvolveMenu_TalentButton, CGameObject)

HRESULT CEvolveMenu_TalentButton::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	Set_UIPosition(g_iWinCX >> 1, g_iWinCY >> 1, 80.f, 80.f);

	m_pButtonFrame = ADD_STATIC_CUSTOMUI;

	m_pButtonFrame.lock()->Set_UIPosition(g_iWinCX >> 1, g_iWinCY >> 1, 80.f, 80.f);
	m_pButtonFrame.lock()->Set_Texture("EvolveMenu_PW_Frame");
	m_pButtonFrame.lock()->Set_Depth(0.2f);


	m_pButtonActive = ADD_STATIC_CUSTOMUI;

	m_pButtonActive.lock()->Set_UIPosition
	(
		g_iWinCX >> 1, g_iWinCY >> 1, 
		180.f, 180.f
	);
	m_pButtonActive.lock()->Set_Texture("EvolveMenu_PW_None");
	m_pButtonActive.lock()->Set_Depth(0.3f);

	m_bActive = false;


	Add_Child(m_pButtonFrame);
	Add_Child(m_pButtonActive);


	return S_OK;
}

HRESULT CEvolveMenu_TalentButton::Start()
{
	__super::Start();

	return S_OK;
}

void CEvolveMenu_TalentButton::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Update_Child();
}

void CEvolveMenu_TalentButton::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CEvolveMenu_TalentButton::Update_Child()
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

void CEvolveMenu_TalentButton::OnMouseOver()
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

void CEvolveMenu_TalentButton::OnMouseOut()
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

void CEvolveMenu_TalentButton::OnLButtonDown()
{	

}

void CEvolveMenu_TalentButton::OnLButtonClick()
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

void CEvolveMenu_TalentButton::OnEnable(void* pArg)
{
	__super::OnEnable(pArg);
}
