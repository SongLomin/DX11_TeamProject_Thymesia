#pragma once
#include "EasingComponent.h"

BEGIN(Client)

class CEasingComponent_Transform final : public CEasingComponent
{
    GAMECLASS_H(CEasingComponent_Transform)
    CLONE_H(CEasingComponent_Transform, CComponent)

public:
    virtual void Start() override;

	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	// CEasingComponent을(를) 통해 상속됨
 
public:
    virtual void	Set_Lerp(_float2	vStart, _float2	vTarget, _float fTime,
        EASING_TYPE eEasingType, PLAY_TYPE ePlayType, _bool bCustomUse, _bool bLerpUseOffset = true);
    
    _float2         Get_Lerp() 
    {
        _float2    Out_fLerp = {m_vLerped.x, m_vLerped.y };
    
        return     Out_fLerp;
    }; 
private:
    _bool           m_bLerpUseOffset;
};


END

