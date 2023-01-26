#include "stdafx.h"
#include "VargBossStateBase.h"
#include "EliteMonState/Joker/JokerState_Execution_Start.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"
#include "MonsterHPBar_Base.h"
#include "Status_Monster.h"
#include "PhysXCharacterController.h"
#include "PhysXController.h"

GAMECLASS_C(CJokerState_TakeExecution_Start);
CLONE_C(CJokerState_TakeExecution_Start, CComponent)

HRESULT CJokerState_TakeExecution_Start::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_TakeExecution_Start::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CJokerState_TakeExecution_Start::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_TakeExecution_Start");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_TakeExecution_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_TakeExecution_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CJokerState_TakeExecution_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CJokerState_TakeExecution_Start::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	_bool  m_bf = Get_OwnerMonster()->Get_EliteExecutionStartOnOff();

	//m_pPhysXControllerCom.lock()->Set_EnableSimulation(false);

	if (Get_OwnerMonster()->Get_EliteExecutionStartOnOff())
	{
		PxControllerFilters Filters;
		_matrix                    vDoorOpenPlayerMatrix = Get_Owner().lock()->Get_Component<CJokerState_Sp_Open>().lock()->Get_PlayerTransform();
		m_pPhysXControllerCom.lock()->Set_Position(
			XMVectorSet(121.2f, 49.58f, -48.90f, 1.f),
			GAMEINSTANCE->Get_DeltaTime(),
			Filters);
		Get_OwnerCharacter().lock()->Get_Transform()->Set_Look2D(-vDoorOpenPlayerMatrix.r[2]);

		weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
		weak_ptr<CCharacter> pOtherCharacter = Weak_StaticCast<CCharacter>(pCurrentPlayer);


		_matrix vOtherWorldMatrix = Get_OwnerCharacter().lock()->Get_Transform()->Get_WorldMatrix();
		vOtherWorldMatrix.r[3] = XMVectorSet(121.2f, 49.58f, -48.90f, 1.f);
		_matrix                    vResultOtherWorldMatrix;
		vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vOtherWorldMatrix, XMVectorSet(0.f, 0.f, 1.f, 0.f));
		pOtherCharacter.lock()->Get_PhysX().lock()->Set_Position(
			vResultOtherWorldMatrix.r[3],
			GAMEINSTANCE->Get_DeltaTime(),
			Filters);
		pOtherCharacter.lock()->Get_Transform()->Set_Look2D(-vOtherWorldMatrix.r[2]);

		Get_OwnerMonster()->Set_EliteExecutionStartOnOff(false);
	}

	m_bOnce = true;

	m_bTwoOnce = true;
	
	m_pOwner.lock()->Get_Component<CStatus_Monster>().lock()->CallBack_UI_Disable();

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif
#endif

}

void CJokerState_TakeExecution_Start::OnStateEnd()
{
	__super::OnStateEnd();

	//m_pPhysXControllerCom.lock()->Set_EnableSimulation(true);


}

void CJokerState_TakeExecution_Start::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CJokerState_Execution_Loop>(0.05f);
}

void CJokerState_TakeExecution_Start::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CJokerState_TakeExecution_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_TakeExecution_Start::Free()
{

}

_bool CJokerState_TakeExecution_Start::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;
	
	//
	//PxControllerFilters Filters;
	//weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
	//weak_ptr<CCharacter> pOtherCharacter = Weak_StaticCast<CCharacter>(pCurrentPlayer);
	//_matrix vOtherWorldMatrix = Get_OwnerCharacter().lock()->Get_Transform()->Get_WorldMatrix();
	//
 	//if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 1 && m_bOnce)
	//{
	//			m_bOnce = false;
	//		PxControllerFilters Filters;
	//		weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
	//		weak_ptr<CCharacter> pOtherCharacter = Weak_StaticCast<CCharacter>(pCurrentPlayer);
	//		_matrix                    vDoorOpenPlayerMatrix = Get_Owner().lock()->Get_Component<CJokerState_Sp_Open>().lock()->Get_PlayerTransform();
	//		m_pPhysXControllerCom.lock()->Set_Position(
	//			XMVectorSet(121.273582f, 49.5800247f, -48.9076538f, 1.f),
	//			GAMEINSTANCE->Get_DeltaTime(),
	//			Filters);
	//		Get_OwnerCharacter().lock()->Get_Transform()->LookAt2D(-vDoorOpenPlayerMatrix.r[2]);
	//
	//		
	//	
	//		Get_OwnerMonster()->Set_BossExecutionStartOnOff(false);
	//	
	//	
	//}
	//
	//if ((m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 2  && !m_bOnce && m_bTwoOnce))
	//{
	//	_matrix vOtherWorldMatrix = Get_OwnerCharacter().lock()->Get_Transform()->Get_WorldMatrix();
	//	m_bTwoOnce = false;
	//
	//	_matrix                    vResultOtherWorldMatrix;
	//	vResultOtherWorldMatrix = Get_OwnerMonster()->Get_Transform()->Get_WorldMatrix();
	//	pOtherCharacter.lock()->Get_PhysXController().lock()->Set_Position(XMVectorSet(123.273582f, 49.5800247f, -48.6076538f, 1.f),
	//		GAMEINSTANCE->Get_DeltaTime(),
	//		Filters);
	//	
	//}
	//
	//if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() >= 0.7f)
	//{
	//	
	//	PxControllerFilters Filters;
	//	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
	//	weak_ptr<CCharacter> pOtherCharacter = Weak_StaticCast<CCharacter>(pCurrentPlayer);
	//
	//	_matrix vOtherWorldMatrix = Get_OwnerCharacter().lock()->Get_Transform()->Get_WorldMatrix();
	//
	//	pOtherCharacter.lock()->Get_Transform()->LookAt2D(-vOtherWorldMatrix.r[2]);
	//	
	//}


	

	return false;
}

void CJokerState_TakeExecution_Start::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CEliteStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
}

