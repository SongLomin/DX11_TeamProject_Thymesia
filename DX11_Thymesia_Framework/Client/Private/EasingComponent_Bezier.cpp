#include "stdafx.h"
#include "EasingComponent_Bezier.h"


GAMECLASS_C(CEasingComponent_Bezier)
CLONE_C(CEasingComponent_Bezier, CComponent)

void CEasingComponent_Bezier::Start()
{
	__super::Start();

}

void CEasingComponent_Bezier::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CEasingComponent_Bezier::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CEasingComponent_Bezier::Set_Lerp_OneCurve(_float2 vStart, _float2 vCurve, _float2 vTarget, _float fTime, EASING_TYPE eEasingType, PLAY_TYPE ePlayType, _bool bCustomUse)
{
    Set_Lerp_TwoCurve(vStart, vCurve, vCurve, vTarget, fTime, eEasingType, ePlayType, bCustomUse);
}

void CEasingComponent_Bezier::Set_Lerp_TwoCurve(_float2 vStart, _float2 vCurve1, _float2 vCurve2, _float2 vTarget, _float fTime, EASING_TYPE eEasingType, PLAY_TYPE ePlayType, _bool bCustomUse)
{
    _float4         _vStart;
    _float4         _vTarget;

    m_vCurve1 = vCurve1;
    m_vCurve2 = vCurve2;

    _vStart = { vStart.x,vStart.y,0.f,0.f };
    _vTarget = { vTarget.x,vTarget.y,0.f,0.f };
    CEasingComponent::Set_Lerp(_vStart, _vTarget, fTime, eEasingType, ePlayType, bCustomUse);
}

_float2 CEasingComponent_Bezier::Get_Lerp()
{
    _float  fRatio = Get_PlayingRatio();//진행도 0~1 비율로 가져오기.

    float t = fRatio;
    float t2 = t * t;
    float one_minus_t = 1.0 - t;
    float one_minus_t2 = one_minus_t * one_minus_t;

    _vector vStart = XMLoadFloat4(&m_vStart);
    _vector vTarget = XMLoadFloat4(&m_vTarget);
    _vector vCurve1 = XMVectorSet(m_vCurve1.x, m_vCurve1.y, 0.f, 1.f);
    _vector vCurve2 = XMVectorSet(m_vCurve2.x, m_vCurve2.y, 0.f, 1.f);

    _vector  vBezier =  (vStart* one_minus_t2 * one_minus_t + vCurve1 * 3.0 * t * one_minus_t2 + 
        vCurve2 * 3.0 * t2 * one_minus_t + vTarget * t2 * t);

    _float2  fBezier = { vBezier.m128_f32[0], vBezier.m128_f32[1] };

    return fBezier;
}
