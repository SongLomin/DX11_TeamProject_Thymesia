#include "stdafx.h"
#include "EasingTransform.h"
#include "Component.h"
#include "Easing_Utillity.h"

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

    ZeroMemory(&m_vLerped, sizeof(_float4));
    ZeroMemory(&m_vStart, sizeof(_float4));
    ZeroMemory(&m_vTarget, sizeof(_float4));

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
        m_bLerp = false;
        m_fLerpTick = 0.f;
        Callback_LerpEnd();
        Callback_LerpEnd.Clear();
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
}

void CEasingTransform::Set_LerpFloat(_float In_fStart, _float In_fEnd, _float In_fTime, EASING_TYPE eType)
{
    m_vStart = _float4(In_fStart, In_fStart, In_fStart, In_fStart);
    m_vTarget = _float4(In_fEnd, In_fEnd, In_fEnd, In_fEnd);

    m_vLerped = m_vStart;
    m_fLerpTick = 0.f;
    m_fLerpTime = In_fTime;
    m_eLerpType = eType;
    m_bLerp = true;

}

void CEasingTransform::Set_LerpFloat2(_float2 In_fStart, _float2 In_fEnd, _float In_fTime, EASING_TYPE eType)
{
    m_vStart = _float4(In_fStart.x, In_fStart.y, 0.f, 0.f);
    m_vTarget = _float4(In_fEnd.x, In_fEnd.y, 0.f, 0.f);

    m_vLerped = m_vStart;
    m_fLerpTick = 0.f;
    m_fLerpTime = In_fTime;
    m_eLerpType = eType;
    m_bLerp = true;

}

_float4 CEasingTransform::Get_Lerp()
{
    return m_vLerped;
}
