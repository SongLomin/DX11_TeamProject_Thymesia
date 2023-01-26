#include "stdafx.h"
#include "UI_EvolveMenu_SKillIcon.h"
#include "UI_Utils.h"
#include "UIManager.h"
#include "Shader.h"
#include "EasingComponent_Float.h"
#include "UI_Effect_MagicCircle.h"


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

void CUI_EvolveMenu_SKillIcon::Call_OnEasingFloatConditionEvent(_uint iEventNumber)
{
    weak_ptr<CUI_Effect_MagicCircle> pEffect = GAMEINSTANCE->Get_GameObject_UseMemoryPool<CUI_Effect_MagicCircle>(LEVEL_STATIC);

    if (!pEffect.lock())
        pEffect = GAMEINSTANCE->Add_GameObject<CUI_Effect_MagicCircle>(LEVEL_STATIC);
 
    pEffect.lock()->Play(GetPos(), (_float)(iEventNumber + 1));

    GAMEINSTANCE->PlaySound2D("Skill_Circle.ogg", iEventNumber * 0.1f);

    if (iEventNumber == 7)
    {
     //   Add_Shaking(0.5, (_float)(iEventNumber + 1) * 2);
    }
}

void CUI_EvolveMenu_SKillIcon::SetUp_Component()
{
    m_pEasingRatio = Add_Component<CEasingComponent_Float>();

    m_pEasingRatio.lock()->Callback_OnConditionEvent += bind(&CUI_EvolveMenu_SKillIcon::Call_OnEasingFloatConditionEvent, this, placeholders::_1);
}

_float CUI_EvolveMenu_SKillIcon::Get_Ratio()
{
    return m_pEasingRatio.lock()->Get_Lerp();
}

void CUI_EvolveMenu_SKillIcon::StartGauge()
{
    m_pEasingRatio.lock()->Set_Lerp(m_fRatio, 1.f, 3.f, EASING_TYPE::LINEAR, CEasingComponent::ONCE);
    m_pEasingRatio.lock()->Add_Condition(0.0f, EASING_FLOAT_CONDITION::BIGGER, 1);
    m_pEasingRatio.lock()->Add_Condition(0.2f, EASING_FLOAT_CONDITION::BIGGER, 2);
    m_pEasingRatio.lock()->Add_Condition(0.4f, EASING_FLOAT_CONDITION::BIGGER, 3);
    m_pEasingRatio.lock()->Add_Condition(0.6f, EASING_FLOAT_CONDITION::BIGGER, 5);
    //m_pEasingRatio.lock()->Add_Condition(0.9f, EASING_FLOAT_CONDITION::BIGGER, 7);
}

void CUI_EvolveMenu_SKillIcon::EndGauge()
{
    m_pEasingRatio.lock()->Set_Lerp(m_fRatio, 0.f, 1.f, EASING_TYPE::SINE_OUT, CEasingComponent::ONCE);

    Add_Shaking(0.3f, 1.f);

    m_pEasingRatio.lock()->Clear_ConditionEventList();
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
