#include "stdafx.h"
#include "CorvusStates/StateExecution/NorMob_Execution.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "Monster.h"
#include "NorMonStates.h"

GAMECLASS_C(CNorMob_Execution);
CLONE_C(CNorMob_Execution, CComponent)

HRESULT CNorMob_Execution::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMob_Execution::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMob_Execution::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Corvus.ao|Corvus_StunExecute_StartL_L");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMob_Execution::Call_AnimationEnd, this);
}

void CNorMob_Execution::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMob_Execution::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CNorMob_Execution::OnDisable()
{

}

void CNorMob_Execution::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);


//	GET_SINGLE(CGameManager)->Start_Cinematic(m_pModelCom, "camera",XMMatrixIdentity());
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif
#endif

}

void CNorMob_Execution::OnStateEnd()
{
	__super::OnStateEnd();
	GET_SINGLE(CGameManager)->End_Cinematic();
}

void CNorMob_Execution::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}



void CNorMob_Execution::OnEventMessage(weak_ptr<CBase> pArg)
{
	m_pTargetObject = Weak_Cast<CGameObject>(pArg);
	Weak_Cast<CMonster>(m_pTargetObject).lock()->Change_State<CNorMonState_Die>();
}

void CNorMob_Execution::Free()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMob_Execution::Call_AnimationEnd, this);
}

_bool CNorMob_Execution::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	//if (Check_RequirementUltimateState())
	//{
	//	Rotation_NearToLookDir();
	//	Get_OwnerPlayer()->Change_State<CNorMonState_UltimateSkill>();
	//	return true;
	//}
	//
	//if (Check_RequirementAttackState())
	//{
	//	if (!Rotation_InputToLookDir())
	//		Rotation_NearToLookDir();
	//
	//	Get_OwnerPlayer()->Change_State<CNorMonState_Attack>();
	//	Get_OwnerPlayer()->Get_Component<CNorMonState_Attack>().lock()->Play_AttackWithIndex(0);
	//	return true;
	//}




	return false;
}

