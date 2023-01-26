#include "stdafx.h"
#include "HUD_PlagueWeaponBase.h"
#include "HUD_PlagueWeapon_Steal.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "HUD_Hover.h"
#include "HUD_PlagueWeapon_Steal_Icon.h"
#include "ProgressBar.h"
#include "Texture.h"
#include "PlayerSkillHeader.h"


GAMECLASS_C(CHUD_PlagueWeapon_Steal);
CLONE_C(CHUD_PlagueWeapon_Steal, CGameObject);

HRESULT CHUD_PlagueWeapon_Steal::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CHUD_PlagueWeapon_Steal::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pPlagueWeapon_Border = GAMEINSTANCE->Add_GameObject<CProgressBar>(LEVEL_STATIC, &m_tUIDesc);
    m_pPlagueWeapon_Border.lock()->Set_Texture("HUD_FrameBorder");
    m_pPlagueWeapon_Border.lock()->Set_Depth(0.4f);
    m_pPlagueWeapon_Border.lock()->Set_PassIndex(3);
    m_pPlagueWeapon_Border.lock()->Set_RenderGroup(RENDERGROUP::RENDER_BEFOREUI);

    m_pPlagueWeapon_Main = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
    m_pPlagueWeapon_Main.lock()->Set_Texture("HUD_PlagueWeapon_Frame_Steal");
    m_pPlagueWeapon_Main.lock()->Set_Depth(0.3f);
    m_pPlagueWeapon_Main.lock()->Set_Size(201.f, 201.f);
    m_pPlagueWeapon_Main.lock()->Set_RenderGroup(RENDERGROUP::RENDER_BEFOREUI);

    m_pIcon = GAMEINSTANCE->Add_GameObject<CHUD_PlagueWeapon_Steal_Icon>(LEVEL_STATIC, &m_tUIDesc);
    m_pIcon.lock()->Set_Depth(0.2f);
    m_pIcon.lock()->Set_Size(160.f, 160.f);

    m_pPlagueWeapon_Ready = GAMEINSTANCE->Add_GameObject<CProgressBar>(LEVEL_STATIC, &m_tUIDesc);
    m_pPlagueWeapon_Ready.lock()->Set_Texture("HUD_PlagueWeapon_Frame_Ready");
    m_pPlagueWeapon_Ready.lock()->Set_Depth(0.1f);
    m_pPlagueWeapon_Ready.lock()->Set_PassIndex(3);
    m_pPlagueWeapon_Ready.lock()->Set_AlphaColor(0.3f);
    m_pPlagueWeapon_Ready.lock()->Set_Enable(false);
    m_pPlagueWeapon_Ready.lock()->Set_RenderGroup(RENDERGROUP::RENDER_BEFOREUI);

    m_pPlagueWeapon_Decoration = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
    m_pPlagueWeapon_Decoration.lock()->Set_Texture("HUD_PlagueWeapon_Frame_Steal_Decoration");
    m_pPlagueWeapon_Decoration.lock()->Set_Depth(0.0f);
    m_pPlagueWeapon_Decoration.lock()->Set_Size(201.f, 201.f);
    
    m_pHover = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_STATIC, &m_tUIDesc);
    m_pHover.lock()->Get_Component<CTexture>().lock()->Use_Texture("HUD_Frame_Hover");
    m_pHover.lock()->Set_RenderGroup(RENDERGROUP::RENDER_BEFOREUI);

    m_eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;

    Add_Child(m_pPlagueWeapon_Main);
    Add_Child(m_pIcon);
    Add_Child(m_pPlagueWeapon_Decoration);
    Add_Child(m_pPlagueWeapon_Border);

    //HoverDesc
    //HoverFaderDesc
    m_tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
    m_tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
    m_tFaderDesc.fDelayTime = 0.f;
    m_tFaderDesc.fFadeMaxTime = 1.f;
    m_tFaderDesc.vFadeColor = _float4(0, 0, 0, 0.7f);

    return S_OK;
}

HRESULT CHUD_PlagueWeapon_Steal::Start()
{
    __super::Start();

    Bind_Player();

    Call_OnChangeSkill
    (
        GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->
        Get_Component<CPlayerSkill_System>().lock()->Get_StealSkill()
    );

    return S_OK;
}

void CHUD_PlagueWeapon_Steal::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

}

void CHUD_PlagueWeapon_Steal::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

}

HRESULT CHUD_PlagueWeapon_Steal::Render(ID3D11DeviceContext* pDeviceContext)
{
    //Render    

    return S_OK;
}

void CHUD_PlagueWeapon_Steal::Call_OnChangeSkill(weak_ptr<CSkill_Base> pSkillBase)
{
    //약탈 무기의 쿨타임은 스킬 시스템에서 가져온다.
    Init_Icon(pSkillBase.lock()->Get_SkillName());

    pSkillBase.lock()->Callback_StartSkill =
        bind(&CHUD_PlagueWeaponBase::Call_UseStartSkill, this);

    pSkillBase.lock()->Callback_UpdateCoolDown =
        bind(&CHUD_PlagueWeaponBase::Call_UpdateCoolDown, this, placeholders::_1);

    pSkillBase.lock()->Callback_EndCoolDown =
        bind(&CHUD_PlagueWeaponBase::Call_EndCoolDown, this);

    Weak_StaticCast<CHUD_PlagueWeapon_Steal_Icon>(m_pIcon).lock()->Start_FadeIn(-2.f, 2.f, 1.f);
}

void CHUD_PlagueWeapon_Steal::Call_UseStartSkill()
{
    m_pPlagueWeapon_Border.lock()->Set_Enable(true);
    m_pPlagueWeapon_Ready.lock()->Set_Enable(true);

    m_pPlagueWeapon_Main.lock()->Set_AlphaColor(0.3f);
    Weak_StaticCast<CHUD_PlagueWeapon_Steal_Icon>(m_pIcon).lock()->Start_FadeOut(2.f, -2.f, 1.f);
}

void CHUD_PlagueWeapon_Steal::Call_UpdateCoolDown(_float fCoolDownRatio)
{
    __super::Call_UpdateCoolDown(fCoolDownRatio);
}

void CHUD_PlagueWeapon_Steal::Call_EndCoolDown()
{
    __super::Call_EndCoolDown();
}

void CHUD_PlagueWeapon_Steal::Bind_Player()
{
    CUI::Bind_Player();

    weak_ptr<CPlayerSkill_System> pSkillSystem = m_pPlayer.lock()->Get_Component<CPlayerSkill_System>();

    pSkillSystem.lock()->Callback_OnStealSkill =
        bind(&CHUD_PlagueWeaponBase::Call_OnChangeSkill, this, placeholders::_1);
}


void CHUD_PlagueWeapon_Steal::Free()
{

}
