#include "stdafx.h"
#include "HUD_PlagueWeapon_Main.h"
#include "HUD_PlagueWeaponBase.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "CustomUI.h"
#include "ProgressBar.h"
#include "HUD_Hover.h"
#include "Status_Player.h"
#include "Engine_Macro.h"
#include "PlayerSkill_System.h"



GAMECLASS_C(CHUD_PlagueWeapon_Main);
CLONE_C(CHUD_PlagueWeapon_Main, CGameObject);


HRESULT CHUD_PlagueWeapon_Main::Initialize_Prototype()
{
    __super::Initialize_Prototype();
   

    return S_OK;
}

HRESULT CHUD_PlagueWeapon_Main::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_pPlagueWeapon_Border = GAMEINSTANCE->Add_GameObject<CProgressBar>(LEVEL_STATIC, &m_tUIDesc);
    m_pPlagueWeapon_Border.lock()->Set_Texture("HUD_FrameBorder");
    m_pPlagueWeapon_Border.lock()->Set_Depth(0.4f);
    m_pPlagueWeapon_Border.lock()->Set_PassIndex(3);
    m_pPlagueWeapon_Border.lock()->Set_Enable(false);
    m_pPlagueWeapon_Border.lock()->Set_RenderGroup(RENDERGROUP::RENDER_BEFOREUI);

    m_pPlagueWeapon_Main = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
    m_pPlagueWeapon_Main.lock()->Set_Texture("HUD_PlagueWeapon_Frame");
    m_pPlagueWeapon_Main.lock()->Set_Depth(0.3f);

    m_pIcon = GAMEINSTANCE->Add_GameObject<CCustomUI>(LEVEL_STATIC, &m_tUIDesc);
    m_pIcon.lock()->Set_Texture("None");
    m_pIcon.lock()->Set_Depth(0.2f);
       
    _float fSize = m_tUIDesc.fSizeX * 1.72f;

    m_pIcon.lock()->Set_Size(fSize, fSize);

    m_pPlagueWeapon_Ready = GAMEINSTANCE->Add_GameObject<CProgressBar>(LEVEL_STATIC, &m_tUIDesc);
    m_pPlagueWeapon_Ready.lock()->Set_Texture("HUD_PlagueWeapon_Frame_Ready");
    m_pPlagueWeapon_Ready.lock()->Set_Depth(0.1f);
    m_pPlagueWeapon_Ready.lock()->Set_PassIndex(3);
    m_pPlagueWeapon_Ready.lock()->Set_AlphaColor(0.3f);
    m_pPlagueWeapon_Ready.lock()->Set_Enable(false);
    m_pPlagueWeapon_Ready.lock()->Set_RenderGroup(RENDERGROUP::RENDER_BEFOREUI);


    m_pHover = GAMEINSTANCE->Add_GameObject<CHUD_Hover>(LEVEL_STATIC, &m_tUIDesc);
    m_pHover.lock()->Get_Component<CTexture>().lock()->Use_Texture("HUD_Frame_Hover");
    m_pHover.lock()->Set_RenderGroup(RENDERGROUP::RENDER_BEFOREUI);


    m_eRenderGroup = RENDERGROUP::RENDER_BEFOREUI;


    Add_Child(m_pPlagueWeapon_Main);
    Add_Child(m_pIcon);

    //HoverDesc
    //HoverFaderDesc
    m_tFaderDesc.eLinearType = LINEAR_TYPE::LNIEAR;
    m_tFaderDesc.eFaderType = FADER_TYPE::FADER_OUT;
    m_tFaderDesc.fDelayTime = 0.f;
    m_tFaderDesc.fFadeMaxTime = 1.f;
    m_tFaderDesc.vFadeColor = _float4(0, 0, 0, 0.7f);

    return S_OK;
}

HRESULT CHUD_PlagueWeapon_Main::Start()
{
    __super::Start();

    Bind_Player();

    Call_OnChangeSkill
    (
        GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->
        Get_Component<CPlayerSkill_System>().lock()->Get_EquipSkill(m_eSkillSocketType)
    );
    
    return S_OK;
}

void CHUD_PlagueWeapon_Main::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

}

void CHUD_PlagueWeapon_Main::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CHUD_PlagueWeapon_Main::Render(ID3D11DeviceContext* pDeviceContext)
{
    //__super::Render(pDeviceContext);

    return S_OK;
}

void CHUD_PlagueWeapon_Main::Set_SocketType(_uint iSocket_Type)
{
    m_eSkillSocketType = CPlayerSkill_System::SOCKET_TYPE(iSocket_Type);
}




