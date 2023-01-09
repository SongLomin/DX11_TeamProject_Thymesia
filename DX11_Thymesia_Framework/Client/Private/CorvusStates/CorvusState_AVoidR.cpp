#include "stdafx.h"
#include "CorvusStates/CorvusState_AVoidR.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"

GAMECLASS_C(CCorvusState_AVoidR);
CLONE_C(CCorvusState_AVoidR, CComponent)

HRESULT CCorvusState_AVoidR::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_AVoidR::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	m_fNextCombatRatio = 0.23f;
	m_fNextNonCombatRatio = 0.6f;
	m_fNextAvoidRatio = 0.46f;
	return S_OK;
}

void CCorvusState_AVoidR::Start()
{
	__super::Start();
	
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_AvoidShortMag_R");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_AVoidR::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_AVoidR::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_AVoidR::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_InputAgainAvoid();

	Rotation_TargetToLookDir();

	Check_AndChangeNextState();
}

void CCorvusState_AVoidR::Check_InputAgainAvoid()
{
	if (!KEY_INPUT(KEY::SPACE, KEY_STATE::TAP))
	{
		return;
	}
	m_IsAgainAvoid = true;
}

void CCorvusState_AVoidR::OnDisable()
{

}

void CCorvusState_AVoidR::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_AVoidR>().lock())
	{
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex, 5);
	}
	else
	{
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	}
#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		
#endif
#endif
}

void CCorvusState_AVoidR::OnStateEnd()
{
	__super::OnStateEnd();


}

void CCorvusState_AVoidR::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_AVoidR::Play_AttackWithIndex(const _tchar& In_iAttackIndex)
{
	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
}

void CCorvusState_AVoidR::Free()
{

}

void CCorvusState_AVoidR::OnDestroy()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_AVoidR::Call_AnimationEnd, this, placeholders::_1);
}

_bool CCorvusState_AVoidR::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 24)
	{
		if (KEY_INPUT(KEY::D, KEY_STATE::HOLD))
		{
			if (Check_RequirementDashState())
			{
				m_IsAgainAvoid = false;
				Get_OwnerPlayer()->Change_State<CCorvusState_AVoidR>();
				return false;
			}
		}
		

		else if (!KEY_INPUT(KEY::D, KEY_STATE::HOLD) &&
			KEY_INPUT(KEY::S, KEY_STATE::HOLD))
		{
			if (Check_RequirementDashState())
			{
				Get_OwnerPlayer()->Change_State<CCorvusState_AVoidB>();
				return true;
			}
			
		}

		else if (!KEY_INPUT(KEY::D, KEY_STATE::HOLD) &&
			KEY_INPUT(KEY::W, KEY_STATE::HOLD))
		{
			if (Check_RequirementDashState())
			{
				Get_OwnerPlayer()->Change_State<CCorvusState_AVoidF>();
				return true;
			}
			
		}

		else if (!KEY_INPUT(KEY::D, KEY_STATE::HOLD) &&
			KEY_INPUT(KEY::A, KEY_STATE::HOLD))
		{
			if (Check_RequirementDashState())
			{
				Get_OwnerPlayer()->Change_State<CCorvusState_AVoidL>();
				return true;
			}

		}

		else if (!KEY_INPUT(KEY::D, KEY_STATE::HOLD) &&
			KEY_INPUT(KEY::S, KEY_STATE::HOLD))
		{
			if (Check_RequirementDashState())
			{
				Get_OwnerPlayer()->Change_State<CCorvusState_AVoidB>();
				return true;
			}

		}

		else if (!KEY_INPUT(KEY::D, KEY_STATE::HOLD) &&
			KEY_INPUT(KEY::W, KEY_STATE::HOLD))
		{
			if (Check_RequirementDashState())
			{
				Get_OwnerPlayer()->Change_State<CCorvusState_AVoidF>();
				return true;
			}

		}


	}

	

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 15)
	{
		if (Check_RequirementAttackState())
		{
			
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
			
			Get_OwnerPlayer()->Change_State<CCorvusState_Parry2>();
			return true;
		}

		if (Check_RequirementClawAttackState())
		{
			
			Get_OwnerPlayer()->Change_State<CCorvusState_ClawAttackTab>();
			return true;
		}
	}

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() >= 0.75f)
	{
		if (KEY_INPUT(KEY::W, KEY_STATE::HOLD))
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_Run>();
			return true;
		}
		else if (KEY_INPUT(KEY::A, KEY_STATE::HOLD))
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_RunL>();
			return true;
		}
		else if (KEY_INPUT(KEY::D, KEY_STATE::HOLD))
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_RunR>();
			return true;
		}
		else if (KEY_INPUT(KEY::S, KEY_STATE::HOLD))
		{
			Get_OwnerPlayer()->Change_State<CCorvusState_RunB>();
			return true;
		}

	}


	




	return false;
}

_bool CCorvusState_AVoidR::Check_RuquireMnetRepeatAvoidkState()
{
	_uint iTargetKeyFrameMin = 80;
	_uint iTargetKeyFrameMax = 999;




	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(iTargetKeyFrameMin, iTargetKeyFrameMax) && m_IsAgainAvoid)
	{
		return true;
	}



	return false;
}

