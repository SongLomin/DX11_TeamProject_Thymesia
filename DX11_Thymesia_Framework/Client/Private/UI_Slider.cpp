#include "stdafx.h"
#include "UI_Slider.h"
#include "UI_Utils.h"
#include "CustomUI.h"
#include "UIManager.h"
#include "ProgressBar.h"

GAMECLASS_C(CUI_Slider)
CLONE_C(CUI_Slider, CGameObject)


HRESULT CUI_Slider::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    Set_UIPosition
    (
        363.f,
        225.f,
        230.f,
        45.f,
        ALIGN_LEFTTOP
    );

    m_pBG = GAMEINSTANCE->Add_GameObject< CProgressBar>(LEVEL_STATIC);
    m_pFill = GAMEINSTANCE->Add_GameObject< CProgressBar>(LEVEL_STATIC);
    m_pTrack = ADD_STATIC_CUSTOMUI;

    m_pBG.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY);
    m_pBG.lock()->Set_Size(230.f, 6.f);

    m_pFill.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY);
    m_pFill.lock()->Set_Size(230.f, 6.f);

    m_pTrack.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY);
    m_pTrack.lock()->Set_Size(3.f, 26.f);

    m_pBG.lock()->Set_Depth(0.5);
    m_pFill.lock()->Set_Depth(0.49);
    m_pTrack.lock()->Set_Depth(0.48);

    m_pBG.lock()->Set_Texture("UI_Slider_BG");
    m_pFill.lock()->Set_Texture("UI_Slider_Fill");
    m_pTrack.lock()->Set_Texture("UI_Slider_Track");

    Add_Child(m_pBG);
    Add_Child(m_pFill);
    Add_Child(m_pTrack);
    

    SetToAmount();

    return S_OK;
}

HRESULT CUI_Slider::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_Slider::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();
    __super::Tick(fTimeDelta);

    if (m_bDragAble)
    {
        if (Check_Drag())
        {
            CalcAmount();
            CheckAmount();
            SetToAmount();
            Callback_ChangeSliderAmount(m_iAmount);
        }

        for (auto& elem : m_vecChildUI)
        {
            elem.lock()->Set_Y(m_tUIDesc.fY);
        }
    }
}

void CUI_Slider::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();
    __super::LateTick(fTimeDelta);
}

void CUI_Slider::OnLButtonDown()
{
    __super::OnLButtonDown();
    
    CalcAmount();
    CheckAmount();
    SetToAmount();

    Callback_ChangeSliderAmount(m_iAmount);

    m_bDragAble = true;
}

void CUI_Slider::OnMouseOut()
{
    __super::OnMouseOut();

    m_bDragAble = false;
}

void CUI_Slider::Set_Y(_float fY)
{
    __super::Set_Y(fY);

    for (auto& elem : m_vecChildUI)
    {
        elem.lock()->Set_Y(fY);
    }
}

void CUI_Slider::IncreaseAmount(_int iAmount)
{
    m_iAmount += iAmount;
   
    CheckAmount();
    SetToAmount();

    Callback_ChangeSliderAmount(m_iAmount);
}

void CUI_Slider::DecreaseAmount(_int iAmount)
{
    m_iAmount -= iAmount;
    CheckAmount();
    SetToAmount();

    Callback_ChangeSliderAmount(m_iAmount);
}

_bool CUI_Slider::Check_Drag()
{
    if (m_eButtonState == UI_BUTTON_BUTTON_DOWN)
    {
        if (abs(GAMEINSTANCE->Get_DIMouseMoveState(MMS_X)) > 0)
        {
            return true;
        }
        return false;
    }
    return false;
}

void CUI_Slider::CalcAmount()
{
    POINT   mousePt = GET_SINGLE(CUIManager)->Get_MousePoint();
    //마우스 PT받아와서
    
    /*
        비율 가져오기
    */
    _float2      fStart = m_pBG.lock()->Get_Point(CUI::UI_POINT::LEFT);
    _float      fSize = m_pBG.lock()->Get_Size().x;

    _float      fRatio = (mousePt.x - fStart.x) / fSize;

    m_iAmount = fRatio * m_iMaxAmount;
  
}

void CUI_Slider::CheckAmount()
{
    if (m_iAmount < 0)
    {
        m_iAmount = 0;
    }
    else if (m_iAmount > m_iMaxAmount)
    {
        m_iAmount = m_iMaxAmount;
    }
}

void CUI_Slider::SetToAmount()
{
    _float fRatio = ((_float)(m_iAmount) / (_float)m_iMaxAmount);

    m_pFill.lock()->Set_Ratio(fRatio);

    _float2      fStart = m_pBG.lock()->Get_Point(CUI::UI_POINT::LEFT);
    _float      fSize = m_pBG.lock()->Get_Size().x;

    m_pTrack.lock()->Set_X(fStart.x + (fSize * fRatio));
}

void CUI_Slider::Free()
{

}
