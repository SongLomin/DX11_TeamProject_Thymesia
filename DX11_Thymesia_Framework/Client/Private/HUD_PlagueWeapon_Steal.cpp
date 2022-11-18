#include "stdafx.h"
#include "HUD_PlagueWeaponBase.h"
#include "HUD_PlagueWeapon_Steal.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "HUD_Hover.h"
#include "HUD_PlagueWeapon_Steal_Icon.h"
#include "Player_ProgressBar.h"
#include "Texture.h"


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


    m_pPlagueWeapon_Border = GAMEINSTANCE->Add_GameObject<CPlayer_ProgressBar>(LEVEL_STATIC, &m_tUIDesc);
    m_pPlagueWeapon_Border.lock()->Set_Texture("HUD_FrameBorder");
    m_pPlagueWeapon_Border.lock()->Set_Depth(0.4f);
    m_pPlagueWeapon_Border.lock()->Set_PassIndex(3);

    m_pPlagueWeapon_Main = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
    m_pPlagueWeapon_Main.lock()->Set_Texture("HUD_PlagueWeapon_Frame_Steal");
    m_pPlagueWeapon_Main.lock()->Set_Depth(0.3f);
    m_pPlagueWeapon_Main.lock()->Set_Size(201.f, 201.f);


    m_pPlagueWeapon_Icon = GAMEINSTANCE->Add_GameObject<CHUD_PlagueWeapon_Steal_Icon>(LEVEL_STATIC, &m_tUIDesc);
    m_pPlagueWeapon_Icon.lock()->Set_Texture("SkillIcon_Axe");
    m_pPlagueWeapon_Icon.lock()->Set_Depth(0.2f);
    m_pPlagueWeapon_Icon.lock()->Set_Size(160.f, 160.f);

    m_pPlagueWeapon_Ready = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
    m_pPlagueWeapon_Ready.lock()->Set_Texture("HUD_PlagueWeapon_Frame_Ready");
    m_pPlagueWeapon_Ready.lock()->Set_Depth(0.1f);
    m_pPlagueWeapon_Ready.lock()->Set_AlphaColor(0.3f);
    m_pPlagueWeapon_Ready.lock()->Set_Enable(false);

    m_pPlagueWeapon_Decoration = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
    m_pPlagueWeapon_Decoration.lock()->Set_Texture("HUD_PlagueWeapon_Frame_Steal_Decoration");
    m_pPlagueWeapon_Decoration.lock()->Set_Depth(0.0f);
    m_pPlagueWeapon_Decoration.lock()->Set_Size(201.f, 201.f);
    

    m_pHover = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_STATIC, &m_tUIDesc);
    m_pHover.lock()->Get_Component<CTexture>().lock()->Use_Texture("HUD_Frame_Hover");

    m_vecChildUI.push_back(m_pPlagueWeapon_Main);
    m_vecChildUI.push_back(m_pPlagueWeapon_Icon);
    m_vecChildUI.push_back(m_pPlagueWeapon_Decoration);



    //HoverDesc
    //HoverFaderDesc
    m_tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
    m_tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
    m_tFaderDesc.fDelayTime = 0.f;
    m_tFaderDesc.fFadeMaxTime = 1.f;
    m_tFaderDesc.vFadeColor = _float4(0, 0, 0, 0.7f);

    m_fCoolTime = 5.f;
    m_fNowTime = 5.f;



    return S_OK;
}

HRESULT CHUD_PlagueWeapon_Steal::Start()
{
    __super::Start();


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

HRESULT CHUD_PlagueWeapon_Steal::Render()
{
    //Render    

    return S_OK;
}


void CHUD_PlagueWeapon_Steal::Free()
{
}
