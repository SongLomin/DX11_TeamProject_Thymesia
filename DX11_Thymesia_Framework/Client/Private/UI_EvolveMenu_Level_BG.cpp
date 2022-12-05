#include "stdafx.h"
#include "UI_EvolveMenu_Level_BG.h"
#include "CustomUI.h"
#include "Shader.h"
#include "Texture.h"


HRESULT CUI_EvolveMenu_Level_BG::Initialize(void* pArg)
{
    __super::Initialize(pArg);


    Set_Texture("PauseMenu_Background1");
    
    m_pUVAnimMask = Add_Component<CTexture>();
    m_pUVAnimMask.lock()->Use_Texture("UI_Evolve_Level_BG_UVAnimMask");

    m_fUVOffset = {0.f, 0.f};
    m_fUVOffsetAmount = 0.07f;

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

    m_fUVOffset.y += (m_fUVOffsetAmount * fTimeDelta);

}

void CUI_EvolveMenu_Level_BG::LateTick(_float fTimeDelta)
{
    fTimeDelta *= GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::UI);


    __super::LateTick(fTimeDelta);

}

HRESULT CUI_EvolveMenu_Level_BG::Render()
{
    __super::Render();


    return S_OK;
}

void CUI_EvolveMenu_Level_BG::Set_Texture(const _char* sKey)
{
    //¾²Áö¸¶
}

HRESULT CUI_EvolveMenu_Level_BG::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

    m_pTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture");
    m_pShaderCom.lock()->Set_RawValue("g_UVOffset", &m_fUVOffset, sizeof(_float2));

    return S_OK;
}
