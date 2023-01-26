#include "stdafx.h"
#include "CorvusStates/CorvusState_JoggingStartEnd.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "CorvusStates/CorvusStates.h"

GAMECLASS_C(CCorvusState_JoggingStartEnd);
CLONE_C(CCorvusState_JoggingStartEnd, CComponent)

HRESULT CCorvusState_JoggingStartEnd::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_JoggingStartEnd::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CCorvusState_JoggingStartEnd::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_WalkF_REnd");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_JoggingStartEnd::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_JoggingStartEnd::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	;//m_pBehavior.lock()->RunStartEnd_Tick(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_JoggingStartEnd::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_JoggingStartEnd::Call_AnimationEnd(_uint iEndAnimIndex) //¾Ö´Ï¸ÞÀÌ¼Ç²÷³´À»´ë
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();
}

void CCorvusState_JoggingStartEnd::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);


#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: RunStartEnd -> OnStateStart" << endl;
#endif

#endif
}

void CCorvusState_JoggingStartEnd::OnStateEnd()
{
	__super::OnStateEnd();

}

void CCorvusState_JoggingStartEnd::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_JoggingStartEnd::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_JoggingStartEnd::Free()
{
}

_bool CCorvusState_JoggingStartEnd::Check_AndChangeNextState()
{
	//if (Check_RequirementAttackState())
	//{
	//	if (!Rotation_InputToLookDir())
	//		Rotation_NearToLookDir();
	//
	//	Get_OwnerPlayer()->Change_State<CNorMonState_Attack>();
	//	Get_OwnerPlayer()->Get_Component<CNorMonState_Attack>().lock()->Play_AttackWithIndex(0);
	//	return true;
	//}
	//
	//if (Check_RequirementUltimateState())
	//{
	//	Rotation_NearToLookDir();
	//	Get_OwnerPlayer()->Change_State<CNorMonState_UltimateSkill>();
	//	return true;
	//}
	//
	//if (Check_RequirementDashState())
	//{
	//	Rotation_InputToLookDir();
	//	Get_OwnerPlayer()->Change_State<CNorMonState_Dash>();
	//	return true;
	//}

	if (Check_RequirementJoggingState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_JoggingStart>();
		return true;
	}

	else if (Check_RequirementRunState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_Run>();
		return true;
	}

	else if (Check_RequirementSprintState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_Sprint>();
		return true;
	}

	else if (Check_RequirementAVoidState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_AVoid>();
		return true;
	}

	return false;
}

