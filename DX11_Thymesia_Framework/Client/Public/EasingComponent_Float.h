#pragma once
#include "EasingComponent.h"

BEGIN(Client)

enum class EASING_FLOAT_CONDITION
{
    LESSER,
    BIGGER
};

typedef struct tagEasingFloatConditionDesc
{
    EASING_FLOAT_CONDITION  _eCondition = EASING_FLOAT_CONDITION::LESSER;
    _float                  _fAmount = 0.f;

    _uint                   _iEventNumber = 0;
    _bool                   _bDelete = false;

}EASINGFLOAT_CONDTIONDESC;


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
    
public:
    void            Add_Condition(EASINGFLOAT_CONDTIONDESC tConditionDesc, _uint EventNumber);
    void            Add_Condition(_float fAmount, EASING_FLOAT_CONDITION eCondition, _uint iEventNumber);

    void            Clear_ConditionEventList() { m_ConditionEventList.clear(); }

    FDelegate<_uint>                Callback_OnConditionEvent;


private:
    list<EASINGFLOAT_CONDTIONDESC>  m_ConditionEventList;

};


END

