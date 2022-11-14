#include "stdafx.h"
#include "HUD_PlagueWeapon.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "CustomUI.h"
#include "Player_ProgressBar.h"
#include "HUD_Hover.h"

GAMECLASS_C(CHUD_PlagueWeapon);
CLONE_C(CHUD_PlagueWeapon, CGameObject);

HRESULT CHUD_PlagueWeapon::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CHUD_PlagueWeapon::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    if (nullptr != pArg)
        memcpy(&m_tUIDesc, pArg, sizeof(UI_DESC));
    else
        Set_UIPosition(1405.f, 785.f, 93.f, 93.f);

    m_tUIDesc.fDepth = 0.f;

    m_pPlagueWeapon_Border = GAMEINSTANCE->Add_GameObject<CPlayer_ProgressBar>(LEVEL_STATIC, &m_tUIDesc);
    m_pPlagueWeapon_Border.lock()->Set_Texture("HUD_FrameBorder");
    m_pPlagueWeapon_Border.lock()->Set_Depth(0.4f);
    m_pPlagueWeapon_Border.lock()->Set_PassIndex(3);
    m_pPlagueWeapon_Border.lock()->Set_Enable(false);

    m_pPlagueWeapon_Main = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
    m_pPlagueWeapon_Main.lock()->Set_Texture("HUD_PlagueWeapon_Frame");
    m_pPlagueWeapon_Main.lock()->Set_Depth(0.3f);

    m_pPlagueWeapon_Icon = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
    m_pPlagueWeapon_Icon.lock()->Set_Texture("SkillIcon_Axe");
    m_pPlagueWeapon_Icon.lock()->Set_Depth(0.2f);
    m_pPlagueWeapon_Icon.lock()->Set_Size(160.f, 160.f);

    m_pPlagueWeapon_Ready = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
    m_pPlagueWeapon_Ready.lock()->Set_Texture("HUD_PlagueWeapon_Frame_Ready");
    m_pPlagueWeapon_Ready.lock()->Set_Depth(0.1f);
    m_pPlagueWeapon_Ready.lock()->Set_AlphaColor(0.3f);
    m_pPlagueWeapon_Ready.lock()->Set_Enable(false);

    m_pHover = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_STATIC, &m_tUIDesc);
    m_pHover.lock()->Get_Component<CTexture>().lock()->Use_Texture("HUD_Frame_Hover");

    //HoverDesc
    //HoverFaderDesc
    m_tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
    m_tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
    m_tFaderDesc.fDelayTime = 0.f;
    m_tFaderDesc.fFadeMaxTime = 1.f;
    m_tFaderDesc.vFadeColor = _float4(0, 0, 0, 0.7f);

    m_fCoolTime = 5.f;
    m_fNowTime = 5.f;

    GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::BATTLEUI, Cast<CGameObject>(m_pPlagueWeapon_Main));
    GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::BATTLEUI, Cast<CGameObject>(m_pPlagueWeapon_Border));
    GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::BATTLEUI, Cast<CGameObject>(m_pPlagueWeapon_Ready));
    GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::BATTLEUI, Cast<CGameObject>(m_pHover));
    GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::BATTLEUI, Cast<CGameObject>(m_pPlagueWeapon_Icon));

    return S_OK;
}

HRESULT CHUD_PlagueWeapon::Start()
{
    __super::Start();


    return S_OK;
}

void CHUD_PlagueWeapon::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    
    if (m_bIsCoolTime)
    {
        m_fNowTime += fTimeDelta;
        if (m_fCoolTime <= m_fNowTime)//쿨타임이 다 찼을 시.
        {
            m_bIsCoolTime = false;
            m_pPlagueWeapon_Border.lock()->Set_Enable(false);
            m_pPlagueWeapon_Ready.lock()->Set_Enable(false);
            //호버 이벤트 발생

            m_pHover.lock()->CallBack_FadeEnd -= bind(&CHUD_PlagueWeapon::Call_HoverFadeEnd, this, placeholders::_1);
            CHUD_Hover::HUDHOVERDESC tHoverDesc;
            tHoverDesc.m_bSizeChange = true;
            tHoverDesc.m_fSizeMag = 0.2f;
            m_pHover.lock()->CallBack_FadeEnd += bind(&CHUD_PlagueWeapon::Call_HoverFadeEnd, this, placeholders::_1);
            m_pHover.lock()->Init_Fader(m_tFaderDesc, tHoverDesc,
                CHUD_Hover::HUD_HOVER_ANIMATION_FROM_ALPHA);

        }
        m_pPlagueWeapon_Border.lock()->Set_Ratio(m_fNowTime / m_fCoolTime);
    }
#ifdef _DEBUG
    if (KEY_INPUT(KEY::Q, KEY_STATE::TAP))
    {
        Call_UseSkill();
    }
#endif // _DEBUG
}

void CHUD_PlagueWeapon::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    if (m_bIsCoolTime)
    {
       _float alpha =  m_pPlagueWeapon_Ready.lock()->Get_AlphaColor();
    }
}

HRESULT CHUD_PlagueWeapon::Render()
{
    //Render    


    return S_OK;
}

void CHUD_PlagueWeapon::Call_ChangeSkill()
{
}

void CHUD_PlagueWeapon::Call_UseSkill()
{
    //TODO : 역병무기 쿨타임 테스트 입니다.
    Add_Shaking(2.f, 5.f);

    m_fCoolTime = 5.f;
    m_fNowTime = 0.f;
    m_bIsCoolTime = true;
    m_pPlagueWeapon_Border.lock()->Set_Enable(true);
    m_pPlagueWeapon_Ready.lock()->Set_Enable(true);
}

void CHUD_PlagueWeapon::Call_HoverFadeEnd(FADER_TYPE eFaderType)
{
    m_pHover.lock()->Set_UIDesc(m_tUIDesc);
    m_pHover.lock()->Set_Enable(false);

}

void CHUD_PlagueWeapon::Free()
{
}
