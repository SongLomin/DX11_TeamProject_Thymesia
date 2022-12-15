#pragma once
#include "UI_LerpBar.h"

BEGIN(Client)

class CProgressBar;
class CCustomUI;
class CHUD_Hover;
class CEasingComponent_Float;


class CPlayer_HPBar final : public CCustomUI
{
    friend class CPreset_UIDesc;
public:
    GAMECLASS_H(CPlayer_HPBar);
    CLONE_H(CPlayer_HPBar, CGameObject);

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual HRESULT Start();
    virtual void Tick(_float fTimeDelta);
    virtual void LateTick(_float fTimeDelta);
    virtual HRESULT Render(ID3D11DeviceContext* pDeviceContext);
    virtual void    SetUp_Component() override;


public:
    void        Set_CurrentHp(_float   _fCurrentHp);
    void        Set_MaxHp(_float   _fMaxHp) { m_fMaxHp = _fMaxHp; }
    _float      Get_Ratio() { return m_fLerpHp / m_fMaxHp; }


public: //Callback Function
    void      Call_UpdateStatus();
    void      Call_ChangeCurrentHP(_float fCurrentHP);

protected://From Parent
    virtual void        Bind_Player();
    virtual void        OnDisable() override;


private://Only myFunction
    void            Update_HPBar();
    void            Update_Track(_float fRatio);
    void            Make_TextDesc();

private://Only myMember
    weak_ptr< CProgressBar>   m_pMainBar;
    weak_ptr< CCustomUI>   m_pBG;
    weak_ptr< CCustomUI>   m_pBorderLeft;
    weak_ptr< CCustomUI>   m_pBorderRight;
    weak_ptr< CHUD_Hover>   m_pTrack;

private://Component
    weak_ptr<CEasingComponent_Float> m_pEasingFloatCom;

private://only myVariable;
    _float         m_fMaxHp;
    _float         m_fCurrentHp;
    _float         m_fLerpHp;
    TEXTINFO      m_tTextInfo;

//from parent
private:
    void Free();

};

END
