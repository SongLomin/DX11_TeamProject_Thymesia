#include "stdafx.h"
#include "CorvusStates/CorvusState_Idle.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "PhysXController.h"

GAMECLASS_C(CCorvusState_Idle);
CLONE_C(CCorvusState_Idle, CComponent)

HRESULT CCorvusState_Idle::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CCorvusState_Idle::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_Idle::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_WalkIdle");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Idle::Call_AnimationEnd, this);
}

void CCorvusState_Idle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_Idle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_Idle::Call_AnimationEnd()
{

}

void CCorvusState_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bFirstFoot = true;

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_Climb_L_Down_End>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_Climb_R_Down_End>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_Climb_R_UP_End>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_Climb_L_UP_End>().lock())
	{
		m_bLadderLock = true;
	}
	else
	{
		m_bLadderLock = false;
	}


	if (!m_pModelCom.lock().get())
	{
		m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	}

	Get_OwnerPlayer()->Set_LadderCheck(false);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "CORVUSMonState: Stand -> OnStateStart" << endl;
#endif
#endif

	/*
	1: 바그처형
	2: 도끼
	3. 단검
	4. 바그대검
	5. 누나검
	6. 마법사
	7. 케인(마법사)?몰름


	*/
	//TODO 임시임
	switch (m_iSkillType)
	{
	case 0:
		cout << m_iSkillType << "바그처형" << endl;
		break;
	case 1:
		cout << m_iSkillType << "도끼" << endl;
		break;
	case 2:
		cout << m_iSkillType << "단검" << endl;
		break;
	case 3:
		cout << m_iSkillType << "바그대검" << endl;
		break;
	case 4:
		cout << m_iSkillType << "누나검" << endl;
		break;
	case 5:
		cout << m_iSkillType << "마법사" << endl;
		break;
	case 6:
		cout << m_iSkillType << "케인(마법사)?몰름" << endl;
		break;
	}

}

void CCorvusState_Idle::OnStateEnd()
{
	__super::OnStateEnd();
}

_bool CCorvusState_Idle::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;


	
		PxControllerCollisionFlags Flags = Get_OwnerCharacter().lock()->Get_LastCollisionFlags();

		if (!m_bLadderLock)
		{
			if (!m_bFirstFoot)
			{
				if (!(Flags & PxControllerCollisionFlag::eCOLLISION_DOWN))
				{
					Rotation_InputToLookDir();
					Get_OwnerPlayer()->Change_State<CCorvusState_Fall_Start>();
					return true;
				}
			}
			else
			{
				m_bFirstFoot = false;
			}
			
		}

	


	if (Check_RequirementRunState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_Run>();
		return true;
	}

	if (Check_RequirementJoggingState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_JoggingStart>();
		return true;
	}


	if (Check_RequirementSprintState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_SprintStart>();
		return true;
	}

	if (Check_RequirementAVoidState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_AVoid>();
		return true;
	}

	if (Check_RequirementAttackState())
	{
		weak_ptr<CGameObject> pTargetObject;

		if (Check_RequirementExcuteState(pTargetObject))
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_NorMob_Execution>();
			Get_OwnerPlayer()->Get_CurState().lock()->OnEventMessage(Weak_Cast<CBase>(pTargetObject));
		}
		else
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_LAttack1>();

		}
		return true;
	}

	if (Check_RequirementParryState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_Parry1>();
		return true;
	}

	if (Check_RequirementHealingState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_BasicHealing>();
		return true;
	}

	if (Check_RequirementClawAttackHoldState())
	{
		Rotation_InputToLookDir();
		Get_OwnerPlayer()->Change_State<CCorvusState_ClawAttackHold>();
		return true;
	}



	//임시
	if (KEY_INPUT(KEY::O, KEY_STATE::TAP))
	{
		Rotation_InputToLookDir();
		switch (m_iSkillType)
		{
		case 0:
			Get_OwnerPlayer()->Change_State<CCorvusState_NorMob_Execution>();
			break;
		case 1:
			Get_OwnerPlayer()->Change_State<CCorvusState_PS_Axe>();
			break;
		case 2:
			Get_OwnerPlayer()->Change_State<CCorvusState_PS_Knife>();
			break;
		case 3:
			Get_OwnerPlayer()->Change_State<CCorvusState_PS_VargSword>();
			break;
		case 4:
			Get_OwnerPlayer()->Change_State<CCorvusState_PS_UrdSword>();
			break;
		case 5:
			Get_OwnerPlayer()->Change_State<CCorvusState_PS_Magician>();
			break;
		case 6:
			Get_OwnerPlayer()->Change_State<CCorvusState_PS_CaneSword>();
			break;
		}
		return true;
	}

	//임시
	if (KEY_INPUT(KEY::NUM9, KEY_STATE::TAP))
	{
		m_iSkillType += 1;
	}
	if (KEY_INPUT(KEY::NUM8, KEY_STATE::TAP))
	{
		m_iSkillType -= 1;
	}

	if (m_iSkillType < 0)
		m_iSkillType = 0;

	//임시임위에까지 싹다지울거




	return false;

}

void CCorvusState_Idle::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Idle::Call_AnimationEnd, this);
}


void CCorvusState_Idle::Free()
{
}


