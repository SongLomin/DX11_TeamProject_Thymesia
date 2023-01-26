#include "stdafx.h"
#include "UI_Scroll.h"
#include "UI_Utils.h"
#include "CustomUI.h"
#include "GameManager.h"
#include "UIManager.h"

GAMECLASS_C(CUI_Scroll)
CLONE_C(CUI_Scroll, CGameObject)


HRESULT CUI_Scroll::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    CreateScroll();

    Set_UIPosition(0.f, 0.f ,0.f, 0.f);

    return S_OK;
}

HRESULT CUI_Scroll::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_Scroll::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::Tick(fTimeDelta);

    if (m_bScrollAble)
    {
        if (CheckMoveWheelCurrentTick())
        {
            Scrolling(fTimeDelta);
            CheckTrackOverHeadOrTail();
            CalcScrolledTrackToMaxSize();
            Callback_OnWheelMove(m_fMaxSize * m_fProgressRatio);
        }

        if (Check_Drag())
        {
            ScrollingToDrag();
            CheckTrackOverHeadOrTail();
            CalcScrolledTrackToMaxSize();
            Callback_OnWheelMove(m_fMaxSize * m_fProgressRatio);
        }

    }

}

void CUI_Scroll::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);


}

void CUI_Scroll::CreateScroll()
{
    m_pDecorationHead = ADD_STATIC_CUSTOMUI;
    m_pDecorationHead.lock()->Set_Texture("Scroll_Head");
    m_pDecorationHead.lock()->Set_Size(11.f, 5.f);

    m_pDecorationTail = ADD_STATIC_CUSTOMUI;
    m_pDecorationTail.lock()->Set_Texture("Scroll_Tail");
    m_pDecorationTail.lock()->Set_Size(11.f, 5.f);

    m_pScrollBoder = ADD_STATIC_CUSTOMUI;
    m_pScrollBoder.lock()->Set_Texture("Scroll_Border");


    m_pScrollTrack = ADD_STATIC_CUSTOMUI;
    m_pScrollTrack.lock()->Set_Texture("Scroll_Track");

    Add_Child(m_pDecorationHead);
    Add_Child(m_pDecorationTail);
    Add_Child(m_pScrollBoder);
    Add_Child(m_pScrollTrack);

}


void CUI_Scroll::Scrolling(_float fTimeDelta)
{
    _float fAmount = GAMEINSTANCE->Get_DIMouseMoveState(MMS_WHEEL) * fTimeDelta;

    m_pScrollTrack.lock()->Add_Y(-fAmount);

}

void CUI_Scroll::Up_Scroll(_float fUpAmount)
{
    m_pScrollTrack.lock()->Add_Y(fUpAmount);

    CheckTrackOverHeadOrTail();
    CalcScrolledTrackToMaxSize();
    Callback_OnWheelMove(m_fMaxSize * m_fProgressRatio);
}

void CUI_Scroll::Down_Scroll(_float fDownAmount)
{
    m_pScrollTrack.lock()->Add_Y(-fDownAmount);

    CheckTrackOverHeadOrTail();
    CalcScrolledTrackToMaxSize();
    Callback_OnWheelMove(m_fMaxSize * m_fProgressRatio);
}

void CUI_Scroll::CheckTrackOverHeadOrTail()
{
    _float m_pScrollTrackTop = m_pScrollTrack.lock()->Get_Point(UI_POINT::TOP).y;
    _float m_pHeadBottom = m_pDecorationHead.lock()->Get_Point(UI_POINT::BOTTOM).y;

    _float m_pScrollTrackBottom = m_pScrollTrack.lock()->Get_Point(UI_POINT::BOTTOM).y;
    _float m_TailTop = m_pDecorationTail.lock()->Get_Point(UI_POINT::TOP).y;

    if (m_pScrollTrackTop < m_pHeadBottom)
    {
        m_pScrollTrack.lock()->Set_Y(m_pHeadBottom + (m_pScrollTrack.lock()->Get_UIDESC().fSizeY * 0.5f));
    }

    if (m_pScrollTrackBottom > m_TailTop)
    {
        m_pScrollTrack.lock()->Set_Y(m_TailTop - (m_pScrollTrack.lock()->Get_UIDESC().fSizeY  * 0.5f));
    }
}

void CUI_Scroll::CalcScrolledTrackToMaxSize()
{
    _float fRatio = 0.f;
    _float  fScrollMaxSize = m_pScrollBoder.lock()->Get_SizeY() - m_pScrollTrack.lock()->Get_SizeY();
    _float  fTrackMovement = m_pScrollTrack.lock()->Get_Y() - m_pDecorationHead.lock()->Get_Point(CUI::UI_POINT::LEFT_BOTTOM).y - 
                    m_pScrollTrack.lock()->Get_SizeY() * 0.5f;

    m_fProgressRatio = fTrackMovement / fScrollMaxSize;//순수 
}

_bool CUI_Scroll::CheckMoveWheelCurrentTick()
{
    _long  fWheelMovement = abs(GAMEINSTANCE->Get_DIMouseMoveState(MMS_WHEEL)) * 7;

    if (fWheelMovement > 0)
    {
        return true;
    }
    
    return false;
}

_bool CUI_Scroll::Check_Drag()
{
    /*
        스크롤 범위 내에 있으면서,
        스크롤을 누르고 있다면 트루 반환
    */

    if (m_eButtonState == UI_BUTTON_BUTTON_DOWN)
    {
        return true;
    }

    return false;
}

void CUI_Scroll::ScrollingToDrag()
{
    POINT   tMousePt = GET_SINGLE(CUIManager)->Get_MousePoint();

    m_pScrollTrack.lock()->Set_Y((_float)tMousePt.y);

}

void CUI_Scroll::Reset_Scroll()
{
    _float m_pHeadBottom = m_pDecorationHead.lock()->Get_Point(UI_POINT::BOTTOM).y;
    m_pScrollTrack.lock()->Set_Y(m_pHeadBottom + (m_pScrollTrack.lock()->Get_UIDESC().fSizeY * 0.5f));
    CheckTrackOverHeadOrTail();
    CalcScrolledTrackToMaxSize();
    Callback_OnWheelMove(m_fMaxSize * m_fProgressRatio);
}

void CUI_Scroll::SetUp_ScrollFromLeftTop(const _float fX, const _float fY, _float fSizeX, const _float fScrollSize, const _float fStartSize, const _float fMaxSize)
{
    m_pDecorationHead.lock()->Set_UIPosition(fX, fY, fSizeX, 5.f, CUI::ALIGN_LEFTTOP);

    _float2  fHeadLeftBottomPos = m_pDecorationHead.lock()->Get_Point(CUI::UI_POINT::LEFT_BOTTOM);

    //StartSize : 처음 화면에 보이는 크기.
    _float  fTrackSize = (fStartSize / fMaxSize) * fScrollSize;

    m_fMaxSize = fMaxSize;

   
    m_pScrollBoder.lock()->Set_UIPosition
    (
        fX,
        fHeadLeftBottomPos.y,
        fSizeX,
        fScrollSize,
        CUI::ALIGN_LEFTTOP
    );

    m_pScrollTrack.lock()->Set_UIPosition
    (
        fX,
        fHeadLeftBottomPos.y,
        fSizeX,
        fTrackSize,
        CUI::ALIGN_LEFTTOP
    );

    m_pDecorationTail.lock()->Set_UIPosition
    (   fX,
        m_pScrollBoder.lock()->Get_Point(CUI::UI_POINT::LEFT_BOTTOM).y,
        fSizeX,
        5.f,
        CUI::ALIGN_LEFTTOP
    );

    m_tUIDesc = m_pScrollBoder.lock()->Get_UIDESC();
    m_tUIDesc.fSizeX = fSizeX;
}

void CUI_Scroll::Set_Depth(_float fDepth)
{
    __super::Set_Depth(fDepth);

    for (auto& elem : m_vecChildUI)
    {
        elem.lock()->Set_Depth(fDepth);
    }
    m_pScrollTrack.lock()->Set_Depth(fDepth - 0.1f);
}
