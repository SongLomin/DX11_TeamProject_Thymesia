#include "stdafx.h"
#include "HUD_PlagueWeapon_Steal_Icon.h"
#include "CustomUI.h"
#include "Texture.h"
#include "Shader.h"
#include "GameManager.h"
#include "Engine_Defines.h"

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

    m_pDissolveTexture = Add_Component<CTexture>();
    m_pDissolveTexture.lock()->Use_Texture("Dissolve_1");

    m_pDissolveMaskTexture = Add_Component<CTexture>();
    m_pDissolveMaskTexture.lock()->Use_Texture("Dissolve_Mask");



    m_fRatio = 0.f;
    m_iPassIndex = 4;

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

    //TODO : Dissolve 아이콘 테스트 코드입니다.
#ifdef _DEBUG
    if (KEY_INPUT(KEY::Z, KEY_STATE::HOLD))
    {
        m_fRatio -= 3 * fTimeDelta;
        if (m_fRatio < 0.f)
            m_fRatio = 0.f;
    }
    if (KEY_INPUT(KEY::X, KEY_STATE::HOLD))
    {
        m_fRatio += 3 * fTimeDelta;
        if (m_fRatio >= 1.f)
            m_fRatio = 1.f;
    }

    if (KEY_INPUT(KEY::NUM0, KEY_STATE::HOLD))
    {
        m_iTextureIndex = 0;
    }
    if (KEY_INPUT(KEY::NUM1, KEY_STATE::HOLD))
    {
        m_iTextureIndex = 1;
    }
    if (KEY_INPUT(KEY::NUM2, KEY_STATE::HOLD))
    {
        m_iTextureIndex = 2;
    }
    if (KEY_INPUT(KEY::NUM3, KEY_STATE::HOLD))
    {
        m_iTextureIndex = 3;
    }
    if (KEY_INPUT(KEY::NUM4, KEY_STATE::HOLD))
    {
        m_iTextureIndex = 4;
    }
#endif


}

void CHUD_PlagueWeapon_Steal_Icon::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

}

HRESULT CHUD_PlagueWeapon_Steal_Icon::Render()
{
    __super::Render();


    return S_OK;
}

HRESULT CHUD_PlagueWeapon_Steal_Icon::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

    m_pDissolveTexture.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DissolveTexture", m_iTextureIndex);

    m_pDissolveMaskTexture.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture");


    return S_OK;
}

void CHUD_PlagueWeapon_Steal_Icon::Free()
{
    __super::Free();
}
