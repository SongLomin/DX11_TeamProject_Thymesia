#include "stdafx.h"
#include "UI_Button.h"
#include "GameManager.h"
#include "UIManager.h"

HRESULT CUI_Button_Base::Initialize(void* pArg)
{
    __super::Initialize(pArg);

	m_bClickToggle = false;
	m_eButtonState = UI_BUTTON_OUT;

    return S_OK;
}

HRESULT CUI_Button_Base::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_Button_Base::Tick(_float fTimeDelta)
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

void CUI_Button_Base::LateTick(_float fTimeDelta)
{
    
    __super::LateTick(fTimeDelta);
	
}

void CUI_Button_Base::Set_ButtonLevel(BUTTON_LEVEL eButtonLevel)
{
	m_eButtonLevel = eButtonLevel;
}

void CUI_Button_Base::Update_MouseOver()
{
	POINT		pt;

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

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

void CUI_Button_Base::Update_LButtonDown()
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

void CUI_Button_Base::Update_LClick()
{
	if (KEY_INPUT(KEY::LBUTTON, KEY_STATE::AWAY))
	{
		OnLButtonUp();
		if (m_eButtonState != UI_BUTTON_OUT)
		{
			Callback_OnLButtonClicked();
			OnLButtonClick();
			m_bClickToggle = !m_bClickToggle;
			m_eButtonState = UI_BUTTON_OUT;
		}
	}
}

void CUI_Button_Base::OnMouseOver()
{
}

void CUI_Button_Base::OnMouseOut()
{
}

void CUI_Button_Base::OnLButtonDown()
{
}

void CUI_Button_Base::OnLButtonUp()
{

}


void CUI_Button_Base::OnLButtonClick()
{
}

void CUI_Button_Base::CheckMouseOver()
{
}

void CUI_Button_Base::CheckMouseOut()
{
}

void CUI_Button_Base::CheckLButtonDown()
{
}

void CUI_Button_Base::CheckLButtonClick()
{
}

void CUI_Button_Base::UnCheckMouseOver()
{
}

void CUI_Button_Base::UnCheckMouseOut()
{
}

void CUI_Button_Base::UnCheckLButtonDown()
{
}

void CUI_Button_Base::UnCheckLButtonClick()
{
}



