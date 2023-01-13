#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_Axe.h"
#include "Animation.h"
#include "PhysXController.h"
#include "GameManager.h"

GAMECLASS_C(CCorvusState_PS_Axe);
CLONE_C(CCorvusState_PS_Axe, CComponent)

void CCorvusState_PS_Axe::Call_AnimationEnd(_uint iEndAnimIndex)
{
	__super::Call_AnimationEnd(iEndAnimIndex);
}

void CCorvusState_PS_Axe::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 38:
		TurnOn_Effect("Corvus_PW_EyeGlow");
		return;
	case 63:
	{
		_matrix OwnerWorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
		_vector vShakingOffset = XMVectorSet(0.f, -1.f, 0.f, 0.f);
		vShakingOffset = XMVector3TransformNormal(vShakingOffset, OwnerWorldMatrix);
		GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffset, 0.3f, 1.f, 9.f, 0.4f);
		GAMEINSTANCE->Set_MotionBlur(0.2f);
	}
	return;
	case 156:
		TurnOff_Effect("Corvus_PW_EyeGlow");
		return;
	}
}

HRESULT CCorvusState_PS_Axe::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS_Axe::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CCorvusState_PS_Axe::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_PW_Axe");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_Axe::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_PS_Axe::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCorvusState_PS_Axe::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CCorvusState_PS_Axe::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_PS_Axe::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit += bind(&CCorvusState_PS_Axe::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_Axe::OnStateEnd()
{
	__super::OnStateEnd();

	if (m_pThisAnimationCom.lock())
		m_pThisAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CCorvusState_PS_Axe::Call_NextKeyFrame, this, placeholders::_1);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit -= bind(&CCorvusState_PS_Axe::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_Axe::OnEventMessage(weak_ptr<CBase> pArg)
{
	__super::OnEventMessage(pArg);
}

void CCorvusState_PS_Axe::Free()
{
	__super::Free();

	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_Axe::Call_AnimationEnd, this, placeholders::_1);
}
