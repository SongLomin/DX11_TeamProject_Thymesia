#include "stdafx.h"
#include "UI_EvolveMenu_SKillIcon.h"
#include "UI_Utils.h"
#include "UIManager.h"
#include "Shader.h"
#include "EasingComponent_Float.h"

GAMECLASS_C(CUI_EvolveMenu_SKillIcon)
CLONE_C(CUI_EvolveMenu_SKillIcon, CGameObject)

HRESULT CUI_EvolveMenu_SKillIcon::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CUI_EvolveMenu_SKillIcon::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    Set_PassIndex(15);
    SetUp_Component();

    m_fRatio = 0.f;
    return S_OK;
}

HRESULT CUI_EvolveMenu_SKillIcon::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_EvolveMenu_SKillIcon::Tick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();
    __super::Tick(fTimeDelta);

    if (m_pEasingRatio.lock()->Is_Lerping())
    {
        m_fRatio = (m_pEasingRatio.lock()->Get_Lerp());     
    }
}

void CUI_EvolveMenu_SKillIcon::LateTick(_float fTimeDelta)
{
    fTimeDelta = CUI_Utils::UI_TimeDelta();

    __super::LateTick(fTimeDelta);
}

void CUI_EvolveMenu_SKillIcon::SetUp_Component()
{
    m_pEasingRatio = Add_Component<CEasingComponent_Float>();
}

_float CUI_EvolveMenu_SKillIcon::Get_Ratio()
{
    return m_pEasingRatio.lock()->Get_Lerp();
}

void CUI_EvolveMenu_SKillIcon::StartGauge()
{
    m_pEasingRatio.lock()->Set_Lerp(m_fRatio, 1.f, 3.f, EASING_TYPE::SINE_IN, CEasingComponent::ONCE);
}

void CUI_EvolveMenu_SKillIcon::EndGauge()
{
    m_pEasingRatio.lock()->Set_Lerp(m_fRatio, 0.f, 3.f, EASING_TYPE::SINE_OUT, CEasingComponent::ONCE);
}


HRESULT CUI_EvolveMenu_SKillIcon::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();
    
    if (m_iPassIndex == 15)
    {
      
        m_pShaderCom.lock()->Set_RawValue("g_Ratio", &m_fRatio, sizeof(_float));
    }

    return S_OK;
}

void CUI_EvolveMenu_SKillIcon::Free()
{
}
