#include "stdafx.h"
#include "EasingComponent_Float.h"
#include "Component.h"
#include "Easing_Utillity.h"
#include "UI.h"
#include "EasingComponent.h"


GAMECLASS_C(CEasingComponent_Float)
CLONE_C(CEasingComponent_Float, CComponent)


void CEasingComponent_Float::Start()
{
    __super::Start();
}

void CEasingComponent_Float::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

}

void CEasingComponent_Float::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

}

float CEasingComponent_Float::Get_RatioFromCurrentToTarget()
{
    return m_vLerped.x / m_vTarget.x;
}

void CEasingComponent_Float::Set_Lerp(_float vStart, _float vTarget, _float fTime, EASING_TYPE eEasingType, PLAY_TYPE ePlayType)
{
    _float4         _vStart;
    _float4         _vTarget;

    _vStart = { vStart, 0.f,0.f,0.f };
    _vTarget = { vTarget,0.f,0.f,0.f };

    CEasingComponent::Set_Lerp(_vStart, _vTarget, fTime, eEasingType, ePlayType, true);
}




