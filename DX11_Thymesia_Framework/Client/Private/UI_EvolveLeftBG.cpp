#include "stdafx.h"
#include "UI_EvolveLeftBG.h"
#include "CustomUI.h"
#include "Shader.h"
#include "Texture.h"


GAMECLASS_C(CUI_EvolveLeftBG)
CLONE_C(CUI_EvolveLeftBG, CGameObject)

HRESULT CUI_EvolveLeftBG::Initialize(void* pArg)
{
    __super::Initialize(pArg);


    Set_Texture("EvolveMenu_LeftBG");

    m_pUVAnimMask = Add_Component<CTexture>();
    m_pUVAnimMask.lock()->Use_Texture("EvolveMenu_LeftBG_Mask");


    m_fUVOffset = { 0.f, 0.f };
    m_fUVOffsetAmount = 0.03f;

    m_iPassIndex = 11;

    Set_AlphaColor(0.7f);
   // Set_Depth(0.8f);

    m_fNoneAnimRatio = { 0.f, 0.1f };

    return S_OK;
}

HRESULT CUI_EvolveLeftBG::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_EvolveLeftBG::Tick(_float fTimeDelta)
{
    fTimeDelta *= GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::UI);

    __super::Tick(fTimeDelta);

    m_fUVOffset.y -= (m_fUVOffsetAmount * fTimeDelta);

}

void CUI_EvolveLeftBG::LateTick(_float fTimeDelta)
{
    fTimeDelta *= GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::UI);


    __super::LateTick(fTimeDelta);

}

HRESULT CUI_EvolveLeftBG::Render(ID3D11DeviceContext* pDeviceContext)
{
    __super::Render(pDeviceContext);

    return S_OK;
}

void CUI_EvolveLeftBG::Set_Texture(const _char* sKey)
{
    //¾²Áö¸¶
    __super::Set_Texture(sKey);
}

HRESULT CUI_EvolveLeftBG::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

    m_pUVAnimMask.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture");

    m_pShaderCom.lock()->Set_RawValue("g_UVOffset", &m_fUVOffset, sizeof(_float2));

    m_pShaderCom.lock()->Set_RawValue("g_NoneAnimRatio", &m_fNoneAnimRatio, sizeof(_float2));

    return S_OK;
}
