#include "stdafx.h"
#include "NorMonState/NorMonState_SpecialAttackLoop.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"
#include "MobWeapon.h"
#include "PhysXController.h"


GAMECLASS_C(CNorMonState_SpecialAttackLoop);
CLONE_C(CNorMonState_SpecialAttackLoop, CComponent)

HRESULT CNorMonState_SpecialAttackLoop::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_SpecialAttackLoop::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_SpecialAttackLoop::Start()
{
	__super::Start();

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::SKULLSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|LArmor_Shield_Run_F");
		break;
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|LArmor_Shield_Run_F");
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|LArmor_Shield_Run_F");
		break;
	}


	//m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_SpecialAttackLoop::Call_AnimationEnd, this, placeholders::_1);


}

void CNorMonState_SpecialAttackLoop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Turn_ToThePlayer(fTimeDelta);


	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	PxControllerFilters Filters;
	m_pPhysXControllerCom.lock()->MoveWithRotation({ 0.f, 0.f, m_fCurrentSpeed * fTimeDelta }, 0.f, fTimeDelta, Filters, nullptr, m_pTransformCom);
}

void CNorMonState_SpecialAttackLoop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();

}




void CNorMonState_SpecialAttackLoop::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_

#endif
#endif


}

void CNorMonState_SpecialAttackLoop::OnStateEnd()
{
	__super::OnStateEnd();


	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}


//void CNorMonState_SpecialAttackLoop::OnDestroy()
//{
//	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_SpecialAttackLoop::Call_AnimationEnd, this, placeholders::_1);
//}

void CNorMonState_SpecialAttackLoop::Free()
{

}


//void CNorMonState_SpecialAttackLoop::Call_AnimationEnd(_uint iEndAnimIndex)
//{
//	if (!Get_Enable())
//		return;
//
//
//	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
//}


_bool CNorMonState_SpecialAttackLoop::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	_float fPToMDistance = Get_DistanceWithPlayer(); // 플레이어와 몬스터 거리

	if (fPToMDistance <= 1.5f)
	{
		Get_OwnerCharacter().lock()->Change_State<CNorMonState_SpecialAttackEnd>(0.05f);
		return true;
	}

	//플레이어랑 거리가까워지면 스페셜 어택 ->앤드로가게끔

	return false;

}



