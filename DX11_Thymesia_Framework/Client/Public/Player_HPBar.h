#pragma once
#include "UI_LerpBar.h"

BEGIN(Client)

class CProgressBar;
class CCustomUI;
class CHUD_Hover;

class CPlayer_HPBar final : public CUI_LerpBar
{
    GAMECLASS_H(CPlayer_HPBar);
    CLONE_H(CPlayer_HPBar, CGameObject);

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual HRESULT Start();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void OnEventMessage(_uint iArg) override;

public:
    void      Set_CurrentHp(_float   _fCurrentHp, _bool bLerp = false,
        EASING_TYPE eLerpType = EASING_TYPE::QUAD_IN);
    void      Set_MaxHp(_float   _fMaxHp) { m_fMaxHp = _fMaxHp; }

public:
    void      Call_UpdateStatus();
    void      Call_ChangeCurrentHP(_float fCurrentHP);

protected:
    virtual void      Bind_Player();

    virtual void OnDisable() override;

private:
    weak_ptr< CProgressBar>   m_pMainBar;
    weak_ptr< CCustomUI>   m_pBG;
    weak_ptr< CCustomUI>   m_pBorderLeft;
    weak_ptr< CCustomUI>   m_pBorderRight;
    weak_ptr< CHUD_Hover>   m_pTrack;

private:
    _float         m_fMaxHp;
    _float         m_fCurrentHp;
    _float         m_fLerpHp;

private:
    TEXTINFO      m_tTextInfo;
public:
    void Free();

};

END
