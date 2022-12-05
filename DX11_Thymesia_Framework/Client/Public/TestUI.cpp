#include "stdafx.h"
#include "TestUI.h"
#include "Texture.h"
#include "EasingTransform.h"
#include "Shader.h"

GAMECLASS_C(CTestUI)
CLONE_C(CTestUI, CGameObject)

HRESULT CTestUI::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CTestUI::Initialize(void* pArg)
{
    UI_DESC tUIDesc;

    tUIDesc.fX = g_iWinCX >> 1;
    tUIDesc.fY = g_iWinCY >> 1;

    tUIDesc.fSizeX = 600.f;
    tUIDesc.fSizeY = 35.f;

    __super::Initialize(&tUIDesc);

    Set_Texture("EvolveMenu_Text_SelectHighlight");

    m_pMaskingTextureCom = Add_Component<CTexture>();
    m_pMaskingTextureCom.lock()->Use_Texture("Mask");

    Set_PassIndex(9);

    m_pEasingTransformCom = Add_Component<CEasingTransform>();
    m_vUXOffset = { 0.f,0.f };
    Set_Selected(true);
    
    return S_OK;
}

HRESULT CTestUI::Start()
{
    __super::Start();


    return S_OK;
}

void CTestUI::Tick(_float fTimeDelta)
{
    fTimeDelta *= GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::UI);
    __super::Tick(fTimeDelta);

    m_vUXOffset.x -= 0.08f * fTimeDelta;
 
    if (KEY_INPUT(KEY::X, KEY_STATE::TAP))
    {
        m_pEasingTransformCom.lock()->Set_Lerp_Alpha(1.5f, EASING_TYPE::EXPO_OUT, false, CEasingTransform::JUST_START,
            CEasingTransform::USING_CUSTOM);
    }   
}
void CTestUI::LateTick(_float fTimeDelta)
{
    fTimeDelta *= GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::UI);

    __super::LateTick(fTimeDelta);

    m_pEasingTransformCom.lock()->LateTick(fTimeDelta);

}

HRESULT CTestUI::Render()
{
    __super::Render();

    return S_OK;
}

void CTestUI::Set_Selected(_bool bSelected)
{
   

}

HRESULT CTestUI::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

    m_pMaskingTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", 0);
    
    
    m_pShaderCom.lock()->Set_RawValue("g_UVOffset", &m_vUXOffset, sizeof(_float2));

    _float fRatio = (m_pEasingTransformCom.lock()->Get_Lerp().x);
    m_pShaderCom.lock()->Set_RawValue("g_Ratio", &fRatio, sizeof(_float));


    return S_OK;
}
