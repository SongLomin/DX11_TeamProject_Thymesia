#pragma once
#include "EasingComponent.h"

BEGIN(Client)

class CEasingComponent_Float final : public CEasingComponent
{
    GAMECLASS_H(CEasingComponent_Float)
        CLONE_H(CEasingComponent_Float, CComponent)

public:
    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    // CEasingComponent을(를) 통해 상속됨

public:
    float           Get_RatioFromCurrentToTarget();
    virtual void	Set_Lerp(_float	vStart, _float	vTarget, _float fTime,
        EASING_TYPE eEasingType, PLAY_TYPE ePlayType);
    _float         Get_Lerp()
    {
        return     m_vLerped.x;
    };
};


END

