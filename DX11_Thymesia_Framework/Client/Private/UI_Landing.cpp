#include "stdafx.h"
#include "UI_Landing.h"
#include "CustomUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "Fader.h"
#include "Engine_Defines.h"
#include "Client_Defines.h"
#include "HUD_Hover.h"

GAMECLASS_C(CUI_Landing)
CLONE_C(CUI_Landing, CGameObject)

HRESULT CUI_Landing::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CUI_Landing::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_LandingTextures[LANDING_BECONFOUND]   = "Landing_BeconFound";
    m_LandingTextures[LANDING_DEAD]         = "Landing_MemoryInterrupted";
    m_LandingTextures[LANDING_KILL_BOSS]    = "Landing_RecallCompleted";
    m_LandingTextures[LANDING_ENTER_STAGE] = "Landing_SafeHouse";



    m_tLandingUIDesc[LANDING_BECONFOUND].fX = ((_float)g_iWinCX) * 0.5f;
    m_tLandingUIDesc[LANDING_BECONFOUND].fY = ((_float)g_iWinCY) * 0.5f;
    m_tLandingUIDesc[LANDING_BECONFOUND].fSizeX = 730.f;
    m_tLandingUIDesc[LANDING_BECONFOUND].fSizeY = 105.f;
    m_tLandingUIDesc[LANDING_BECONFOUND].fDepth = 0.f;

    m_tLandingUIDesc[LANDING_DEAD].fX = ((_float)g_iWinCX) * 0.5f;
    m_tLandingUIDesc[LANDING_DEAD].fY = ((_float)g_iWinCY) * 0.5f;
    m_tLandingUIDesc[LANDING_DEAD].fSizeX = 1037.f;
    m_tLandingUIDesc[LANDING_DEAD].fSizeY = 150.f;
    m_tLandingUIDesc[LANDING_DEAD].fDepth = 0.f;

    m_tLandingUIDesc[LANDING_KILL_BOSS].fX = ((_float)g_iWinCX) * 0.5f;
    m_tLandingUIDesc[LANDING_KILL_BOSS].fY = ((_float)g_iWinCY) * 0.3f;
    m_tLandingUIDesc[LANDING_KILL_BOSS].fSizeX = 911.f;
    m_tLandingUIDesc[LANDING_KILL_BOSS].fSizeY = 90.f;
    m_tLandingUIDesc[LANDING_KILL_BOSS].fDepth = 0.f;

    m_tLandingUIDesc[LANDING_ENTER_STAGE].fX = ((_float)g_iWinCX) * 0.5f;
    m_tLandingUIDesc[LANDING_ENTER_STAGE].fY = ((_float)g_iWinCY) * 0.3f;
    m_tLandingUIDesc[LANDING_ENTER_STAGE].fSizeX = 820.f;
    m_tLandingUIDesc[LANDING_ENTER_STAGE].fSizeY = 643.f;
    m_tLandingUIDesc[LANDING_ENTER_STAGE].fDepth = 0.f;


    //최소 기본 세팅.
    m_pLanding = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_STATIC, &m_tLandingUIDesc[LANDING_BECONFOUND]);
    m_pLanding.lock()->Get_Component<CTexture>().lock()->Use_Texture("Landing_BeconFound");

    m_pLandingBG = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_STATIC, &m_tLandingUIDesc[LANDING_BECONFOUND]);
    m_pLandingBG.lock()->Get_Component<CTexture>().lock()->Use_Texture("Landing_BG");


    m_tLandingFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
    m_tLandingFaderDesc.eFaderType = FADER_TYPE::FADER_INOUTLOOPING;
    m_tLandingFaderDesc.fDelayTime = 0.f;
    m_tLandingFaderDesc.fFadeMaxTime = 1.5f;
    m_tLandingFaderDesc.vFadeColor = _float4(0, 0, 0, 0.3f);



    return S_OK;
}

HRESULT CUI_Landing::Start()
{
    __super::Start();


    return S_OK;
}

void CUI_Landing::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

#ifdef _DEBUG
  
    if (KEY_INPUT(KEY::V, KEY_STATE::TAP))
    {
        Call_Landing(LANDING_BECONFOUND);
    }
    if (KEY_INPUT(KEY::B, KEY_STATE::TAP))
    {
        Call_Landing(LANDING_KILL_BOSS);
    }
    if (KEY_INPUT(KEY::N, KEY_STATE::TAP))
    {
        Call_Landing(LANDING_ENTER_STAGE);
    }

#endif // _DEBUG

}

void CUI_Landing::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

}

HRESULT CUI_Landing::Render()
{




    return S_OK;
}

void CUI_Landing::Call_Landing(LANDING_TYPE eLandingType)
{
    m_pLanding.lock()->CallBack_FadeEnd -= bind(&CUI_Landing::Call_FadeEnd, this, placeholders::_1);


    CHUD_Hover::HUDHOVERDESC desc;
    desc.m_bSizeChange = true;
    desc.m_fSizeMag = 0.1;

    m_pLanding.lock()->Init_Fader(m_tLandingFaderDesc, desc, CHUD_Hover::HUD_HOVER_ANIMATION_JUSTADD);
    m_pLanding.lock()->Get_Component<CTexture>().lock()->Use_Texture(m_LandingTextures[eLandingType].c_str());
    m_pLanding.lock()->Set_UIDesc(m_tLandingUIDesc[eLandingType]);
    m_pLanding.lock()->CallBack_FadeEnd += bind(&CUI_Landing::Call_FadeEnd, this, placeholders::_1);
    
    if (eLandingType < LANDING_ENTER_STAGE)
    {
        m_pLandingBG.lock()->CallBack_FadeEnd -= bind(&CUI_Landing::Call_FadeEnd, this, placeholders::_1);

        UI_DESC LandingBG_Desc = m_tLandingUIDesc[eLandingType];
        LandingBG_Desc.fSizeX = (_float)g_iWinCX;
        LandingBG_Desc.fSizeY = (_float)330.f;
        LandingBG_Desc.fDepth = 0.1f;

        m_pLandingBG.lock()->Init_Fader(m_tLandingFaderDesc, desc, CHUD_Hover::HUD_HOVER_ANIMATION_JUSTADD);
        m_pLandingBG.lock()->Set_UIDesc(LandingBG_Desc);
        m_pLandingBG.lock()->CallBack_FadeEnd += bind(&CUI_Landing::Call_FadeEnd, this, placeholders::_1);
    }
}

void CUI_Landing::Call_FadeEnd(FADER_TYPE eFaderType)
{
    //이거 루핑 인 에도 돌고 아웃에도 콜백이 발동해서
    //아싸리 확실하게 알파가 0일때만 없애주게 함.

    if (eFaderType == FADER_TYPE::FADER_OUT)
    {
        m_pLanding.lock()->Set_UIDesc(m_tUIDesc);
        m_pLanding.lock()->Set_Enable(false);

        m_pLandingBG.lock()->Set_UIDesc(m_tUIDesc);
        m_pLandingBG.lock()->Set_Enable(false);
    }
}
