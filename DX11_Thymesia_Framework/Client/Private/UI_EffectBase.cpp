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
    __super::Initialize(pArg);

    m_pShaderCom.lock()->Set_ShaderInfo(
        TEXT("Shader_UIEffect"),
        VTXTEX_DECLARATION::Element,
        VTXTEX_DECLARATION::iNumElements);

    Set_Texture("UI_None");

    Init_UIEffect();

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

    if (m_eAnimState == UI_EFFECT_ANIM_STATE::STOP)
    {
        return;
    }

    //현재 클립의 애니메이션이 끝났다.
    if (m_fCurrentClipTime < 0.f)
    {
        Callback_AnimaitonClipEnd(m_iIndex);

        if (m_iIndex >= m_vecUIClipDesc.size())//클립이 다 돌았다면.
        {
            if (m_eAnimState == UI_EFFECT_ANIM_STATE::REPEAT)
            {
                m_iIndex = -1;
                NextClip();
            }
            else
            {
                Init_UIEffect();
            }
            Callback_AnimaitonEnd();
        }
    }
    m_fCurrentClipTime -= fTimeDelta;
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
        Set_UIPosition(fSize.x, fSize.y);
    }
}

void CUI_EffectBase::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

void CUI_EffectBase::ExecuteCurrentClip(UICLIPDESC tagClipDesc)
{
    if (tagClipDesc._eEffectCondition | (_ulong)UI_EFFECT_CONDITION::UI_FROM_ORIGIN_ALPHA)
    {
        m_pEasingAlpha.lock()->Set_Lerp(
            Get_AlphaColor(), 
            tagClipDesc._fTargetAlpha, 
            tagClipDesc._fClipTime,
            tagClipDesc._eAlphaEasingType, 
            CEasingComponent::ONCE, true);
    }
    else
    {
        m_pEasingAlpha.lock()->Set_Lerp(
            tagClipDesc._fStartAlpha, 
            tagClipDesc._fTargetAlpha, 
            tagClipDesc._fClipTime, 
            tagClipDesc._eAlphaEasingType, 
            CEasingComponent::ONCE, true);

    }
    if (tagClipDesc._eEffectCondition | (_ulong)UI_EFFECT_CONDITION::UI_FROM_ORIGIN_POS)
    {
        m_pEasingTransform.lock()->Set_Lerp(
            GetPos(),
            tagClipDesc._fTargetPos,
            tagClipDesc._fClipTime,
            tagClipDesc._eAlphaEasingType,
            CEasingComponent::ONCE, true);
    }
    else
    {
        m_pEasingTransform.lock()->Set_Lerp(
            tagClipDesc._fStartPos,
            tagClipDesc._fTargetPos,
            tagClipDesc._fClipTime,
            tagClipDesc._eAlphaEasingType,
            CEasingComponent::ONCE, true);
    }
    if (tagClipDesc._eEffectCondition | (_ulong)UI_EFFECT_CONDITION::UI_FROM_ORIGIN_ROTATE)
    {
        //m_pEasingRotation.lock()->Set_Lerp(
        //    GetPos(),
        //    tagClipDesc._fTargetPos,
        //    tagClipDesc._fClipTime,
        //    tagClipDesc._eAlphaEasingType,
        //    CEasingComponent::ONCE, true);
    }
    else
    {

    }
    if (tagClipDesc._eEffectCondition | (_ulong)UI_EFFECT_CONDITION::UI_FROM_ORIGIN_SIZE)
    {
        m_pEasingSizing.lock()->Set_Lerp(
            Get_Size(),
            tagClipDesc._fTargetSize,
            tagClipDesc._fClipTime,
            tagClipDesc._eAlphaEasingType,
            CEasingComponent::ONCE, true);
    }
    else
    {
        m_pEasingSizing.lock()->Set_Lerp(
            tagClipDesc._fStartSize,
            tagClipDesc._fTargetSize,
            tagClipDesc._fClipTime,
            tagClipDesc._eAlphaEasingType,
            CEasingComponent::ONCE, true);
    }
}

void CUI_EffectBase::Init_UIEffect()
{
    m_iPassIndex = 0;
    m_fAlphaColor = 1.f;
    m_iIndex = 0;
    m_fCurrentClipTime = 0.f;
    m_eAnimState = UI_EFFECT_ANIM_STATE::STOP;
    Set_Enable(false);
}

void CUI_EffectBase::Add_Clip(UICLIPDESC tUIClipDesc)
{
    m_vecUIClipDesc.push_back(tUIClipDesc);
}

void CUI_EffectBase::Play()
{
    Set_Enable(true);

    Init_UIEffect();
    m_eAnimState = UI_EFFECT_ANIM_STATE::PLAY;

    ExecuteCurrentClip(m_vecUIClipDesc[m_iIndex]);
}

void CUI_EffectBase::Stop()
{
    m_eAnimState = UI_EFFECT_ANIM_STATE::STOP;

    Init_UIEffect();

}

void CUI_EffectBase::SetUp_Component()
{
    m_pEasingAlpha = Add_Component<CEasingComponent_Alpha>();
    m_pEasingFloat = Add_Component<CEasingComponent_Float>();
    m_pEasingRotation = Add_Component<CEasingComponent_Float>();
    m_pEasingSizing = Add_Component<CEasingComponent_Transform>();
    m_pEasingTransform = Add_Component<CEasingComponent_Transform>();
}

void CUI_EffectBase::SetUp_Animaiton()
{
    //TODO : CUI_CustomEffect로 사용되지 않는 UI들은 여기서 애니메이션을 세팅해주세요.

}
void CUI_EffectBase::NextClip()
{
    m_iIndex++;
    m_fCurrentClipTime = m_vecUIClipDesc[m_iIndex]._fClipTime;

    ExecuteCurrentClip(m_vecUIClipDesc[m_iIndex]);
}

void CUI_EffectBase::Call_AnimaitonClipEnd(_uint iCallbackClipIndex)
{
    Callback_AnimaitonClipEnd(iCallbackClipIndex);
}

void CUI_EffectBase::Call_AnimationEnd()
{
    Callback_AnimaitonEnd();
}


