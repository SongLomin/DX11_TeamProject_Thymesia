#pragma once
#include "UI_Container.h"
#include "UI_Button.h"


BEGIN(Client)

class CCustomUI;

class CUI_Scroll : public CUI_Button
{
	GAMECLASS_H(CUI_Scroll)
	CLONE_H(CUI_Scroll, CGameObject)

public:
    virtual HRESULT Initialize(void* pArg);
    virtual HRESULT Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;

public:
    void                Reset_Scroll();
    void                SetUp_ScrollFromLeftTop(const _float fX, const _float fY, _float fSizeX, const _float fScrollSize,
        const _float fStartSize, const _float fMaxSize);

    virtual void        Set_Depth(_float fDepth) override;
    _float              Get_OffsetYFromRatio() {return (m_fMaxSize * m_fProgressRatio); }

    _float              Get_CurrentProgressiveOffset() { return m_fMaxSize* m_fProgressRatio; }

public:
    void                Call_BindScrollable(_bool bScrollable) {
                        m_bScrollAble = bScrollable;
    }
public:
        FDelegate<_float>         Callback_OnWheelMove;

private:
    void                CreateScroll();
    void                Scrolling(_float fTimeDelta);
    void                Up_Scroll(_float fUpAmount);
    void                Down_Scroll(_float fDownAmount);



    void                CheckTrackOverHeadOrTail();
    void                CalcScrolledTrackToMaxSize();
    _bool               CheckMoveWheelCurrentTick();


private://Scroll Drag   
    _bool                Check_Drag();
    void                ScrollingToDrag();


private:
    _float              m_fScreenAmount;

private:
    weak_ptr<CCustomUI> m_pDecorationHead;
    weak_ptr<CCustomUI> m_pDecorationTail;

    weak_ptr<CCustomUI> m_pScrollBoder;
    weak_ptr<CCustomUI> m_pScrollTrack;

private:
    _float              m_fMaxSize;
    _float              m_fProgressRatio = 0.f;
    _bool               m_bScrollAble = true;

    _float              m_fSensitivity = 1.f;


};

END