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
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CVarg_Execution::Call_AnimationEnd, this);
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

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	cout << "LuxiyaState: RunStart -> OnStateStart" << endl;
#endif

	m_iDustEffectIndex = GET_SINGLE(CGameManager)->Use_EffectGroup("Dust", m_pTransformCom);
}

void CVarg_Execution::OnStateEnd()
{
	__super::OnStateEnd();

	GET_SINGLE(CGameManager)->UnUse_EffectGroup("Dust", m_iDustEffectIndex);
}

void CVarg_Execution::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CVarg_Execution::Free()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CVarg_Execution::Call_AnimationEnd, this);
}

_bool CVarg_Execution::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	//if (Check_RequirementUltimateState())
	//{
	//	Rotation_NearToLookDir();
	//	Get_OwnerPlayer()->Change_State<CLuxiyaState_UltimateSkill>();
	//	return true;
	//}
	//
	//if (Check_RequirementAttackState())
	//{
	//	if (!Rotation_InputToLookDir())
	//		Rotation_NearToLookDir();
	//
	//	Get_OwnerPlayer()->Change_State<CLuxiyaState_Attack>();
	//	Get_OwnerPlayer()->Get_Component<CLuxiyaState_Attack>().lock()->Play_AttackWithIndex(0);
	//	return true;
	//}




	return false;
}

