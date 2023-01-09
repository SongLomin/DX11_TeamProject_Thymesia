#pragma once
#include "EasingComponent.h"

BEGIN(Client)

class CEasingComponent_Alpha final : public CEasingComponent
{
    GAMECLASS_H(CEasingComponent_Alpha)
    CLONE_H(CEasingComponent_Alpha, CComponent)

public:

    virtual void Start() override;
    virtual void Tick(_float fTimeDelta) override;
    virtual void LateTick(_float fTimeDelta) override;
    // CEasingComponent을(를) 통해 상속됨

public:
    virtual void	Set_Lerp(_float	vStart, _float	vTarget, _float fTime,
        EASING_TYPE eEasingType, PLAY_TYPE ePlayType, _bool bCustomUse);
    virtual void	Set_Lerp_once(_float	vStart, _float	vTarget, _float fTime,
        EASING_TYPE eEasingType, _bool bCustomUse);
    _float         Get_Lerp()
    {
        return     m_vLerped.x;
    };
};


END

