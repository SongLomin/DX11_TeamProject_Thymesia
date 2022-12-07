#include "stdafx.h"
#include "EasingTransform.h"
#include "Component.h"
#include "Easing_Utillity.h"
#include "UI.h"


GAMECLASS_C(CEasingTransform)
CLONE_C(CEasingTransform, CComponent)


HRESULT CEasingTransform::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEasingTransform::Initialize(void* pArg)
{
    m_fLerpTick = 0.f;
    m_fLerpTime = 0.f;
    m_bLerp = false;
    m_bLoop = false;


    ZeroMemory(&m_vLerped, sizeof(_float4));
    ZeroMemory(&m_vStart, sizeof(_float4));
    ZeroMemory(&m_vTarget, sizeof(_float4));


    m_eUsingType = USING_ONLY_OFFSET;
	return S_OK;
}

void CEasingTransform::Start()
{
}

void CEasingTransform::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    if (m_fLerpTick >= m_fLerpTime)
    {
        if (!m_bLoop)
        {
            m_bLerp = false;
            m_fLerpTick = 0.f;
            Callback_LerpEnd();
            Callback_LerpEnd.Clear();
        }
        else
        {//루프가 다시 돔.
            switch (m_eLooping_Type)
            {
            case Client::CEasingTransform::JUST_START:
                Set_LerpFloat2_StartFromZero(_float2(m_vTarget.x, m_vTarget.y), m_fLerpTime, m_eLerpType, m_bLoop, m_eLooping_Type);
                break;

            case Client::CEasingTransform::GO_AND_BACK:
                Set_LerpFloat2(_float2(m_vTarget.x, m_vTarget.y),
                    _float2(m_vStart.x, m_vStart.y), m_fLerpTime, m_eLerpType, m_bLoop, m_eLooping_Type);
                break;

            case Client::CEasingTransform::JUST_START_ALPHA:
                Set_Lerp_Alpha(m_fLerpTime, m_eLerpType, m_bLoop, m_eLooping_Type);
                break;
            case Client::CEasingTransform::GO_AND_BACK_ALPHA:
                Set_LerpFloat(m_vTarget.x, m_vStart.x, m_fLerpTime, m_eLerpType, m_bLoop, m_eLooping_Type,
                    CEasingTransform::USING_ONLY_ALPHA);
                break;
            default:
                break;
            }
        }
    }
    if (m_bLerp)
    {
        m_fLerpTick += fTimeDelta;

        if (m_fLerpTick > m_fLerpTime)
            m_fLerpTick = m_fLerpTime;

        _vector  vLerpedVec = CEasing_Utillity::LerpToType(
            XMLoadFloat4(&m_vStart),
            XMLoadFloat4(&m_vTarget), m_fLerpTick, m_fLerpTime, m_eLerpType);
        XMStoreFloat4(&m_vLerped, vLerpedVec);
    }
}

void CEasingTransform::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    if (m_bLerp == false)
        return;

    switch (m_eUsingType)
    {
    case Client::CEasingTransform::USING_ONLY_ALPHA:
        Weak_StaticCast<CUI>(m_pOwner).lock()->Set_AlphaColor(m_vLerped.x);
        break;
    case Client::CEasingTransform::USING_ONLY_OFFSET:
    {
        _float2 vOffset;
        
        vOffset.x = m_vLerped.x;
        vOffset.y = m_vLerped.y;

        Weak_StaticCast<CUI>(m_pOwner).lock()->Set_OffsetPosition(vOffset);
    }
        break;
    default:
        break;
    }
}



void CEasingTransform::Set_Lerp_Alpha(_float In_fTime, EASING_TYPE eType, _bool bLoop, LOOPING_TYPE eLoopingType, EASING_TRANSFORM_USING_TYPE eUsingType)
{
    m_eUsingType = m_eUsingType;
    Set_LerpFloat(0.f, 1.f, In_fTime, eType, bLoop, eLoopingType, m_eUsingType);
}

void CEasingTransform::Set_LerpFloat(_float In_fStart, _float In_fEnd, _float In_fTime, EASING_TYPE eType, _bool bLoop, LOOPING_TYPE eLoopingType, EASING_TRANSFORM_USING_TYPE eUsingType)
{
    m_vStart = _float4(In_fStart, In_fStart, In_fStart, In_fStart);
    m_vTarget = _float4(In_fEnd, In_fEnd, In_fEnd, In_fEnd);

    m_vLerped = m_vStart;
    m_fLerpTick = 0.f;
    m_fLerpTime = In_fTime;
    m_eLerpType = eType;
    m_bLerp = true;
    m_bLoop = bLoop;
    m_eLooping_Type = eLoopingType;
    m_eUsingType = eUsingType;
    // m_eUsingType = USING_ONLY_OFFSET;
}

void CEasingTransform::Set_LerpFloat2(_float2 In_fStart, _float2 In_fEnd, _float In_fTime, EASING_TYPE eType, _bool bLoop
    , LOOPING_TYPE eLoopingType)
{
    m_vStart = _float4(In_fStart.x, In_fStart.y, 0.f, 0.f);
    m_vTarget = _float4(In_fEnd.x, In_fEnd.y, 0.f, 0.f);

    m_vLerped = m_vStart;
    m_fLerpTick = 0.f;
    m_fLerpTime = In_fTime;
    m_eLerpType = eType;
    m_bLerp = true;
    m_bLoop = bLoop;
    m_eLooping_Type = eLoopingType;

    m_eUsingType = USING_ONLY_OFFSET;


}

void CEasingTransform::Set_LerpFloat2_StartFromZero(_float2 In_fEnd, _float In_fTime, EASING_TYPE eType, _bool bLoop, LOOPING_TYPE eLoopingType)
{
    m_vStart = _float4(0, 0, 0.f, 0.f);
    m_vTarget = _float4(In_fEnd.x, In_fEnd.y, 0.f, 0.f);

    m_vLerped = m_vStart;
    m_fLerpTick = 0.f;
    m_fLerpTime = In_fTime;
    m_eLerpType = eType;
    m_bLerp = true;
    m_bLoop = bLoop;
    m_eLooping_Type = eLoopingType;

    m_eUsingType = USING_ONLY_OFFSET;
}


_float4 CEasingTransform::Get_Lerp()
{
    return m_vLerped;
}


