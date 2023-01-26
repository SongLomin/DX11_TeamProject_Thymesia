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

    Set_PassIndex(0);

    m_pEasingTransformCom = Add_Component<CEasingComponent_Alpha>();
    m_iDissolveIndex = 0;
   

    m_bSelected = false;
    
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

HRESULT CUI_EvolveTalent_Active::Render(ID3D11DeviceContext* pDeviceContext)
{
    __super::Render(pDeviceContext);

    return S_OK;
}

void CUI_EvolveTalent_Active::Set_Click(_bool bSelected)
{
    Set_PassIndex(4);
    if (bSelected)
    {
        Set_Texture("EvolveMenu_PW_Active");

        m_bSelected = true;
        m_pEasingTransformCom.lock()->Set_Lerp(0.f, 1.f, 1.f, EASING_TYPE::CIRC_IN,
            CEasingComponent::ONCE, false);

        m_iDissolveIndex = 0;
        m_pEasingTransformCom.lock()->Callback_LerpEnd += bind(&CUI_EvolveTalent_Active::Call_SelectedTrueEnd, this);
    }
    else
    {
        m_bSelected = false;
        m_pEasingTransformCom.lock()->Set_Lerp(1.f, 0.f, 1.f, EASING_TYPE::CIRC_OUT,
            CEasingComponent::ONCE, false);

        m_pEasingTransformCom.lock()->Callback_LerpEnd += bind(&CUI_EvolveTalent_Active::Call_SelectedFalseEnd, this);

        m_iDissolveIndex = 1;
    }
}

void CUI_EvolveTalent_Active::Animation_MouseOver()
{
    if (m_pEasingTransformCom.lock()->Is_Lerping())
        return;

    if (m_bSelected)
    {
        Set_Texture("EvolveMenu_PW_Active_Hover");
       // m_pEasingTransformCom.lock()->Set_Lerp(m_fAlphaColor, 1.f, 1.f, EASING_TYPE::CIRC_OUT,
        //    CEasingComponent::ONCE, false);
    }
    else
    {
        Set_Texture("EvolveMenu_PW_Frame_Hover");
        //m_pEasingTransformCom.lock()->Set_Lerp(m_fAlphaColor, 1.f, 1.f, EASING_TYPE::CIRC_OUT,
        //    CEasingComponent::ONCE, false);
    }

}

void CUI_EvolveTalent_Active::Animation_MouseOut()
{
    if (m_pEasingTransformCom.lock()->Is_Lerping())
        return;

    //m_pEasingTransformCom.lock()->Stop();

    if (m_bSelected)
    {
        Set_Texture("EvolveMenu_PW_Active");
    }
    else
    {
        Set_Texture("None");

    }
}

HRESULT CUI_EvolveTalent_Active::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

    m_pMaskingTextureCom.lock()->Set_ShaderResourceView(m_pShaderCom, "g_DissolveTexture", m_iDissolveIndex);

    _float  m_fRange = m_pEasingTransformCom.lock()->Get_Lerp();

    m_pShaderCom.lock()->Set_RawValue("g_Ratio", &m_fRange, sizeof(_float));

    return S_OK;
}

void CUI_EvolveTalent_Active::Call_SelectedTrueEnd()//활성화가 끝나면
{
    Set_PassIndex(0);
}

void CUI_EvolveTalent_Active::Call_SelectedFalseEnd()//비활성화가 끝나면
{
    Set_Texture("None");

    Set_PassIndex(0);
    m_fAlphaColor = 1.f;
}

