#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_Knife.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "Animation.h"
#include "PhysXController.h"
#include "GameManager.h"

GAMECLASS_C(CCorvusState_PS_Knife);
CLONE_C(CCorvusState_PS_Knife, CComponent)

void CCorvusState_PS_Knife::Call_AnimationEnd(_uint iEndAnimIndex)
{
	__super::Call_AnimationEnd(iEndAnimIndex);
}

void CCorvusState_PS_Knife::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 63:
		TurnOn_Effect("Corvus_PW_EyeGlow");
		return;
	case 50:
		_float3 vPosition;
		ZeroMemory(&vPosition, sizeof(_float3));
		XMStoreFloat3(&vPosition, m_pOwner.lock()->Get_Transform()->Get_Position());
		GAMEINSTANCE->Set_RadialBlur(0.3f, vPosition);
		return;
	case 78:
		TurnOff_Effect("Corvus_PW_EyeGlow");
		return;
	}
}

HRESULT CCorvusState_PS_Knife::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS_Knife::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CCorvusState_PS_Knife::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_PW_Knife_A_V1");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_Knife::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_PS_Knife::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCorvusState_PS_Knife::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

_bool CCorvusState_PS_Knife::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 100)
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

void CCorvusState_PS_Knife::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);
}

void CCorvusState_PS_Knife::OnStateEnd()
{
	__super::OnStateEnd();
}

void CCorvusState_PS_Knife::OnEventMessage(weak_ptr<CBase> pArg)
{
	__super::OnEventMessage(pArg);
}

void CCorvusState_PS_Knife::Free()
{
	__super::Free();

	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_Knife::Call_AnimationEnd, this, placeholders::_1);
}
