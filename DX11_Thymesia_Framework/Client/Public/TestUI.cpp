#include "stdafx.h"
#include "TestUI.h"
#include "Texture.h"
#include "EasingComponent_Transform.h"
#include "EasingComponent_Alpha.h"

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

    tUIDesc.fSizeX = 128.f;
    tUIDesc.fSizeY = 128.f;

    __super::Initialize(&tUIDesc);

    Set_Texture("HighLight");

    m_pMaskingTextureCom = Add_Component<CTexture>();
    m_pMaskingTextureCom.lock()->Use_Texture("Test");

    Set_PassIndex(10);

    m_pEasingTransformCom = Add_Component<CEasingComponent_Alpha>();
    m_vUXOffset = { 0.f,0.f };
    Set_Selected(true);

  

    m_tTest.bAlways = false;
    m_tTest.bCenterAlign = true;
    m_tTest.fRotation = 0.f;
    m_tTest.vColor = _float4(1.f, 1.f, 1.f, 1.f);
    m_tTest.vPosition = _float2(500, 500);
    m_tTest.vScale = _float2(2.f, 2.f);


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
 
     
}
void CTestUI::LateTick(_float fTimeDelta)
{
    fTimeDelta *= GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::UI);

    __super::LateTick(fTimeDelta);

    m_pEasingTransformCom.lock()->LateTick(fTimeDelta);

    //m_tTest.szText = to_wstring((_uint)(m_pEasingTransformCom.lock()->Get_Lerp().x * 8));

    GAMEINSTANCE->Add_Text((_uint)FONT_INDEX::PRETENDARD, m_tTest);

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
  
    _float fRatio =  (((m_pEasingTransformCom.lock()->Get_Lerp()) * 8.f));
    m_pShaderCom.lock()->Set_RawValue("g_Ratio", &fRatio, sizeof(_float));

    return S_OK;
}
