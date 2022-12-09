#include "stdafx.h"
#include "UI_EvolveTalent_Active.h"
#include "Texture.h"
#include "EasingComponent_Transform.h"
#include "EasingComponent_Alpha.h"

#include "Shader.h"

GAMECLASS_C(CUI_EvolveTalent_Active)
CLONE_C(CUI_EvolveTalent_Active, CGameObject)

HRESULT CUI_EvolveTalent_Active::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}

HRESULT CUI_EvolveTalent_Active::Initialize(void* pArg)
{
    __super::Initialize(&pArg);

    Set_Texture("None");

    m_pMaskingTextureCom = Add_Component<CTexture>();
    m_pMaskingTextureCom.lock()->Use_Texture("Dissolve_1");

    Set_PassIndex(4);

    m_pEasingTransformCom = Add_Component<CEasingComponent_Alpha>();
  
   
    return S_OK;
}

HRESULT CUI_EvolveTalent_Active::Start()
{
    __super::Start();


    return S_OK;
}

void CUI_EvolveTalent_Active::Tick(_float fTimeDelta)
{
    fTimeDelta *= GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::UI);
    __super::Tick(fTimeDelta);

 
}
void CUI_EvolveTalent_Active::LateTick(_float fTimeDelta)
{
    fTimeDelta *= GAMEINSTANCE->Get_TimeScale((_uint)TIMESCALE_LAYER::UI);

    __super::LateTick(fTimeDelta);

}

HRESULT CUI_EvolveTalent_Active::Render()
{
    __super::Render();

    return S_OK;
}

void CUI_EvolveTalent_Active::Set_Selected(_bool bSelected)
{
    if (bSelected)
    {
        m_pEasingTransformCom.lock()->Set_Lerp(0.f, 1.f, 1.f, EASING_TYPE::CIRC_IN,
            CEasingComponent::ONCE, false);
    }
    else
    {
        m_pEasingTransformCom.lock()->Set_Lerp(1.f, 0.f, 1.f, EASING_TYPE::CIRC_IN,
            CEasingComponent::ONCE, false);
    }
}

HRESULT CUI_EvolveTalent_Active::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

    m_pMaskingTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DissolveTexture", 0);

    _float  m_fRange = m_pEasingTransformCom.lock()->Get_Lerp();

    m_pShaderCom.lock()->Set_RawValue("g_Ratio", &m_fRange, sizeof(_float));

    return S_OK;
}
