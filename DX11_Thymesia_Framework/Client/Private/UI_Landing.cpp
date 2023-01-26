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
#include "UI_EvolveMenu_Level.h"
#include "UI_EvolveMenu.h"
#include "ClientLevel.h"
#include "UIManager.h"
#include "GameInstance.h"
#include "ClientLevel.h"
#include "EasingComponent_Alpha.h"
#include "FadeMask.h"
#include "EasingComponent_Float.h"

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
    m_tLandingFaderDesc.fFadeMaxTime = 1.f;
    m_tLandingFaderDesc.vFadeColor = _float4(0, 0, 0, 0.3f);

    m_vecChildUI.push_back(m_pLanding);
    m_vecChildUI.push_back(m_pLandingBG);

    m_pEasingBlurAmount = Add_Component<CEasingComponent_Alpha>();
    m_pFadeMask = GAMEINSTANCE->Get_GameObjects<CFadeMask>(LEVEL_STATIC).front();
    m_pEasingFloat = Add_Component<CEasingComponent_Float>();
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
   
    if (m_pEasingBlurAmount.lock()->Is_Lerping())
    {
        _float3 fPlayerWorldPos;

        XMStoreFloat3(&fPlayerWorldPos,
            GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_WorldPosition());

        GAMEINSTANCE->Set_RadialBlur(m_pEasingBlurAmount.lock()->Get_Lerp(), 
            fPlayerWorldPos);
    }
}
void CUI_Landing::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

}

HRESULT CUI_Landing::Render(ID3D11DeviceContext* pDeviceContext)
{

    return S_OK;
}

void CUI_Landing::Call_Landing(LANDING_TYPE eLandingType)
{
    m_pLanding.lock()->CallBack_FadeEnd -= bind(&CUI_Landing::Call_FadeEnd, this, placeholders::_1);
    m_ePreCalledLanding = eLandingType;
    CHUD_Hover::HUDHOVERDESC desc;
    desc.bSizeChange = true;
    desc.fSizeMag = 0.1f;
    desc.eType = CHUD_Hover::HUD_HOVER_ANIMATION_JUSTADD;
   
    if (eLandingType == LANDING_KILL_BOSS)
    {
        GAMEINSTANCE->PlaySound2D("BossKill.mp3", GET_SINGLE(CUIManager)->Get_SoundType(UI_SOUND_TYPE::SOUND_EFFECT));
    }
    if ((_uint)eLandingType < (_uint)LANDING_ENTER_STAGE)
    {
        m_pLanding.lock()->Init_Fader(m_tLandingFaderDesc, desc);
        m_pLanding.lock()->Get_Component<CTexture>().lock()->Use_Texture(m_LandingTextures[eLandingType].c_str());
        m_pLanding.lock()->Set_UIDesc(m_tLandingUIDesc[eLandingType]);
        m_pLanding.lock()->CallBack_FadeEnd += bind(&CUI_Landing::Call_FadeEnd, this, placeholders::_1);

        m_pLandingBG.lock()->CallBack_FadeEnd -= bind(&CUI_Landing::Call_FadeEnd, this, placeholders::_1);

        UI_DESC LandingBG_Desc = m_tLandingUIDesc[eLandingType];
        LandingBG_Desc.fSizeX = (_float)g_iWinCX;
        LandingBG_Desc.fSizeY = (_float)330.f;
        LandingBG_Desc.fDepth = 0.1f;

        m_pLandingBG.lock()->Init_Fader(m_tLandingFaderDesc, desc);
        m_pLandingBG.lock()->Set_UIDesc(LandingBG_Desc);
        m_pLandingBG.lock()->CallBack_FadeEnd += bind(&CUI_Landing::Call_FadeEnd, this, placeholders::_1);

    }
    else//스테이지에 따라 나와야함.GetCurrentLevel이나 머시기...그런걸로
    {
        m_pLanding.lock()->Init_Fader(m_tLandingFaderDesc, desc);

        LEVEL eLevel =  Weak_StaticCast<CClientLevel>(GAMEINSTANCE->Get_CurrentLevel()).lock()->Get_MyLevel();

        switch (eLevel)
        {
        case Client::LEVEL_LOBBY:
            m_pLanding.lock()->Set_Size(820.f, 643.f);
            m_pLanding.lock()->Get_Component<CTexture>().lock()->Use_Texture("Landing_SafeHouse");
            break;
        case Client::LEVEL_GAMEPLAY:
            m_pLanding.lock()->Set_Size(820.f, 643.f);
            m_pLanding.lock()->Get_Component<CTexture>().lock()->Use_Texture("Landing_SeaOfTrees");
            break;
        case Client::LEVEL_STAGE2:
            m_pLanding.lock()->Set_Size(820.f, 643.f);
            m_pLanding.lock()->Get_Component<CTexture>().lock()->Use_Texture("Landing_RoyalGarden");
            break;
        case Client::LEVEL_STAGE3:
            m_pLanding.lock()->Set_Size(820.f, 643.f);
            m_pLanding.lock()->Get_Component<CTexture>().lock()->Use_Texture("Landing_HermesFortress");
            break;
        case Client::LEVEL_TEST:
            m_pLanding.lock()->Set_Size(820.f, 643.f);
            m_pLanding.lock()->Get_Component<CTexture>().lock()->Use_Texture("Landing_SafeHouse");
            break;
        default:
            m_pLanding.lock()->Get_Component<CTexture>().lock()->Use_Texture("Landing_HermesFortress");
            break;
        }
        m_pLanding.lock()->Set_UIDesc(m_tLandingUIDesc[LANDING_ENTER_STAGE]);
        m_pLanding.lock()->CallBack_FadeEnd += bind(&CUI_Landing::Call_FadeEnd, this, placeholders::_1);
    }
}

void CUI_Landing::Call_Landing(LANDING_TYPE eLandingType, _float fCallTime)
{
    m_pLanding.lock()->Set_AlphaColor(_float4(0.f, 0.f, 0.f, 0.f));
    Set_Enable(true);

    m_ePreCalledLanding = eLandingType;
    
    m_pEasingFloat.lock()->Set_Lerp(0.f, 1.f, fCallTime, EASING_TYPE::QUAD_IN, CEasingComponent::ONCE);
    m_pEasingFloat.lock()->Callback_LerpEnd += bind(&CUI_Landing::Call_TimerEnd, this);
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

        switch (m_ePreCalledLanding)
        {
        case Client::CUI_Landing::LANDING_BECONFOUND:
         //   Weak_StaticCast<CClientLevel>(GAMEINSTANCE->Get_CurrentLevel()).lock()->Call_Enable_EvolveMenu();
            break;
        case Client::CUI_Landing::LANDING_DEAD:
            break;
        case Client::CUI_Landing::LANDING_KILL_BOSS:
            break;
        case Client::CUI_Landing::LANDING_ENTER_STAGE:
            break;
        case Client::CUI_Landing::LANDING_END:
            break;
        default:
            break;
        }
    }
}

void CUI_Landing::Call_TimerEnd()
{
    m_pLanding.lock()->Set_AlphaColor(_float4(0.f, 0.f, 0.f, 1.f));

    Call_Landing(m_ePreCalledLanding);
}

