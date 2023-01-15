#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_Scythe.h"
#include "Animation.h"
#include "PhysXController.h"
#include "GameManager.h"
#include "CorvusStates/CorvusStates.h"

GAMECLASS_C(CCorvusState_PS_Scythe);
CLONE_C(CCorvusState_PS_Scythe, CComponent)

void CCorvusState_PS_Scythe::Call_AnimationEnd(_uint iEndAnimIndex)
{
	__super::Call_AnimationEnd(iEndAnimIndex);
}

void CCorvusState_PS_Scythe::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 60:
		TurnOn_Effect("Corvus_PW_EyeGlow");
		return;
	case 74:
		GAMEINSTANCE->Set_MotionBlur(0.3f);
		return;
	case 145:
		TurnOff_Effect("Corvus_PW_EyeGlow");
		return;
	}
}

HRESULT CCorvusState_PS_Scythe::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS_Scythe::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CCorvusState_PS_Scythe::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_PW_Scythe");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_Scythe::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_PS_Scythe::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCorvusState_PS_Scythe::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

_bool CCorvusState_PS_Scythe::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 176)
	{
		if (Check_RequirementRunState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_Run>();
			return true;
		}
	}
	else
	{
		Rotation_TargetToLookDir();
	}

	return false;
}



void CCorvusState_PS_Scythe::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_PS_Scythe::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit += bind(&CCorvusState_PS_Scythe::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_Scythe::OnStateEnd()
{
	__super::OnStateEnd();

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CCorvusState_PS_Scythe::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit -= bind(&CCorvusState_PS_Scythe::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_Scythe::OnEventMessage(weak_ptr<CBase> pArg)
{
	__super::OnEventMessage(pArg);
}

void CCorvusState_PS_Scythe::Free()
{
	__super::Free();

	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_Scythe::Call_AnimationEnd, this, placeholders::_1);
}
