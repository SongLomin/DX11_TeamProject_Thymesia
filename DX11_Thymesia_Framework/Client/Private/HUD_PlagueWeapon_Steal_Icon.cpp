#include "stdafx.h"
#include "HUD_PlagueWeapon_Steal_Icon.h"
#include "CustomUI.h"
#include "DissolveUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"
#include "EasingComponent_Float.h"



GAMECLASS_C(CHUD_PlagueWeapon_Steal_Icon);
CLONE_C(CHUD_PlagueWeapon_Steal_Icon, CGameObject);

HRESULT CHUD_PlagueWeapon_Steal_Icon::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CHUD_PlagueWeapon_Steal_Icon::Initialize(void* pArg)
{
    __super::Initialize(pArg);


    m_pMask = Add_Component<CTexture>();

    Set_Texture("None");

    m_pMask.lock()->Use_Texture("MaskColor_Vertical");

    m_iPassIndex = 5;

    SetUp_Component();


    return S_OK;
}

HRESULT CHUD_PlagueWeapon_Steal_Icon::Start()
{
    __super::Start();

    return S_OK;
}

void CHUD_PlagueWeapon_Steal_Icon::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    if (m_pEasingFloat.lock()->Is_Lerping())
    {
        m_fRatio = m_pEasingFloat.lock()->Get_Lerp();
    }

}

void CHUD_PlagueWeapon_Steal_Icon::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

}

HRESULT CHUD_PlagueWeapon_Steal_Icon::Render(ID3D11DeviceContext* pDeviceContext)
{
    __super::Render(pDeviceContext);

    return S_OK;
}

HRESULT CHUD_PlagueWeapon_Steal_Icon::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();
   
    m_pMask.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture");

    return S_OK;
}

void CHUD_PlagueWeapon_Steal_Icon::Free()
{
   
}
