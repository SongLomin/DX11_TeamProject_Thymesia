#pragma once
#include "EasingComponent.h"

BEGIN(Client)

class CEasingComponent_Bezier :    public CEasingComponent
{
public:
	GAMECLASS_H(CEasingComponent_Bezier)
	CLONE_H(CEasingComponent_Bezier, CComponent)

public:
    virtual void Start() override;

    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    // CEasingComponent을(를) 통해 상속됨

public:
    virtual void	Set_Lerp_OneCurve(_float2 vStart, _float2 vCurve, _float2 vTarget, _float fTime,
        EASING_TYPE eEasingType, PLAY_TYPE ePlayType, _bool bCustomUse);

    virtual void	Set_Lerp_TwoCurve(_float2 vStart, _float2 vCurve1, _float2 vCurve2, _float2 vTarget, _float fTime,
        EASING_TYPE eEasingType, PLAY_TYPE ePlayType, _bool bCustomUse);

    _float2         Get_Lerp();

private:
    _float2          m_vCurve1;
    _float2          m_vCurve2;

};

END
