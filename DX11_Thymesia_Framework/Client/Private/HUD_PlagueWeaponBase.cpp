#include "stdafx.h"
#include "HUD_PlagueWeaponBase.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "CustomUI.h"
#include "Player_ProgressBar.h"
#include "HUD_Hover.h"
#include "CustomUI.h"


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
    
    if (m_bIsCoolTime)
    {
        m_fNowTime += fTimeDelta;
        if (m_fCoolTime <= m_fNowTime)//쿨타임이 다 찼을 시.
        {
            m_bIsCoolTime = false;
            m_pPlagueWeapon_Border.lock()->Set_Enable(false);
            m_pPlagueWeapon_Ready.lock()->Set_Enable(false);
            //호버 이벤트 발생

            m_pHover.lock()->CallBack_FadeEnd -= bind(&CHUD_PlagueWeaponBase::Call_HoverFadeEnd, this, placeholders::_1);
            CHUD_Hover::HUDHOVERDESC tHoverDesc;
            tHoverDesc.bSizeChange = true;
            tHoverDesc.fSizeMag = 0.2f;
            tHoverDesc.eType = CHUD_Hover::HUD_HOVER_ANIMATION_FROM_ALPHA;
            m_pHover.lock()->CallBack_FadeEnd += bind(&CHUD_PlagueWeaponBase::Call_HoverFadeEnd, this, placeholders::_1);
            m_pHover.lock()->Init_Fader(m_tFaderDesc, tHoverDesc);

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

void CHUD_PlagueWeaponBase::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

}

HRESULT CHUD_PlagueWeaponBase::Render()
{
    

    return S_OK;
}

void CHUD_PlagueWeaponBase::Call_UseSkill()
{
    //TODO : 역병무기 쿨타임 테스트 입니다.
    Add_Shaking(2.f, 5.f);

    m_fCoolTime = 5.f;
    m_fNowTime = 0.f;
    m_bIsCoolTime = true;
    m_pPlagueWeapon_Border.lock()->Set_Enable(true);
    m_pPlagueWeapon_Ready.lock()->Set_Enable(true);
}

void CHUD_PlagueWeaponBase::Call_HoverFadeEnd(FADER_TYPE eFaderType)
{
    m_pHover.lock()->Set_UIDesc(m_tUIDesc);
    m_pHover.lock()->Set_Enable(false);

}

void CHUD_PlagueWeaponBase::Free()
{
}
