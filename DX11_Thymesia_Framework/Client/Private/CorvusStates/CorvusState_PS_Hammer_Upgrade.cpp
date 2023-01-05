#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_Hammer_Upgrade.h"
#include "Animation.h"
#include "PhysXController.h"
#include "GameManager.h"

GAMECLASS_C(CCorvusState_PS_Hammer_Upgrade);
CLONE_C(CCorvusState_PS_Hammer_Upgrade, CComponent)

void CCorvusState_PS_Hammer_Upgrade::Call_AnimationEnd()
{
	__super::Call_AnimationEnd();
}

void CCorvusState_PS_Hammer_Upgrade::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 60:
		TurnOn_Effect("Corvus_PW_EyeGlow");
		return;
	case 71:
		GAMEINSTANCE->Set_MotionBlur(0.15f);
		return;
	case 127:
		GAMEINSTANCE->Set_MotionBlur(0.15f);
		return;
	case 178:
		GAMEINSTANCE->Set_MotionBlur(0.15f);
		return;
	case 239:
	{
		_matrix OwnerWorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
		_vector vShakingOffset = XMVectorSet(0.f, -1.f, 0.f, 0.f);
		vShakingOffset = XMVector3TransformNormal(vShakingOffset, OwnerWorldMatrix);
		GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffset, 0.3f, 1.f, 9.f, 0.4f);
		GAMEINSTANCE->Set_MotionBlur(0.3f);
	}
	return;
	case 292:
		TurnOff_Effect("Corvus_PW_EyeGlow");
		return;
	}
}

HRESULT CCorvusState_PS_Hammer_Upgrade::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS_Hammer_Upgrade::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CCorvusState_PS_Hammer_Upgrade::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_PW_Hammer_B_Ver2");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_Hammer_Upgrade::Call_AnimationEnd, this);
}

void CCorvusState_PS_Hammer_Upgrade::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCorvusState_PS_Hammer_Upgrade::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CCorvusState_PS_Hammer_Upgrade::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_PS_Hammer_Upgrade::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit += bind(&CCorvusState_PS_Hammer_Upgrade::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_Hammer_Upgrade::OnStateEnd()
{
	__super::OnStateEnd();

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CCorvusState_PS_Hammer_Upgrade::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit -= bind(&CCorvusState_PS_Hammer_Upgrade::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_Hammer_Upgrade::OnEventMessage(weak_ptr<CBase> pArg)
{
	__super::OnEventMessage(pArg);
}

void CCorvusState_PS_Hammer_Upgrade::Free()
{
	__super::Free();

	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_Hammer_Upgrade::Call_AnimationEnd, this);
}
