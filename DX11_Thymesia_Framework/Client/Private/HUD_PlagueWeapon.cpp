#include "stdafx.h"
#include "HUD_PlagueWeapon.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "CustomUI.h"
#include "Player_ProgressBar.h"


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

    m_pPlagueWeapon_Border = GAMEINSTANCE->Add_GameObject<CPlayer_ProgressBar>(LEVEL_STATIC, &m_tUIDesc);
    m_pPlagueWeapon_Border.lock()->Set_Texture("HUD_FrameBorder");
    m_pPlagueWeapon_Border.lock()->Set_Depth(0.4f);
    m_pPlagueWeapon_Border.lock()->Set_PassIndex(10);

    m_pPlagueWeapon_Main = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
    m_pPlagueWeapon_Main.lock()->Set_Texture("HUD_PlagueWeapon_Frame");
    m_pPlagueWeapon_Main.lock()->Set_Depth(0.3f);

    m_pPlagueWeapon_Icon = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
    m_pPlagueWeapon_Icon.lock()->Set_Texture("SkillIcon_Axe");
    m_pPlagueWeapon_Icon.lock()->Set_Depth(0.2f);
    m_pPlagueWeapon_Icon.lock()->Set_Size(160.f, 160.f);

    GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::BATTLEUI, Cast<CGameObject>(m_pPlagueWeapon_Main));
    GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::BATTLEUI, Cast<CGameObject>(m_pPlagueWeapon_Border));
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

}

void CHUD_PlagueWeapon::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

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
}

void CHUD_PlagueWeapon::Free()
{
}
