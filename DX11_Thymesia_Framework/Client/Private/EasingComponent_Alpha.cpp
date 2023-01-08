#include "stdafx.h"
#include "EasingComponent_Alpha.h"
#include "Component.h"
#include "Easing_Utillity.h"
#include "UI.h"
#include "EasingComponent.h"


GAMECLASS_C(CEasingComponent_Alpha)
CLONE_C(CEasingComponent_Alpha, CComponent)


void CEasingComponent_Alpha::Start()
{
    __super::Start();
}

void CEasingComponent_Alpha::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

}

void CEasingComponent_Alpha::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    if (m_bLerp == false)
        return;
    if (m_bCustomUse)
        return;

    Weak_StaticCast<CUI>(m_pOwner).lock()->Set_AlphaColor(m_vLerped.x);

}

void CEasingComponent_Alpha::Set_Lerp(_float vStart, _float vTarget, _float fTime, EASING_TYPE eEasingType, PLAY_TYPE ePlayType, _bool bCustomUse)
{
    _float4         _vStart;
    _float4         _vTarget;

    _vStart = { vStart, 0.f,0.f,0.f };
    _vTarget = { vTarget,0.f,0.f,0.f };

    CEasingComponent::Set_Lerp(_vStart, _vTarget, fTime, eEasingType, ePlayType, bCustomUse);

}

void CEasingComponent_Alpha::Set_Lerp_once(_float vStart, _float vTarget, _float fTime, EASING_TYPE eEasingType, _bool bCustomUse)
{
    Set_Lerp(vStart, vTarget, fTime, eEasingType, CEasingComponent::ONCE, bCustomUse);
}




