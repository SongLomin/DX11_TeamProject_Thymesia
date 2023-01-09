#include "stdafx.h"
#include "UI_EffectBase.h"
#include "Texture.h"
#include "Shader.h"
#include "EasingComponent_Transform.h"
#include "EasingComponent_Alpha.h"
#include "EasingComponent_Bezier.h"
#include "EasingComponent_Float.h"


GAMECLASS_C(CUI_EffectBase)
CLONE_C(CUI_EffectBase, CGameObject)

HRESULT CUI_EffectBase::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;

}

HRESULT CUI_EffectBase::Initialize(void* pArg)
{
    CUI::Initialize(pArg);
    
    m_eRenderGroup = RENDERGROUP::RENDER_AFTER_UI;

    m_pShaderCom.lock()->Set_ShaderInfo(
        TEXT("Shader_UIEffect"),
        VTXTEX_DECLARATION::Element,
        VTXTEX_DECLARATION::iNumElements);

    Set_Texture("UI_None");

    Init_UI();

    SetUp_Component();
    
    return S_OK;
}

HRESULT CUI_EffectBase::Start()
{
    __super::Start();

    return S_OK;
}

void CUI_EffectBase::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    if (m_pEasingAlpha.lock()->Is_Lerping())
    {
        Set_AlphaColor(m_pEasingAlpha.lock()->Get_Lerp());
    }
    if (m_pEasingTransform.lock()->Is_Lerping())
    {
        _float2 fPos = m_pEasingTransform.lock()->Get_Lerp();
        Set_UIPosition(fPos.x, fPos.y);
    }
    if (m_pEasingSizing.lock()->Is_Lerping())
    {
        _float2 fSize = m_pEasingSizing.lock()->Get_Lerp();
        Set_Size(fSize.x, fSize.y);
    }
}

void CUI_EffectBase::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

void CUI_EffectBase::SetMaskIndex(_uint iIndex)
{
    m_iMaskIndex = iIndex;
}

HRESULT CUI_EffectBase::SetUp_ShaderResource()
{
    __super::SetUp_ShaderResource();

    m_pMask.lock()->Set_ShaderResourceView(m_pShaderCom, "g_MaskTexture", m_iMaskIndex);
    
    m_pShaderCom.lock()->Set_RawValue("g_UVOffset", &m_fUVOffset, sizeof(_float2));

    return S_OK;
}

void CUI_EffectBase::Init_UI()
{
    m_fAlphaColor = 1.f;
    m_fUVOffset = { 0.f, 0.f };
    Set_Enable(false);
}


void CUI_EffectBase::SetUp_Component()
{
    m_pEasingAlpha = Add_Component<CEasingComponent_Alpha>();
    m_pEasingFloat = Add_Component<CEasingComponent_Float>();
    m_pEasingRotation = Add_Component<CEasingComponent_Float>();
    m_pEasingSizing = Add_Component<CEasingComponent_Transform>();
    m_pEasingTransform = Add_Component<CEasingComponent_Transform>();

    m_pMask = Add_Component<CTexture>();

    m_pMask.lock()->Use_Texture("UVMask");
}

