#include "stdafx.h"
#include "UI_EvolveMenu_Level_BG.h"
#include "CustomUI.h"
#include "Shader.h"
#include "Texture.h"


GAMECLASS_C(CUI_EvolveMenu_Level_BG)
CLONE_C(CUI_EvolveMenu_Level_BG, CGameObject)

HRESULT CUI_EvolveMenu_Level_BG::Initialize(void* pArg)
{
    __super::Initialize(pArg);


    Set_Texture("PauseMenu_Background1");
    
    m_pUVAnimMask = Add_Component<CTexture>();
    m_pUVAnimMask.lock()->Use_Texture("UI_EvolveMenu_Level_BG_Mask");


    m_pSrcTexture = Add_Component<CTexture>();
    m_pSrcTexture.lock()->Use_Texture("UI_Corvus");

    m_fUVOffset = {0.f, 0.f};
    m_fUVOffsetAmount = 0.09f;

    m_iPassIndex = 9;

    Set_AlphaColor(0.8f);
    Set_Depth(0.8f);

    return S_OK;
}

HRESULT CUI_EvolveMenu_Level_BG::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_EvolveMenu_Level_BG::Tick(_float fTimeDelta)
{
    fTimeDelta *= GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::UI);

    __super::Tick(fTimeDelta);

    m_fUVOffset.x -= (m_fUVOffsetAmount * fTimeDelta);

}

void CUI_EvolveMenu_Level_BG::LateTick(_float fTimeDelta)
{
    fTimeDelta *= GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::UI);


    __super::LateTick(fTimeDelta);

}

HRESULT CUI_EvolveMenu_Level_BG::Render(ID3D11DeviceContext* pDeviceContext)
{
    __super::Render(pDeviceContext);

    return S_OK;
}

void CUI_EvolveMenu_Level_BG::Set_Texture(const _char* sKey)
{
    //¾²Áö¸¶
    __super::Set_Texture(sKey);
}

HRESULT CUI_EvolveMenu_Level_BG::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

    m_pUVAnimMask.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture");
    m_pSrcTexture.lock()->Set_ShaderResourceView(m_pShaderCom, "g_SrcTexture");

    m_pShaderCom.lock()->Set_RawValue("g_UVOffset", &m_fUVOffset, sizeof(_float2));




    return S_OK;
}
