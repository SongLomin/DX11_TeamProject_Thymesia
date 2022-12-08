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


GAMECLASS_C(CPlayer_HPBar)
CLONE_C(CPlayer_HPBar, CGameObject);

HRESULT CPlayer_HPBar::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CPlayer_HPBar::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    Set_UIPosition(250.f, 800.f, 400.f, 15.f);

    m_pBG = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    m_pBG.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_HPBar_BG");
    m_pBG.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY, m_tUIDesc.fSizeX, m_tUIDesc.fSizeY);
    m_pBG.lock()->Set_Depth(0.2f);


    m_pMainBar = GAMEINSTANCE->Add_GameObject<CProgressBar>(LEVEL_STATIC);
    m_pMainBar.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_HPBar_MainBar");
    m_pMainBar.lock()->Set_UIPosition(m_tUIDesc.fX, m_tUIDesc.fY, m_tUIDesc.fSizeX, m_tUIDesc.fSizeY - 5.f);
    m_pMainBar.lock()->Set_Depth(0.1f);

    m_pBorderLeft = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    m_pBorderLeft.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_HPBar_Border_Left");
    m_pBorderLeft.lock()->Set_UIPosition(m_tUIDesc.fX - (m_tUIDesc.fSizeX * 0.5f), m_tUIDesc.fY, 26.f, 15.f);

    m_pBorderRight = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC);
    m_pBorderRight.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_HPBar_Border_Right");
    m_pBorderRight.lock()->Set_UIPosition(m_tUIDesc.fX + (m_tUIDesc.fSizeX * 0.5f), m_tUIDesc.fY, 26.f, 15.f);


    Engine::FaderDesc tFaderDesc;

    UI_DESC   tTrackDesc;
    tTrackDesc.fX = m_tUIDesc.fX + (m_tUIDesc.fSizeX * 0.5f);
    tTrackDesc.fY = m_tUIDesc.fY;
    tTrackDesc.fSizeX = 19.f;
    tTrackDesc.fSizeY = 45.f;
    tTrackDesc.fDepth = 0.f;

    tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
    tFaderDesc.eFaderType = FADER_TYPE::FADER_INOUTLOOPING;
    tFaderDesc.fDelayTime = 0.f;
    tFaderDesc.fFadeMaxTime = 1.f;
    tFaderDesc.vFadeColor = _float4(0, 0, 0, 1.f);

    CHUD_Hover::HUDHOVERDESC tHoverDesc;
    ZeroMemory(&tHoverDesc, sizeof(CHUD_Hover::HUDHOVERDESC));

    tHoverDesc.bSizeChange = false;
    tHoverDesc.eType = CHUD_Hover::HUD_HOVER_ANIMATION_FROM_ALPHA;


    m_pTrack = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_STATIC, &tTrackDesc);
    m_pTrack.lock()->Get_Component<CTexture>().lock()->Use_Texture("Player_HPBar_Track");
    m_pTrack.lock()->Init_Fader(tFaderDesc, tHoverDesc);
    m_pTrack.lock()->Set_RenderGroup(RENDERGROUP::RENDER_BEFOREUI);

    m_fLerpHp = 0.f;
    m_fCurrentHp = 0.f;
    m_fMaxHp = 0.f;

    m_tTextInfo.bAlways = false;
    m_tTextInfo.bCenterAlign = false;
    m_tTextInfo.fRotation = 0.f;
    m_tTextInfo.vColor = _float4(0.7f, 0.7f, 0.7f, 0.7f);
    m_tTextInfo.vScale = _float2(0.5, 0.5f);
    m_tTextInfo.vPosition = _float2(m_tUIDesc.fX + m_tUIDesc.fSizeX * 0.5f + 20.f, m_tUIDesc.fY - 10.f);
    m_tTextInfo.eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;

    m_tUIDesc.fDepth = 0.f;

    m_eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;
    GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::PLAYERHUD, Cast<CGameObject>(m_this));

    /*
       weak_ptr< CProgressBar>   m_pMainBar;
    weak_ptr< CCustomUI>   m_pBG;
    weak_ptr< CCustomUI>   m_pBorderLeft;
    weak_ptr< CCustomUI>   m_pBorderRight;
    weak_ptr< CHUD_Hover>   m_pTrack;
    */

    Add_Child(m_pMainBar);
    Add_Child(m_pBG);
    Add_Child(m_pBorderLeft);
    Add_Child(m_pBorderRight);

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

    //TODO : HPBar Lerp Test Code
    if (m_fCurrentHp < 0.f)
        m_fCurrentHp = 0.f;
    else if (m_fCurrentHp > m_fMaxHp)
        m_fCurrentHp = m_fMaxHp;

    if (Is_Lerping())
        m_fLerpHp = Get_Lerp().x;

    _float fRatio = m_fLerpHp / m_fMaxHp;

    if (fRatio < 1.f)
    {
        m_pTrack.lock()->Set_UIPosition((m_tUIDesc.fX - (m_tUIDesc.fSizeX * 0.5f)) + (m_tUIDesc.fSizeX * fRatio), m_tUIDesc.fY);
        m_pTrack.lock()->Set_Enable(true);
    }
    else
        m_pTrack.lock()->Set_Enable(false);

    m_pMainBar.lock()->Set_Ratio(fRatio);

    m_pTrack.lock()->Get_UIDESC();

    m_tTextInfo.szText = to_wstring((_uint)m_fLerpHp);
    m_tTextInfo.szText.append(L"/");
    m_tTextInfo.szText.append(to_wstring((_uint)m_fMaxHp));
}

void CPlayer_HPBar::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::DREAM, m_tTextInfo);

}

HRESULT CPlayer_HPBar::Render()
{
    //__super::Render();

    return S_OK;;
}

void CPlayer_HPBar::OnEventMessage(_uint iArg)
{
}

void CPlayer_HPBar::Set_CurrentHp(_float _fCurrentHp, _bool bLerp, EASING_TYPE eLerpType)
{
    m_fCurrentHp = _fCurrentHp;

    if (m_fCurrentHp <= 0.f)
        m_fCurrentHp = 0.f;

    if (!bLerp)
        m_fLerpHp = m_fCurrentHp;
    else
        Set_Lerp(m_fLerpHp, m_fCurrentHp, 1.f, eLerpType);
}


void CPlayer_HPBar::Call_UpdateStatus()
{
    m_fMaxHp = m_pPlayerStatus.lock()->Get_MaxHP();
    m_fLerpHp = m_fMaxHp;
    m_fCurrentHp = m_fMaxHp;
}

void CPlayer_HPBar::Call_ChangeCurrentHP(_float fCurrentHP)
{
    Set_CurrentHp(fCurrentHP);
}

void CPlayer_HPBar::Bind_Player()
{

#ifndef _ONLY_UI_
    __super::Bind_Player();
    Call_UpdateStatus();

    m_pPlayerStatus.lock()->Callback_ChangeHP += bind(&CPlayer_HPBar::Call_ChangeCurrentHP, this,
        placeholders::_1);

    m_pPlayerStatus.lock()->Callback_Update_Status += bind(&CPlayer_HPBar::Call_UpdateStatus, this);

#else
    m_fMaxHp = 300.f;
    m_fLerpHp = m_fMaxHp;
    m_fCurrentHp = m_fMaxHp;
#endif // !_ONLY_UI_


}

void CPlayer_HPBar::OnDisable()
{
    __super::OnDisable();

    m_pTrack.lock()->Set_Enable(false);
}

void CPlayer_HPBar::Free()
{
    __super::Free();

}