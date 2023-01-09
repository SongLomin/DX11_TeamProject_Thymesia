#include "stdafx.h"
#include "CorvusStates/StateExecution/Varg_Execution.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"

GAMECLASS_C(CVarg_Execution);
CLONE_C(CVarg_Execution, CComponent)

HRESULT CVarg_Execution::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVarg_Execution::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVarg_Execution::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_VSVarg_Execution");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVarg_Execution::Call_AnimationEnd, this, placeholders::_1);
}

void CVarg_Execution::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CVarg_Execution::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CVarg_Execution::OnDisable()
{

}

void CVarg_Execution::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex , 6);
	
	//Varg_Execution camera LocalOffset.
	_matrix LocalMat = XMMatrixIdentity();
	LocalMat *= XMMatrixRotationX(XMConvertToRadians(-90.f));
	LocalMat *= XMMatrixRotationAxis(LocalMat.r[1], XMConvertToRadians(90.f));

	GET_SINGLE(CGameManager)->Start_Cinematic(m_pModelCom,"camera",LocalMat,CINEMATIC_TYPE::EXECUTION);
#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		
#endif
#endif

}

void CVarg_Execution::OnStateEnd()
{
	__super::OnStateEnd();


	GET_SINGLE(CGameManager)->End_Cinematic();
}

void CVarg_Execution::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CVarg_Execution::Free()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVarg_Execution::Call_AnimationEnd, this, placeholders::_1);
}

_bool CVarg_Execution::Check_AndChangeNextState()
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

