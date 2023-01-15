#include "stdafx.h"
#include "Player_HPBar.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "ProgressBar.h"
#include "CustomUI.h"
#include "HUD_Hover.h"
#include "Fader.h"
#include "Player.h"
#include "Status_Player.h"
#include "Easing_Utillity.h"
#include "UI_LerpBar.h"
#include "EasingComponent_Float.h"
#include "Preset_UIDesc.h"


GAMECLASS_C(CPlayer_HPBar)
CLONE_C(CPlayer_HPBar, CGameObject);

HRESULT CPlayer_HPBar::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPlayer_HPBar::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    CPreset_UIDesc::Set_CUI_PlayerHPBar(Weak_StaticCast<CPlayer_HPBar>(m_this));

    SetUp_Component();

    GET_SINGLE(CGameManager)->CallBack_ChangePlayer +=
        bind(&CPlayer_HPBar::Bind_Player, this);

    return S_OK;
}

HRESULT CPlayer_HPBar::Start()
{
    __super::Start();

    Bind_Player();

    return S_OK;
}

void CPlayer_HPBar::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    Update_HPBar();

    Update_Track(Get_Ratio());
}

void CPlayer_HPBar::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
    
    Make_TextDesc();

    GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::DREAM, m_tTextInfo);

}

HRESULT CPlayer_HPBar::Render(ID3D11DeviceContext* pDeviceContext)
{
    //__super::Render(pDeviceContext);

    return S_OK;;
}

void CPlayer_HPBar::SetUp_Component()
{
    m_pEasingFloatCom = Add_Component<CEasingComponent_Float>();
}

void CPlayer_HPBar::Set_CurrentHp(_float _fCurrentHp)
{
    m_fCurrentHp = _fCurrentHp;

    if (m_fCurrentHp < 0.f)
        m_fCurrentHp = 0.f;
    else if (m_fCurrentHp > m_fMaxHp)
        m_fCurrentHp = m_fMaxHp;

    m_pEasingFloatCom.lock()->Set_Lerp(m_fLerpHp, m_fCurrentHp, 0.5f, EASING_TYPE::QUAD_IN, CEasingComponent::ONCE);
}

void CPlayer_HPBar::Call_UpdateStatus()
{
    CStatus_Player::PLAYERDESC tPlayerDesc = m_pPlayerStatus.lock()->Get_PlayerDesc();

    m_fMaxHp = tPlayerDesc.m_fMaxHP;
    m_fLerpHp = m_fMaxHp;
    m_fCurrentHp = m_fMaxHp;

    m_pMainBar.lock()->Set_Ratio(1.f);
}

void CPlayer_HPBar::Call_ChangeCurrentHP(_float fCurrentHP)
{
    Set_CurrentHp(fCurrentHP);
}

void CPlayer_HPBar::Bind_Player()
{
    __super::Bind_Player();

    m_pPlayerStatus.lock()->Callback_ChangeHP += bind(&CPlayer_HPBar::Call_ChangeCurrentHP, this,
        placeholders::_1);

    m_pPlayerStatus.lock()->Callback_Update_Status += bind(&CPlayer_HPBar::Call_UpdateStatus, this);

    Set_MaxHp(m_pPlayerStatus.lock()->Get_MaxHP());
    Set_CurrentHp(m_pPlayerStatus.lock()->Get_CurrentHP());

    //Call_UpdateStatus();
}

void CPlayer_HPBar::OnDisable()
{
    __super::OnDisable();

    m_pTrack.lock()->Set_Enable(false);
}

void CPlayer_HPBar::Update_HPBar()
{
    if (m_pEasingFloatCom.lock()->Is_Lerping())
    {
        m_fLerpHp = m_pEasingFloatCom.lock()->Get_Lerp();
    }

    _float fRatio = m_fLerpHp / m_fMaxHp;

    m_pMainBar.lock()->Set_Ratio(fRatio);

}

void CPlayer_HPBar::Update_Track(_float fRatio)
{
    if (fRatio < 1.f)
    {
        m_pTrack.lock()->Set_UIPosition((m_tUIDesc.fX - (m_tUIDesc.fSizeX * 0.5f)) + (m_tUIDesc.fSizeX * fRatio), m_tUIDesc.fY);
        m_pTrack.lock()->Set_Enable(true);
    }
    else
        m_pTrack.lock()->Set_Enable(false);
}

void CPlayer_HPBar::Make_TextDesc()
{
    m_tTextInfo.szText = to_wstring((_int)m_fLerpHp);
    m_tTextInfo.szText.append(L"/");
    m_tTextInfo.szText.append(to_wstring((_int)m_fMaxHp));
}

void CPlayer_HPBar::Free()
{
    __super::Free();

}