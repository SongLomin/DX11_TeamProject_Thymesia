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
#include "Weapon.h"
#include "ClientComponent_Utils.h"
#include "PlayerSkill_System.h"
#include "Skill_Base.h"
#include "UIManager.h"

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
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Idle::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_Idle::Tick(_float fTimeDelta)
{
	CPlayerStateBase::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

}

void CCorvusState_Idle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	Check_AndChangeNextState();
}

void CCorvusState_Idle::Call_AnimationEnd(_uint iEndAnimIndex)
{
}

void CCorvusState_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	weak_ptr<CPlayer> pPlayer(Weak_Cast<CPlayer>(m_pOwner));
	list<weak_ptr<CWeapon>>	pWeapons(pPlayer.lock()->Get_Weapon());

	for (auto& elem : pWeapons)
	{
		elem.lock()->Set_RenderOnOff(true);
	}

	m_bFirstFoot = true;

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_Climb_L_Down_End>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_Climb_R_Down_End>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_Climb_R_UP_End>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_Climb_L_UP_End>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_CheckPointEnd>().lock())
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

#ifdef _DEBUG_COUT_
	cout << "CORVUSMonState: Stand -> OnStateStart" << endl;
#endif // _DEBUG
}

void CCorvusState_Idle::OnStateEnd()
{
	__super::OnStateEnd();
}

_bool CCorvusState_Idle::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	PxControllerCollisionFlags Flags(Get_OwnerCharacter().lock()->Get_LastCollisionFlags());


	//다른페이지가 켜져있다면 
	if (GET_SINGLE(CUIManager)->Is_OpenedMenu())
		return false;

	if (m_bLockOn)
	{
		if (KEY_INPUT(KEY::W, KEY_STATE::HOLD))
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_Run>();
			return true;
		}

		else if (Check_RequirementAVoidState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_AVoidB>();
			return true;
		}

		else  if (KEY_INPUT(KEY::S, KEY_STATE::HOLD))
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_RunB>();
			return true;
		}

		else  if (KEY_INPUT(KEY::A, KEY_STATE::HOLD))
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_RunL>();
			return true;
		}

		else  if (KEY_INPUT(KEY::D, KEY_STATE::HOLD))
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_RunR>();
			return true;
		}

		else if (Check_RequirementAttackState())
		{
			weak_ptr<CGameObject> pTargetObject;

			if (Check_RequirementExcuteState(pTargetObject))
			{
				_vector vTargetPos(pTargetObject.lock()->Get_Transform()->Get_Position());
				m_pTransformCom.lock()->LookAt2D(vTargetPos);
				Get_OwnerPlayer()->Change_State<CCorvusState_NorMob_Execution>();
				Get_OwnerPlayer()->Get_CurState().lock()->OnEventMessage(Weak_Cast<CBase>(pTargetObject));
			}
			else
				Get_OwnerPlayer()->Change_State<CCorvusState_LAttack1>();
			return true;
		}

		else if (Check_RequirementParryState())
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_Parry1>();
			return true;
		}

		else if (Check_RequirementClawAttackHoldState())
		{

			Get_OwnerPlayer()->Change_State<CCorvusState_ClawAttackHold>();
			return true;
		}

		else if (Check_RequirementHealingState())
		{

			Get_OwnerPlayer()->Change_State<CCorvusState_BasicHealing>();
			return true;
		}

	}
	else
	{
		if (!m_bLadderLock)
		{
			if (!m_bFirstFoot)
			{
				/*if (!(Flags & PxControllerCollisionFlag::eCOLLISION_DOWN))
				{
					Rotation_InputToLookDir();
					Get_OwnerPlayer()->Change_State<CCorvusState_Fall_Start>();
					return true;
				}*/
			}
			else
				m_bFirstFoot = false;

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
			Get_OwnerPlayer()->Change_State<CCorvusState_Sprint>();
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
				_vector vTargetPos = pTargetObject.lock()->Get_Transform()->Get_Position();
				m_pTransformCom.lock()->LookAt2D(vTargetPos);
				Get_OwnerPlayer()->Change_State<CCorvusState_NorMob_Execution>();
				Get_OwnerPlayer()->Get_CurState().lock()->OnEventMessage(Weak_Cast<CBase>(pTargetObject));
			}
			else
			{
				if (!Rotation_InputToLookDir())
					Rotation_TargetToLookDir();

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

		if (Check_RequirementClawAttackHoldState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_ClawAttackHold>();
			return true;
		}

		if (Check_RequirementHealingState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_BasicHealing>();
			return true;
		}

		if (Check_RequirementFeatherAttackState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_FeatherAttack>();
			return true;
		}
	}

	if (KEY_INPUT(KEY::NUM1, KEY_STATE::TAP))
	{
		Rotation_InputToLookDir();

		switch (m_iSkillType)
		{
		case 0:
			m_pOwner.lock()->Get_Component<CPlayerSkill_System>().lock()->UseMainSKill();
			break;
		case 1:
			Get_OwnerPlayer()->Change_State<CCorvusState_PS_BigHand>();
			break;
		case 2:
			Get_OwnerPlayer()->Change_State<CCorvusState_PS_Axe>();
			break;
		case 3:
			Get_OwnerPlayer()->Change_State<CCorvusState_PS_CaneSword_Start>();
			break;
		case 4:
			Get_OwnerPlayer()->Change_State<CCorvusState_PS_Halberds>();
			break;
		case 5:
			Get_OwnerPlayer()->Change_State<CCorvusState_PS_BatRoar>();
			break;
		case 6:
			Get_OwnerPlayer()->Change_State<CCorvusState_AromorLV1_NCamera_Execution>();
			break;
		case 7:
			Get_OwnerPlayer()->Change_State<CCorvusState_Urd_Execution>();
			break;
		}
		return true;
	}

	if (KEY_INPUT(KEY::NUM2, KEY_STATE::TAP))
	{
		Rotation_InputToLookDir();
		m_pOwner.lock()->Get_Component<CPlayerSkill_System>().lock()->UseStealSKill();
		return true;
	}


	// TODO : for testing skills
	if (KEY_INPUT(KEY::NUM9, KEY_STATE::TAP))
	{
		if (9 == m_iSkillType)
			m_iSkillType = 0;
		else
			m_iSkillType++;
	}

	// TODO : for testing skills
	if (KEY_INPUT(KEY::NUM8, KEY_STATE::TAP))
	{
		if (0 == m_iSkillType)
			m_iSkillType = 9;
		else
			m_iSkillType--;
	}

	return false;
}

void CCorvusState_Idle::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Idle::Call_AnimationEnd, this, placeholders::_1);
}


void CCorvusState_Idle::Free()
{
}
