#include "stdafx.h"
#include "UI_EvolveMenu_SelectDecoration.h"
#include "Texture.h"
#include "EasingComponent.h"
#include "EasingComponent_Alpha.h"
#include "Shader.h"

GAMECLASS_C(CUI_EvolveMenu_SelectDecoration )
CLONE_C(CUI_EvolveMenu_SelectDecoration , CGameObject)


HRESULT CUI_EvolveMenu_SelectDecoration ::Initialize(void* pArg)
{
    UI_DESC tUIDesc;

    tUIDesc.fX = g_iWinCX >> 1;
    tUIDesc.fY = g_iWinCY >> 1;

    tUIDesc.fSizeX = 64.f;
    tUIDesc.fSizeY = 64.f;

    __super::Initialize(&tUIDesc);

    Set_Texture("HighLight");

    m_pMaskingTextureCom = Add_Component<CTexture>();
    m_pMaskingTextureCom.lock()->Use_Texture("Test");

    Set_PassIndex(10);

    m_pEasingAlphaCom = Add_Component<CEasingComponent_Alpha>();
    m_vUXOffset = { 0.f,0.f };

    return S_OK;
}

HRESULT CUI_EvolveMenu_SelectDecoration ::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_EvolveMenu_SelectDecoration ::Tick(_float fTimeDelta)
{
    fTimeDelta *= GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::UI);
    __super::Tick(fTimeDelta);

}
void CUI_EvolveMenu_SelectDecoration ::LateTick(_float fTimeDelta)
{
    fTimeDelta *= GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::UI);

    __super::LateTick(fTimeDelta);

}

HRESULT CUI_EvolveMenu_SelectDecoration ::Render(ID3D11DeviceContext* pDeviceContext)
{
    __super::Render(pDeviceContext);

    return S_OK;
}

void CUI_EvolveMenu_SelectDecoration::Start_Animation()
{
    m_pEasingAlphaCom.lock()->Set_Lerp(0.f,1.f, 1.f, EASING_TYPE::CIRC_OUT, CEasingComponent::ONCE, true);
}

HRESULT CUI_EvolveMenu_SelectDecoration ::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

    m_pMaskingTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", 0);
    _float fRatio = 8.f - (((m_pEasingAlphaCom.lock()->Get_Lerp()) * 8.f));
    m_pShaderCom.lock()->Set_RawValue("g_Ratio", &fRatio, sizeof(_float));

    return S_OK;
}
