#include "stdafx.h"
#include "CorvusStates/CorvusState_Short_AvoidF.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "Weapon.h"

GAMECLASS_C(CCorvusState_Short_AvoidF);
CLONE_C(CCorvusState_Short_AvoidF, CComponent)

HRESULT CCorvusState_Short_AvoidF::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_Short_AvoidF::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	m_fNextCombatRatio = 0.23f;
	m_fNextNonCombatRatio = 0.6f;
	m_fNextAvoidRatio = 0.46f;
	return S_OK;
}

void CCorvusState_Short_AvoidF::Start()
{
	__super::Start();
	
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_AvoidShort_F");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Short_AvoidF::Call_AnimationEnd, this);
}

void CCorvusState_Short_AvoidF::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_Short_AvoidF::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CCorvusState_Short_AvoidF::OnDisable()
{

}

void CCorvusState_Short_AvoidF::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CCorvusState_Short_AvoidF>().lock())
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

void CCorvusState_Short_AvoidF::OnStateEnd()
{
	__super::OnStateEnd();


}

void CCorvusState_Short_AvoidF::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_Short_AvoidF::Play_AttackWithIndex(const _tchar& In_iAttackIndex)
{
	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
}

void CCorvusState_Short_AvoidF::Free()
{

}

void CCorvusState_Short_AvoidF::OnDestroy()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Short_AvoidF::Call_AnimationEnd, this);
}

_bool CCorvusState_Short_AvoidF::Check_AndChangeNextState()
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
			Get_OwnerPlayer()->Change_State<CCorvusState_Short_AvoidF>();
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

_bool CCorvusState_Short_AvoidF::Check_RuquireMnetRepeatAvoidkState()
{
	_uint iTargetKeyFrameMin = 80;
	_uint iTargetKeyFrameMax = 999;




	if (m_pModelCom.lock()->Is_CurrentAnimationKeyInRange(iTargetKeyFrameMin, iTargetKeyFrameMax) && m_IsAgainAvoid)
	{
		return true;
	}



	return false;
}

