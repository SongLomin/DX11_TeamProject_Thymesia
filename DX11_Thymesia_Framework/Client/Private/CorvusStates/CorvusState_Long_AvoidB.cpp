#include "stdafx.h"
#include "CorvusStates/CorvusState_Long_AvoidB.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "Weapon.h"

GAMECLASS_C(CCorvusState_Long_AvoidB);
CLONE_C(CCorvusState_Long_AvoidB, CComponent)

HRESULT CCorvusState_Long_AvoidB::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_Long_AvoidB::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	m_fNextCombatRatio = 0.23f;
	m_fNextNonCombatRatio = 0.6f;
	m_fNextAvoidRatio = 0.46f;
	return S_OK;
}

void CCorvusState_Long_AvoidB::Start()
{
	__super::Start();
	
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_AvoidLongB_NEW");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Long_AvoidB::Call_AnimationEnd, this);
}

void CCorvusState_Long_AvoidB::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_Long_AvoidB::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CCorvusState_Long_AvoidB::OnDisable()
{

}

void CCorvusState_Long_AvoidB::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_Long_AvoidB>().lock())
	{
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex, 5);
	}
	else
	{
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	}
#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif
#endif
}

void CCorvusState_Long_AvoidB::OnStateEnd()
{
	__super::OnStateEnd();


}

void CCorvusState_Long_AvoidB::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_Long_AvoidB::Play_AttackWithIndex(const _tchar& In_iAttackIndex)
{
	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
}

void CCorvusState_Long_AvoidB::Free()
{

}

void CCorvusState_Long_AvoidB::OnDestroy()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Long_AvoidB::Call_AnimationEnd, this);
}

_bool CCorvusState_Long_AvoidB::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 24)
	{
		if (Check_RequirementDashState())
		{
			if (!Rotation_InputToLookDir())
				Rotation_TargetToLookDir();

			m_IsAgainAvoid = false;
			Get_OwnerPlayer()->Change_State<CCorvusState_Long_AvoidB>();
			return false;
		}
	}

	

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 15)
	{
		if (Check_RequirementAttackState())
		{
			Rotation_InputToLookDir();
			_flag TalentEffectFlags = Get_OwnerPlayer()->Check_RequirementForTalentEffects();

			if (TalentEffectFlags & (_flag)TALENT_EFFECT_FLAG::AVOID_SLASH_LV1)
			{
				Get_OwnerPlayer()->Change_State<CCorvusState_AVoidSalsh>();
			}
			else if (TalentEffectFlags & (_flag)TALENT_EFFECT_FLAG::AVOID_THRUST_LV1)
			{
				Get_OwnerPlayer()->Change_State<CCorvusState_AVoidThrust>();
			}
			else
			{
				Get_OwnerPlayer()->Change_State<CCorvusState_LAttack1>();
			}
			
			return true;
		}

		if (Check_RequirementParryState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_Parry2>();
			return true;
		}

		if (Check_RequirementClawAttackHoldState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_ClawAttackHold>();
			return true;
		}
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > m_fNextNonCombatRatio)
	{
		if (Check_RequirementRunState())
		{
			Rotation_InputToLookDir();
			Get_OwnerPlayer()->Change_State<CCorvusState_Run>();
			return true;
		}
	}

	




	return false;
}

_bool CCorvusState_Long_AvoidB::Check_RuquireMnetRepeatAvoidkState()
{
	_uint iTargetKeyFrameMin = 80;
	_uint iTargetKeyFrameMax = 999;




	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(iTargetKeyFrameMin, iTargetKeyFrameMax) && m_IsAgainAvoid)
	{
		return true;
	}



	return false;
}

