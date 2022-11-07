#include "stdafx.h"
#include "BehaviorBase.h"
#include "LuxiyaStateBase.h"

_bool CBehaviorBase::Change_State(weak_ptr<CStateBase> In_pNextState)
{
    if (m_bEdit)
    {
        //첫 스테이트 교체면 에딧 상태여도 바뀌도록 한다.
        if (!m_pEditState.lock().get())
        {
            Change_State_Internal(In_pNextState);
            m_pEditState = In_pNextState;
        }

        return Change_State_Internal(m_pEditState);
    }

    return Change_State_Internal(In_pNextState);
}

void CBehaviorBase::State_Tick(_float fTimeDelta)
{
    if (!m_pCurrentState.lock().get())
        return;

    if (!m_pCurrentState.lock().get()->Get_Enable())
        DEBUG_ASSERT;

    m_pCurrentState.lock()->Tick(fTimeDelta);
}

void CBehaviorBase::State_LateTick(_float fTimeDelta)
{
    if (!m_pCurrentState.lock().get())
        return;

    if (!m_pCurrentState.lock().get()->Get_Enable())
        DEBUG_ASSERT;

    m_pCurrentState.lock()->LateTick(fTimeDelta);
}

_bool CBehaviorBase::Change_State_Internal(weak_ptr<CStateBase> In_pNextState)
{
    //다음 스테이트가 nullptr임.
    if (!In_pNextState.lock().get())
        DEBUG_ASSERT;

    if (m_pCurrentState.lock().get())
    {
        m_pCurrentState.lock()->OnStateEnd();
        m_pCurrentState.lock()->Set_Enable(false);
    }

    m_pCurrentState = In_pNextState;
    m_pCurrentState.lock()->OnStateStart(const _float& In_fAnimationBlendTime);
    m_pCurrentState.lock()->Set_Enable(true);

    return true;
}

void CBehaviorBase::OnEventMessage(_uint iArg)
{
    if ((_uint)EVENT_TYPE::ON_EDITINIT == iArg)
    {
        m_bEdit = true;
    }
}
