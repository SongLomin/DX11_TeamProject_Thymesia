#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_BatRoar_Upgrade.h"
#include "Animation.h"
#include "PhysXController.h"
#include "GameManager.h"
#include "CorvusStates/CorvusStates.h"

GAMECLASS_C(CCorvusState_PS_BatRoar_Upgrade);
CLONE_C(CCorvusState_PS_BatRoar_Upgrade, CComponent)

void CCorvusState_PS_BatRoar_Upgrade::Call_AnimationEnd(_uint iEndAnimIndex)
{
	__super::Call_AnimationEnd(iEndAnimIndex);
}

void CCorvusState_PS_BatRoar_Upgrade::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 43:
		TurnOn_Effect("Corvus_PW_EyeGlow_Red");
	return;
	case 53:
	{
		GET_SINGLE(CGameManager)->Activate_Zoom(-2.f, 0.1f, EASING_TYPE::CUBIC_OUT);

		_matrix OwnerWorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
		_vector vShakingOffset = XMVectorSet(0.f, -1.f, 0.f, 0.f);
		vShakingOffset = XMVector3TransformNormal(vShakingOffset, OwnerWorldMatrix);
		GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffset, 0.3f, 1.5f, 6.f, 0.8f);

		_float3 vPlayerPos;
		ZeroMemory(&vPlayerPos, sizeof(_float3));
		XMStoreFloat3(&vPlayerPos, GET_SINGLE(CGameManager)->Get_PlayerPos());
		vPlayerPos.y += 1.f;
		GAMEINSTANCE->Set_RadialBlur(0.3f, vPlayerPos);
	}
	return;
	case 121:
		GET_SINGLE(CGameManager)->Deactivate_Zoom(1.5f, EASING_TYPE::QUAD_OUT);
		return;
	case 130:
		TurnOff_Effect("Corvus_PW_EyeGlow_Red");
		return;
	}
}

HRESULT CCorvusState_PS_BatRoar_Upgrade::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS_BatRoar_Upgrade::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CCorvusState_PS_BatRoar_Upgrade::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_PW_Bat_Roar_B");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_BatRoar_Upgrade::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_PS_BatRoar_Upgrade::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCorvusState_PS_BatRoar_Upgrade::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	Check_AndChangeNextState();

}

_bool CCorvusState_PS_BatRoar_Upgrade::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 137)
	{
		if (Check_RequirementRunState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_Run>();
			return true;
		}
	}

	return false;
}



void CCorvusState_PS_BatRoar_Upgrade::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_PS_BatRoar_Upgrade::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit += bind(&CCorvusState_PS_BatRoar_Upgrade::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_BatRoar_Upgrade::OnStateEnd()
{
	__super::OnStateEnd();

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CCorvusState_PS_BatRoar_Upgrade::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit -= bind(&CCorvusState_PS_BatRoar_Upgrade::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_BatRoar_Upgrade::OnEventMessage(weak_ptr<CBase> pArg)
{
	__super::OnEventMessage(pArg);
}

void CCorvusState_PS_BatRoar_Upgrade::Free()
{
	__super::Free();

	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_BatRoar_Upgrade::Call_AnimationEnd, this, placeholders::_1);
}
