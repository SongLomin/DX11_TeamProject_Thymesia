#include "stdafx.h"
#include "UI_Scroll.h"
#include "UI_Utils.h"
#include "CustomUI.h"

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

    CheckTrackOverHeadOrTail();
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

    m_fProgressRatio = fTrackMovement / fScrollMaxSize;//���� 
}

_bool CUI_Scroll::CheckMoveWheelCurrentTick()
{
    _float  fWheelMovement =  fabsf(GAMEINSTANCE->Get_DIMouseMoveState(MMS_WHEEL));

    if (fWheelMovement > 0.0f)
    {
        return true;
    }
    
    return false;
}



void CUI_Scroll::SetUp_ScrollFromLeftTop(const _float fX, const _float fY, const _float fScrollSize, const _float fStartSize, const _float fMaxSize)
{
    m_pDecorationHead.lock()->Set_UIPosition(fX, fY, 11.f, 5.f, CUI::ALIGN_LEFTTOP);

    _float2  fHeadLeftBottomPos = m_pDecorationHead.lock()->Get_Point(CUI::UI_POINT::LEFT_BOTTOM);

    _float  fTrackSize = (fStartSize / fMaxSize) * fScrollSize;

    m_fMaxSize = fMaxSize;

   
    m_pScrollBoder.lock()->Set_UIPosition
    (
        fX,
        fHeadLeftBottomPos.y,
        11.f,
        fScrollSize,
        CUI::ALIGN_LEFTTOP
    );

    m_pScrollTrack.lock()->Set_UIPosition
    (
        fX,
        fHeadLeftBottomPos.y,
        11.f,
        fTrackSize,
        CUI::ALIGN_LEFTTOP
    );

    m_pDecorationTail.lock()->Set_UIPosition
    (fX,
        m_pScrollBoder.lock()->Get_Point(CUI::UI_POINT::LEFT_BOTTOM).y,
        11.f,
        5.f,
        CUI::ALIGN_LEFTTOP
    );

    m_tUIDesc = m_pScrollBoder.lock()->Get_UIDESC();
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
