#include "stdafx.h"
#include "EasingComponent_Transform.h"
#include "Component.h"
#include "Easing_Utillity.h"
#include "UI.h"
#include "EasingComponent.h"

GAMECLASS_C(CEasingComponent_Transform)
CLONE_C(CEasingComponent_Transform, CComponent)


void CEasingComponent_Transform::Start()
{
    __super::Start();
}

void CEasingComponent_Transform::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

}

void CEasingComponent_Transform::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    if (m_bLerp == false)
        return;
    if (m_bCustomUse)
        return;
    
    Weak_StaticCast<CUI>(m_pOwner).lock()->Set_OffsetPosition(_float2 (m_vLerped.x, m_vLerped.y));
    
}

void CEasingComponent_Transform::Set_Lerp(_float2 vStart, _float2 vTarget, _float fTime, EASING_TYPE eEasingType, PLAY_TYPE ePlayType, _bool bCustomUse)
{
    _float4         _vStart;
    _float4         _vTarget;

    _vStart = {vStart.x,vStart.y,0.f,0.f };
    _vTarget = { vTarget.x,vTarget.y,0.f,0.f };

    CEasingComponent::Set_Lerp(_vStart, _vTarget, fTime, eEasingType, ePlayType, bCustomUse);

}




