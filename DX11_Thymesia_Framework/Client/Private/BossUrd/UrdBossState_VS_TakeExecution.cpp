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

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_VS_TakeExecution::Call_AnimationEnd, this);
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

	//m_pPhysXControllerCom.lock()->Set_Enable(false);

	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.5f, 1.5f, 1.5f));

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex,47);

	_vector vTransformPositon = Get_OwnerMonster()->Get_Transform()->Get_Position();

	cout << "x" << vTransformPositon.m128_f32[0] << "," << "y" << vTransformPositon.m128_f32[1] << "," << "z" << vTransformPositon.m128_f32[2] << endl;

	
	if (Get_OwnerMonster()->Get_BossExecutionStartOnOff())
	{
		PxControllerFilters Filters;
		_matrix                    vResultOtherWorldMatrix;
		weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();
		weak_ptr<CCharacter> pOtherCharacter = Weak_StaticCast<CCharacter>(pCurrentPlayer);
		_matrix vOtherWorldMatrix = pCurrentPlayer.lock()->Get_Transform()->Get_WorldMatrix();
	
		vResultOtherWorldMatrix = SMath::Add_PositionWithRotation(vOtherWorldMatrix, XMVectorSet(1.5f, 0.f, -1.f, 0.f));
			pOtherCharacter.lock()->Get_PhysX().lock()->Set_Position(
				vResultOtherWorldMatrix.r[3],
				GAMEINSTANCE->Get_DeltaTime(),
				Filters);

		Get_OwnerMonster()->Get_Transform()->LookAt2D(vOtherWorldMatrix.r[3]);
		pCurrentPlayer.lock()->Get_Transform()->LookAt2D(Get_OwnerMonster()->Get_WorldPosition());
		

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

	//m_pPhysXControllerCom.lock()->Set_Enable(true);

	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.f, 1.f, 1.f));
}



void CUrdBossState_VS_TakeExecution::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SpecailAttack(true);
	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_VS_TakeExecution::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_VS_TakeExecution::Call_AnimationEnd, this);
}

void CUrdBossState_VS_TakeExecution::Free()
{

}

_bool CUrdBossState_VS_TakeExecution::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	//124 180 ����


	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 60 &&
		m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() <= 180)
	{
		m_pPhysXControllerCom.lock()->Set_Enable(false);
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() > 180)
	{
		m_pPhysXControllerCom.lock()->Set_Enable(true);
	}

	return false;
}

