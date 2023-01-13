#include "stdafx.h"
#include "HUD_PlagueWeaponBase.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "CustomUI.h"
#include "ProgressBar.h"
#include "HUD_Hover.h"
#include "CustomUI.h"
#include "Player.h"
#include "PlayerSkill_System.h"
#include "Skill_Base.h"
#include "UI_Utils.h"



GAMECLASS_C(CHUD_PlagueWeaponBase);
CLONE_C(CHUD_PlagueWeaponBase, CGameObject);

HRESULT CHUD_PlagueWeaponBase::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CHUD_PlagueWeaponBase::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    if (nullptr == pArg)
        MSG_BOX("Argument Error : PlagueWeaponBase");
    
    GET_SINGLE(CGameManager)->CallBack_ChangePlayer +=
        bind(&CHUD_PlagueWeaponBase::Bind_Player, this);

    return S_OK;
}

HRESULT CHUD_PlagueWeaponBase::Start()
{
    __super::Start();


    return S_OK;
}

void CHUD_PlagueWeaponBase::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    
}

void CHUD_PlagueWeaponBase::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

}

HRESULT CHUD_PlagueWeaponBase::Render()
{
    
    return S_OK;
}

//스킬이 바뀌면 여기서 뚜다다다다다다다다
void CHUD_PlagueWeaponBase::Call_OnChangeSkill(weak_ptr<CSkill_Base> pSkillBase)
{
    if (!pSkillBase.lock())
    {
        return;
    }
    pSkillBase.lock()->Callback_StartSkill +=
        bind(&CHUD_PlagueWeaponBase::Call_UseStartSkill, this);

    pSkillBase.lock()->Callback_UpdateCoolDown +=
        bind(&CHUD_PlagueWeaponBase::Call_UpdateCoolDown, this, placeholders::_1);

    pSkillBase.lock()->Callback_EndCoolDown+=
        bind(&CHUD_PlagueWeaponBase::Call_EndCoolDown, this);

    if (pSkillBase.lock()->Is_UseAble())
    {
        Init_UI();
    }
    else
    {
       Call_UseStartSkill();
    }
    Init_Icon(pSkillBase.lock()->Get_SkillName());
}

void CHUD_PlagueWeaponBase::Call_UseStartSkill()
{
    m_pPlagueWeapon_Border.lock()->Set_Enable(true);
    m_pPlagueWeapon_Ready.lock()->Set_Enable(true);

    m_pPlagueWeapon_Main.lock()->Set_AlphaColor(0.3f);
    m_pIcon.lock()->Set_AlphaColor(0.3f);

}

void CHUD_PlagueWeaponBase::Call_UpdateCoolDown(_float fCoolDownRatio)
{
    m_pPlagueWeapon_Border.lock()->Set_Ratio(fCoolDownRatio);
    m_pPlagueWeapon_Ready.lock()->Set_Ratio(fCoolDownRatio);


}

void CHUD_PlagueWeaponBase::Call_EndCoolDown()
{
    m_pHover.lock()->CallBack_FadeEnd -= bind(&CHUD_PlagueWeaponBase::Call_HoverFadeEnd, this, placeholders::_1);
    CHUD_Hover::HUDHOVERDESC tHoverDesc;
    tHoverDesc.bSizeChange = true;
    tHoverDesc.fSizeMag = 0.2f;
    tHoverDesc.eType = CHUD_Hover::HUD_HOVER_ANIMATION_FROM_ALPHA;
    m_pHover.lock()->CallBack_FadeEnd += bind(&CHUD_PlagueWeaponBase::Call_HoverFadeEnd, this, placeholders::_1);
    m_pHover.lock()->Init_Fader(m_tFaderDesc, tHoverDesc);

    m_pPlagueWeapon_Main.lock()->Set_AlphaColor(1.f);
    m_pIcon.lock()->Set_AlphaColor(1.f);
    m_pPlagueWeapon_Ready.lock()->Set_Enable(false);
}

void CHUD_PlagueWeaponBase::Call_HoverFadeEnd(FADER_TYPE eFaderType)
{
    m_pHover.lock()->Set_UIDesc(m_tUIDesc);
    m_pHover.lock()->Set_Enable(false);

}

void CHUD_PlagueWeaponBase::Bind_Player()
{
    __super::Bind_Player();

    weak_ptr<CPlayerSkill_System> pSkillSystem  = m_pPlayer.lock()->Get_Component<CPlayerSkill_System>();
        
    pSkillSystem.lock()->Callback_OnChangeSkill[(_uint)m_eSkillSocketType] =
        bind(&CHUD_PlagueWeaponBase::Call_OnChangeSkill, this, placeholders::_1);
}

void CHUD_PlagueWeaponBase::Init_UI()
{
    m_pHover.lock()->Set_Enable(false);
    m_pPlagueWeapon_Ready.lock()->Set_Enable(false);
    m_pPlagueWeapon_Border.lock()->Set_Ratio(1.f);
    m_pPlagueWeapon_Main.lock()->Set_AlphaColor(1.f);
    m_pIcon.lock()->Set_AlphaColor(1.f);
}

void CHUD_PlagueWeaponBase::Init_Icon(SKILL_NAME eSkillName)
{
    CUI_Utils::Set_SkillIcon(m_pIcon, eSkillName);
}

void CHUD_PlagueWeaponBase::OnEnable(void* pArg)
{
    __super::OnEnable(pArg);
}

void CHUD_PlagueWeaponBase::OnDisable()
{
    __super::OnDisable();

    m_pPlagueWeapon_Border.lock()->Set_Enable(false);
    m_pPlagueWeapon_Ready.lock()->Set_Enable(false);
}

void CHUD_PlagueWeaponBase::Free()
{
}
