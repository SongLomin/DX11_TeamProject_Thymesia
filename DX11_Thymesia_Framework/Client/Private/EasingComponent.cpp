#include "stdafx.h"
#include "EasingComponent.h"
#include "Easing_Utillity.h"

GAMECLASS_C(CEasingComponent)


HRESULT CEasingComponent::Initialize_Prototype()
{
    __super::Initialize_Prototype();

    return S_OK;
}



HRESULT CEasingComponent::Initialize(void* pArg)
{
    __super::Initialize(pArg);

    m_fLerpTick = 0.f;
    m_fLerpTime = 0.f;
    m_bLerp = false;
    m_ePlay_Type = ONCE;

    ZeroMemory(&m_vLerped, sizeof(_float4));
    ZeroMemory(&m_vStart, sizeof(_float4));
    ZeroMemory(&m_vTarget, sizeof(_float4));


    return S_OK;
}

void CEasingComponent::Stop()
{
    m_fLerpTick = 0.f;
    m_bLerp = false;
    m_vLerped = m_vStart;

    if (Callback_LerpEnd.size() > 1)
    {
        Callback_LerpEnd.Clear();
    }
    if (Callback_LerpStart.size() > 1)
    {
        Callback_LerpStart.Clear();
    }
}

void CEasingComponent::Set_Lerp(_float4 vStart, _float4 vTarget, _float fTime, EASING_TYPE eEasingType, PLAY_TYPE ePlayType, _bool bCustomUse)
{
    m_vStart = vStart;
    m_vTarget = vTarget;

    m_vLerped = m_vStart;
    m_fLerpTick = 0.f;
    m_fLerpTime = fTime;
    m_eEasingType = eEasingType;
    m_bLerp = true;
  
   m_ePlay_Type = ePlayType;
   m_bCustomUse = bCustomUse;
}


void CEasingComponent::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    if (!m_bLerp)
        return;

    if (m_fLerpTick >= m_fLerpTime)
    {
        {//루프가 다시 돔.
            switch (m_ePlay_Type)
            {
            case Client::CEasingComponent::ONCE:
                m_bLerp = false;
                m_fLerpTick = 0.f;
                Callback_LerpEnd();
                Callback_LerpEnd.Clear();
                return;
                break;
            case Client::CEasingComponent::LOOP_JUST_START:
                Set_Lerp(m_vStart, m_vTarget, m_fLerpTime, m_eEasingType, m_ePlay_Type, m_bCustomUse);
                break;
            case Client::CEasingComponent::LOOP_GO_AND_BACK:
                Set_Lerp(m_vTarget, m_vStart, m_fLerpTime, m_eEasingType, m_ePlay_Type, m_bCustomUse);
                break;
            default:
                break;
            }
        }
    }
   
    m_fLerpTick += fTimeDelta;

    if (m_fLerpTick > m_fLerpTime)
        m_fLerpTick = m_fLerpTime;

    _vector  vLerpedVec = CEasing_Utillity::LerpToType(
        XMLoadFloat4(&m_vStart),
        XMLoadFloat4(&m_vTarget), m_fLerpTick, m_fLerpTime, m_eEasingType);
    XMStoreFloat4(&m_vLerped, vLerpedVec);
}
