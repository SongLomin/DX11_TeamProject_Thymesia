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
#include "UI_ScriptQueue.h"


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

	m_pPhysXControllerCom.lock()->Set_EnableSimulation(false);


	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex,50);
	m_pModelCom.lock()->Set_AnimationSpeed(1.03f);
	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.5f, 1.5f, 1.5f));

	if (Get_OwnerMonster()->Get_BossExecutionStartOnOff())
	{
		PxControllerFilters Filters; 
		
		m_pPhysXControllerCom.lock()->Set_Position(
			XMVectorSet(-42.3155632f, -18.8200264f, 267.281494f, 1.f),
			GAMEINSTANCE->Get_DeltaTime(),
			Filters);
		_matrix       vDoorOpenPlayerMatrix = Get_Owner().lock()->Get_Component<CUrdBossState_Start>().lock()->Get_PlayerTransform();
		Get_OwnerCharacter().lock()->Get_Transform()->Set_Look2D(-vDoorOpenPlayerMatrix.r[2]);

		weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
		weak_ptr<CCharacter> pOtherCharacter = Weak_StaticCast<CCharacter>(pCurrentPlayer);


		_matrix vOtherWorldMatrix = Get_OwnerCharacter().lock()->Get_Transform()->Get_WorldMatrix();
		vOtherWorldMatrix.r[3] = XMVectorSet(-42.5696678f, -18.8200150f, 266.893677f, 1.f);
		_matrix                    vResultOtherWorldMatrix;
		vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vOtherWorldMatrix, XMVectorSet(0.1f, 0.f, 2.3f, 0.f));
		pOtherCharacter.lock()->Get_PhysX().lock()->Set_Position(
			vResultOtherWorldMatrix.r[3],
			GAMEINSTANCE->Get_DeltaTime(),
			Filters);
		pOtherCharacter.lock()->Get_Transform()->Set_Look2D(-vOtherWorldMatrix.r[2]);
		Get_OwnerMonster()->Set_BossExecutionStartOnOff(false);
	}


	
	
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_VS_TakeExecution::OnStateEnd()
{
	__super::OnStateEnd();


	m_pModelCom.lock()->Set_AnimationSpeed(1.f);


	m_pPhysXControllerCom.lock()->Set_EnableSimulation(true);

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
		GAMEINSTANCE->Get_GameObjects<CUI_ScriptQueue>(LEVEL::LEVEL_STATIC).front().lock()->Call_SetScript_Urd_Phase2();
		m_pOwner.lock()->Get_Component<CStatus_Boss>().lock()->Set_NextPhase();
	}


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


	weak_ptr<CStatus_Boss> pStatus = m_pOwner.lock()->Get_Component<CStatus_Boss>();

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

