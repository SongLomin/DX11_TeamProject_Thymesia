#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_BigHand_End.h"
#include "CorvusStates/CorvusState_Idle.h"
#include "Animation.h"
#include "PhysXController.h"
#include "GameManager.h"
#include "Weapon.h"

GAMECLASS_C(CCorvusState_PS_BigHand_End);
CLONE_C(CCorvusState_PS_BigHand_End, CComponent)

void CCorvusState_PS_BigHand_End::Call_AnimationEnd(_uint iEndAnimIndex)
{
	__super::Call_AnimationEnd(iEndAnimIndex);
}

void CCorvusState_PS_BigHand_End::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	return;
}

HRESULT CCorvusState_PS_BigHand_End::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS_BigHand_End::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CCorvusState_PS_BigHand_End::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_PW_BigHand_End");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_BigHand_End::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_PS_BigHand_End::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CCorvusState_PS_BigHand_End::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}



void CCorvusState_PS_BigHand_End::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pPhysXControllerCom.lock()->Callback_ControllerHit += bind(&CCorvusState_PS_BigHand_End::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_BigHand_End::OnStateEnd()
{
	__super::OnStateEnd();

	m_pPhysXControllerCom.lock()->Callback_ControllerHit -= bind(&CCorvusState_PS_BigHand_End::Call_OtherControllerHit, this, placeholders::_1);
}

void CCorvusState_PS_BigHand_End::OnEventMessage(weak_ptr<CBase> pArg)
{
	__super::OnEventMessage(pArg);
}

void CCorvusState_PS_BigHand_End::Free()
{
	__super::Free();

	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_BigHand_End::Call_AnimationEnd, this, placeholders::_1);
}
