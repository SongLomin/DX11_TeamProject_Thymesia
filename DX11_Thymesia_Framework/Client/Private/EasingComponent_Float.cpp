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

    if (Is_Lerping())
    {
        _float fAmount = Get_Lerp();

        for (auto& elem : m_ConditionEventList)
        {
            if (elem._bDelete)//만에 하나 지워질놈들이 한번더 호출되면 나가기
            {
                continue;
            }
            if (elem._eCondition == EASING_FLOAT_CONDITION::BIGGER)
            {
                if (fAmount > elem._fAmount)
                {
                    Callback_OnConditionEvent(elem._iEventNumber);
                    elem._bDelete = true;
                }
            }
            else if (elem._eCondition == EASING_FLOAT_CONDITION::LESSER)
            {
                if (fAmount < elem._fAmount)
                {
                    Callback_OnConditionEvent(elem._iEventNumber);
                    elem._bDelete = true;
                }
            }
        }
    }
}

void CEasingComponent_Float::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);

    for (auto iter = m_ConditionEventList.begin(); iter != m_ConditionEventList.end();)
    {
        if (iter->_bDelete)
        {
            iter = m_ConditionEventList.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
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

void CEasingComponent_Float::Add_Condition(EASINGFLOAT_CONDTIONDESC tConditionDesc, _uint EventNumber)
{
    tConditionDesc._iEventNumber = EventNumber;

    m_ConditionEventList.push_back(tConditionDesc);

}

void CEasingComponent_Float::Add_Condition(_float fAmount, EASING_FLOAT_CONDITION eCondition, _uint iEventNumber)
{
    EASINGFLOAT_CONDTIONDESC tConditonDesc;

    tConditonDesc._eCondition = eCondition;
    tConditonDesc._fAmount = fAmount;
    tConditonDesc._iEventNumber = iEventNumber;
    tConditonDesc._bDelete = false;

    m_ConditionEventList.push_back(tConditonDesc);

}



