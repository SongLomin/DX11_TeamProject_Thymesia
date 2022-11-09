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

    m_tLandingUIDesc[LANDING_BECONFOUND].fX = ((_float)g_iWinCX) * 0.5f;
    m_tLandingUIDesc[LANDING_BECONFOUND].fY = ((_float)g_iWinCY) * 0.5f;
    m_tLandingUIDesc[LANDING_BECONFOUND].fSizeX = 730.f;
    m_tLandingUIDesc[LANDING_BECONFOUND].fSizeY = 105.f;



    m_pLanding = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_STATIC, &m_tLandingUIDesc[LANDING_BECONFOUND]);
    m_pLanding.lock()->Get_Component<CTexture>().lock()->Use_Texture("Landing_BeconFound");


    m_tLandingFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
    m_tLandingFaderDesc.eFaderType = FADER_TYPE::FADER_INOUTLOOPING;
    m_tLandingFaderDesc.fDelayTime = 0.f;
    m_tLandingFaderDesc.fFadeMaxTime = 2.f;
    m_tLandingFaderDesc.vFadeColor = _float4(0, 0, 0, 0.6f);

    m_PreCalledLanding = LANDING_END;

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
    m_PreCalledLanding = eLandingType;

    CHUD_Hover::HUDHOVERDESC desc;
    desc.m_bSizeChange = false;
    desc.m_fSizeMag = 0.4;

    m_pLanding.lock()->Init_Fader(m_tLandingFaderDesc, desc);
    m_pLanding.lock()->Get_Component<CTexture>().lock()->Use_Texture("Landing_BeconFound");
    m_pLanding.lock()->Set_UIDesc(m_tLandingUIDesc[eLandingType]);
    m_pLanding.lock()->CallBack_FadeEnd += bind(&CUI_Landing::Call_FadeEnd, this, placeholders::_1);


}

void CUI_Landing::Call_FadeEnd(FADER_TYPE eFaderType)
{
    //이거 루핑 인 에도 돌고 아웃에도 콜백이 발동해서
    //아싸리 확실하게 알파가 0일때만 없애주게 함.

    if (eFaderType == FADER_TYPE::FADER_OUT)
    {
        m_pLanding.lock()->Set_UIDesc(m_tUIDesc);
        m_pLanding.lock()->Set_Enable(false);
    }
}
