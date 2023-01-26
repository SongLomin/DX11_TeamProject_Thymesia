#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_BatRoar.h"
#include "Animation.h"
#include "PhysXController.h"
#include "GameManager.h"
#include "Status_Player.h"
#include "CorvusStates/CorvusStates.h"

GAMECLASS_C(CCorvusState_PS_BatRoar);
CLONE_C(CCorvusState_PS_BatRoar, CComponent)

void CCorvusState_PS_BatRoar::Call_AnimationEnd(_uint iEndAnimIndex)
{
	__super::Call_AnimationEnd(iEndAnimIndex);
}

void CCorvusState_PS_BatRoar::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 18:
		GET_SINGLE(CGameManager)->Activate_Zoom(1.5f, 1.f, EASING_TYPE::QUINT_IN);
		return;
	case 60:
		TurnOn_Effect("Corvus_PW_EyeGlow_Red");
		return;
	case 71:
	{
		GET_SINGLE(CGameManager)->Deactivate_Zoom(0.1f, EASING_TYPE::LINEAR);

		_matrix OwnerWorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
		_vector vShakingOffset = XMVectorSet(0.f, -1.f, 0.f, 0.f);
		vShakingOffset = XMVector3TransformNormal(vShakingOffset, OwnerWorldMatrix);
		GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffset, 0.3f, 1.f, 9.f, 0.4f);

		_float3 vPlayerPos;
		ZeroMemory(&vPlayerPos, sizeof(_float3));
		XMStoreFloat3(&vPlayerPos, GET_SINGLE(CGameManager)->Get_PlayerPos());
		vPlayerPos.y += 1.f;
		GAMEINSTANCE->Set_RadialBlur(0.2f, vPlayerPos);

		//스킬 효과
		GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Get_Component<CStatus_Player>().lock()->Heal_PlayerFromMaxHP(0.3f);
	}
		return;
	case 108:
		TurnOff_Effect("Corvus_PW_EyeGlow_Red");
		return;
	}
}

HRESULT CCorvusState_PS_BatRoar::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS_BatRoar::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CCorvusState_PS_BatRoar::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_PW_Bat_Roar_A");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_BatRoar::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_PS_BatRoar::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCorvusState_PS_BatRoar::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

_bool CCorvusState_PS_BatRoar::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 113)
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


void CCorvusState_PS_BatRoar::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_PS_BatRoar::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit += bind(&CCorvusState_PS_BatRoar::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_BatRoar::OnStateEnd()
{
	__super::OnStateEnd();

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CCorvusState_PS_BatRoar::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit -= bind(&CCorvusState_PS_BatRoar::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_BatRoar::OnEventMessage(weak_ptr<CBase> pArg)
{
	__super::OnEventMessage(pArg);
}

void CCorvusState_PS_BatRoar::Free()
{
	__super::Free();

	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_BatRoar::Call_AnimationEnd, this, placeholders::_1);
}
