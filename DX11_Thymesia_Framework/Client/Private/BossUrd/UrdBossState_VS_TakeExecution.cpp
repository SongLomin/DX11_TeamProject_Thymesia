#include "stdafx.h"
#include "BossUrd/UrdBossState_VS_TakeExecution.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"
#include "PhysXController.h"
#include "PhysXCharacterController.h"
#include "Status_Boss.h"
#include "Status_Monster.h"

GAMECLASS_C(CUrdBossState_VS_TakeExecution);
CLONE_C(CUrdBossState_VS_TakeExecution, CComponent)

HRESULT CUrdBossState_VS_TakeExecution::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_VS_TakeExecution::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_VS_TakeExecution::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_VS_TakeExecution|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_VS_TakeExecution::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_VS_TakeExecution::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_VS_TakeExecution::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_VS_TakeExecution::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (Check_RequirementIsTargeted())
		GET_SINGLE(CGameManager)->Release_Focus();

	GET_SINGLE(CGameManager)->Disable_Layer(OBJECT_LAYER::PLAYERHUD);
	GET_SINGLE(CGameManager)->Disable_Layer(OBJECT_LAYER::BATTLEUI);


	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex,49);
	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(2.8f, 2.8f, 2.8f));


	
	
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_VS_TakeExecution::OnStateEnd()
{
	__super::OnStateEnd();

	//m_pPhysXControllerCom.lock()->Set_Enable(true);
	m_pModelCom.lock()->Set_AnimationSpeed(1.f);

	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.f, 1.f, 1.f));
}



void CUrdBossState_VS_TakeExecution::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	GET_SINGLE(CGameManager)->Enable_Layer(OBJECT_LAYER::PLAYERHUD);
 
	weak_ptr<CStatus_Boss> pStatus = m_pOwner.lock()->Get_Component<CStatus_Boss>();

	if (pStatus.lock()->Get_Desc().m_iLifeCount == 2)
	{
		m_pOwner.lock()->Get_Component<CStatus_Boss>().lock()->Set_NextPhase();
	}
	
	//Rotation_TargetToLookRevirseDir();

	//m_pTransformCom.lock()->Set_Look2D(-m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK));
	//PxControllerFilters Filters;
	//m_pPhysXControllerCom.lock()->Move(XMVectorSet(-1.4064102f, 0.f, -1.2742195f, 0.f), 0.f, 0.001f, Filters);

	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SpecailAttack(true);
	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.f);
}

void CUrdBossState_VS_TakeExecution::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_VS_TakeExecution::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_VS_TakeExecution::Free()
{

}

_bool CUrdBossState_VS_TakeExecution::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	PxControllerFilters Filters;
	weak_ptr<CStatus_Boss> pStatus = m_pOwner.lock()->Get_Component<CStatus_Boss>();
	_matrix                    vResultOtherWorldMatrix;
	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
	weak_ptr<CCharacter> pOtherCharacter = Weak_StaticCast<CCharacter>(pCurrentPlayer);
	_matrix vOtherWorldMatrix = pCurrentPlayer.lock()->Get_Transform()->Get_WorldMatrix();
	_matrix vMonsterMaxtrix = Get_OwnerMonster()->Get_Transform()->Get_WorldMatrix();

	//124 180 °¡Áö
	
	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 1 &&
		(Get_OwnerMonster()->Get_BossExecutionStartOnOff() ))
	{
		if (Get_OwnerMonster()->Get_BossExecutionStartOnOff())
		{
			
	
			vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vOtherWorldMatrix, XMVectorSet(0.8f, 0.f, -0.9f, 0.f));
			pOtherCharacter.lock()->Get_PhysX().lock()->Set_Position(
				vResultOtherWorldMatrix.r[3],
				GAMEINSTANCE->Get_DeltaTime(),
				Filters);
			vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vResultOtherWorldMatrix, XMVectorSet(-0.8f, 0.f, 0.5f, 0.f));
			Get_OwnerMonster()->Get_Transform()->LookAt2D(vResultOtherWorldMatrix.r[3]);
			vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vMonsterMaxtrix, XMVectorSet(0.8f, 0.f, -0.4f, 0.f));
			pCurrentPlayer.lock()->Get_Transform()->LookAt2D(vResultOtherWorldMatrix.r[3]);
	
	
			Get_OwnerMonster()->Set_BossExecutionStartOnOff(false);
		}
	}


	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 122 &&
		m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() <= 164)
	{
		m_pPhysXControllerCom.lock()->Set_Enable(false);
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 140 &&
		m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() <= 160 &&
		m_bOne)
	{
		m_pModelCom.lock()->Set_AnimationSpeed(0.2f);
		m_bOne = false;
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() > 160  && !m_bOne)
	{
		m_pModelCom.lock()->Set_AnimationSpeed(1.f);
		m_bOne = true;
		m_bAnimaionSpeedControl = true;
	}


	
	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() > 165)
	{
		m_pPhysXControllerCom.lock()->Set_Enable(true);

	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() > 366 && 
		m_bAnimaionSpeedControl)
	{		

		//pCurrentPlayer.lock()->Get_Transform()->LookAt2D(vMonsterMaxtrix.r[3]);
		m_bAnimaionSpeedControl = false;
	}

	if (pStatus.lock()->Get_Desc().m_iLifeCount == 1)
	{
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 217)
		{
			Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Dead>(0.05f);
			return true;
		}
	}



	return false;
}

