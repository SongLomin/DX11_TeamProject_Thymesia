#include "stdafx.h"
#include "LuxiyaStates/LuxiyaState_Dash.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "LuxiyaStates/LuxiyaStates.h"
#include "Status.h"

GAMECLASS_C(CLuxiyaState_Dash);
CLONE_C(CLuxiyaState_Dash, CComponent)

HRESULT CLuxiyaState_Dash::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	m_fFixedPlayRatio = 0.3f;

	return S_OK;
}

HRESULT CLuxiyaState_Dash::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_iAnimIndex = 26;
	return S_OK;
}

void CLuxiyaState_Dash::Start()
{
	__super::Start();
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CLuxiyaState_Dash::Call_AnimationEnd, this);
}

void CLuxiyaState_Dash::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	_vector vMoveDir = XMVectorSet(0.f, 0.f, 0.f, 0.f);
	vMoveDir = m_pModelCom.lock()->Get_DeltaBonePosition("Bip001");
	m_pTransformCom.lock()->Add_PositionWithRotation(vMoveDir, m_pNaviCom);
}

void CLuxiyaState_Dash::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (Check_AndChangeNextState())
	{
		// 대쉬 끝나며 충돌을 켠다.
		Get_OwnerCharacter().lock()->Set_RigidColliderEnable(true);
	}
}

void CLuxiyaState_Dash::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_pStatusCom.lock()->Add_DashRatio(-0.31f);
	
	// 대쉬 중일때 충돌을 끈다.
	//Get_OwnerCharacter().lock()->Set_RigidColliderEnable(false);

	if (Get_OwnerPlayer()->Is_Warning())
	{
		if (Open_HyperSpace())
		{
			Get_OwnerPlayer()->Set_DodgeTime(13.f);
		}
		else
		{
			Get_OwnerPlayer()->Set_DodgeTime(3.f);
		}
	}

#ifdef _DEBUG
	cout << "LuxiyaState: Dash -> OnStateStart" << endl;

#endif
}

void CLuxiyaState_Dash::OnStateEnd()
{
	__super::OnStateEnd();
}

void CLuxiyaState_Dash::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CLuxiyaState_Stand>();
}

void CLuxiyaState_Dash::OnDestroy()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CLuxiyaState_Dash::Call_AnimationEnd, this);
}

void CLuxiyaState_Dash::Free()
{
	
}

_bool CLuxiyaState_Dash::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (Check_RequirementDashState())
	{
		Rotation_InputToLookDir();
		//Open_HyperSpace();
		Get_OwnerPlayer()->Change_State<CLuxiyaState_Dash>();
		return true;
	}

	if (Check_RequirementUltimateState())
	{
		Rotation_NearToLookDir();
		Get_OwnerPlayer()->Change_State<CLuxiyaState_UltimateSkill>();
		return true;
	}

	if (Check_RequirementAttackState())
	{
		if (!Rotation_InputToLookDir())
			Rotation_NearToLookDir();

		Get_OwnerPlayer()->Change_State<CLuxiyaState_Attack>();

		//3번부터 공격
		Get_OwnerPlayer()->Get_Component<CLuxiyaState_Attack>().lock()->Play_AttackWithIndex(5);


		return true;
	}

	if (Check_RequirementRunState())
	{
		
		
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CLuxiyaState_RunStart>();
		return true;
	}

	return false;
}


