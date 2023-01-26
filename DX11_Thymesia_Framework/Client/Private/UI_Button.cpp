#include "stdafx.h"
#include "UI_Button.h"
#include "GameManager.h"
#include "UIManager.h"

GAMECLASS_C(CUI_Button)
CLONE_C(CUI_Button, CGameObject)

HRESULT CUI_Button::Initialize(void* pArg)
{
    __super::Initialize(pArg);

	m_bClickToggle = false;
	m_eButtonState = UI_BUTTON_OUT;

    return S_OK;
}

HRESULT CUI_Button::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_Button::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

	if (m_eButtonLevel != GET_SINGLE(CUIManager)->Get_ButtonLevel())
	{
		return;
	}
	POINT	pt = GET_SINGLE(CUIManager)->Get_MousePoint();
	
	Update_MouseOver();

	if (m_eButtonState == UI_BUTTON_OUT)
		return;

	Update_LButtonDown();
	Update_LClick();
}

void CUI_Button::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
	
}

void CUI_Button::Set_ButtonLevel(BUTTON_LEVEL eButtonLevel)
{
	m_eButtonLevel = eButtonLevel;
}

void CUI_Button::Update_MouseOver()
{
	POINT		pt;

	pt = GET_SINGLE(CUIManager)->Get_MousePoint();

	if (pt.x < ((m_tUIDesc.fX) - (m_tUIDesc.fSizeX * 0.5f)) ||
		pt.x >((m_tUIDesc.fX) + (m_tUIDesc.fSizeX * 0.5f)) ||
		pt.y < ((m_tUIDesc.fY) - (m_tUIDesc.fSizeY * 0.5f)) ||
		pt.y > ((m_tUIDesc.fY) + (m_tUIDesc.fSizeY * 0.5f))
		)
	{
		if (m_eButtonState != UI_BUTTON_OUT)//이제막 나가진 경우 
		{
			Callback_OnMouseOut();
			OnMouseOut();
			m_eButtonState = UI_BUTTON_OUT;
		}		
	}
	else 
	{
		if (m_eButtonState == UI_BUTTON_OUT)//이제 막 들어온 경우
		{
			Callback_OnMouseOver();
			OnMouseOver();
			m_eButtonState = UI_BUTTON_OVER;
		}
	}
}

void CUI_Button::Update_LButtonDown()
{
	if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::TAP))
	{
		if (m_eButtonState != UI_BUTTON_BUTTON_DOWN)
		{
			Callback_OnLButtonDown();
			OnLButtonDown();
		}
		m_eButtonState = UI_BUTTON_BUTTON_DOWN;
	}
}

void CUI_Button::Update_LClick()
{
	if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::AWAY))
	{
		OnLButtonUp();
		if (m_eButtonState != UI_BUTTON_OUT)
		{
			Callback_OnLButtonClicked();
			OnLButtonClick();
			m_bClickToggle = !m_bClickToggle;
			m_eButtonState = UI_BUTTON_OVER;
		}
	}
}

void CUI_Button::OnMouseOver()
{
}

void CUI_Button::OnMouseOut()
{
}

void CUI_Button::OnLButtonDown()
{
}

void CUI_Button::OnLButtonUp()
{

}


void CUI_Button::OnLButtonClick()
{
}

void CUI_Button::CheckMouseOver()
{
}

void CUI_Button::CheckMouseOut()
{
}

void CUI_Button::CheckLButtonDown()
{
}

void CUI_Button::CheckLButtonClick()
{
}

void CUI_Button::UnCheckMouseOver()
{
}

void CUI_Button::UnCheckMouseOut()
{
}

void CUI_Button::UnCheckLButtonDown()
{
}

void CUI_Button::UnCheckLButtonClick()
{
}



