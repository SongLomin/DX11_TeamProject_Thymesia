#include "stdafx.h"
#include "CorvusStates/CorvusState_AVoid.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "Weapon.h"

GAMECLASS_C(CCorvusState_AVoid);
CLONE_C(CCorvusState_AVoid, CComponent)

HRESULT CCorvusState_AVoid::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_AVoid::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	m_fNextCombatRatio = 0.23f;
	m_fNextNonCombatRatio = 0.6f;
	m_fNextAvoidRatio = 0.46f;
	return S_OK;
}

void CCorvusState_AVoid::Start()
{
	__super::Start();
	
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_AvoidF");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_AVoid::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_AVoid::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_AVoid::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_InputAgainAvoid();

	Check_AndChangeNextState();
}

void CCorvusState_AVoid::Check_InputAgainAvoid()
{
	if (!KEY_INPUT(KEY::SPACE, KEY_STATE::TAP))
	{
		return;
	}
	m_IsAgainAvoid = true;
}

void CCorvusState_AVoid::OnDisable()
{

}

void CCorvusState_AVoid::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	weak_ptr<CPlayer> pPlayer = Weak_Cast<CPlayer>(m_pOwner);
	list<weak_ptr<CWeapon>>	pWeapons = pPlayer.lock()->Get_Weapon();

	pWeapons.front().lock()->Set_RenderOnOff(true);

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_AVoid>().lock())
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

void CCorvusState_AVoid::OnStateEnd()
{
	__super::OnStateEnd();


}

void CCorvusState_AVoid::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_AVoid::Play_AttackWithIndex(const _tchar& In_iAttackIndex)
{
	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
}

void CCorvusState_AVoid::Free()
{

}

void CCorvusState_AVoid::OnDestroy()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_AVoid::Call_AnimationEnd, this, placeholders::_1);
}

_bool CCorvusState_AVoid::Check_AndChangeNextState()
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

			_flag TalentEffectFlags = Get_OwnerPlayer()->Check_RequirementForTalentEffects();

			if (TalentEffectFlags & (_flag)TALENT_EFFECT_FLAG::LONG_AVOID_LV1)
			{
				Get_OwnerPlayer()->Change_State<CCorvusState_Long_AvoidF>();
				return true;
			}
			else if (TalentEffectFlags & (_flag)TALENT_EFFECT_FLAG::SHORT_AVOID_LV1)
			{
				Get_OwnerPlayer()->Change_State<CCorvusState_Short_AvoidF>();
				return true;
			}
			
			//¿©±â¼­ ¶ì¶ì¶ìÇØÁà¾ßÇÔ
			Get_OwnerPlayer()->Change_State<CCorvusState_Long_AvoidF>();
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

_bool CCorvusState_AVoid::Check_RuquireMnetRepeatAvoidkState()
{
	_uint iTargetKeyFrameMin = 80;
	_uint iTargetKeyFrameMax = 999;




	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(iTargetKeyFrameMin, iTargetKeyFrameMax) && m_IsAgainAvoid)
	{
		return true;
	}



	return false;
}

