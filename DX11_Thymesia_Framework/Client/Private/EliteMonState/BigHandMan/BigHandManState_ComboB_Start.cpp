#include "stdafx.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "EliteMonState/BigHandMan/BigHandManStates.h"



GAMECLASS_C(CBigHandManState_ComboB_Start);
CLONE_C(CBigHandManState_ComboB_Start, CComponent)

HRESULT CBigHandManState_ComboB_Start::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBigHandManState_ComboB_Start::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBigHandManState_ComboB_Start::Start()
{
	__super::Start();

	//턴이나 턴어택에서 아이들로 들어오면 워크로 들어오기 

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV2Villager_M.ao|LV2Villager01_M_ComboB_Start");


	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBigHandManState_ComboB_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CBigHandManState_ComboB_Start::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bAttackLookAtLimit)
	{
		TurnAttack(fTimeDelta);
	}

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBigHandManState_ComboB_Start::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CBigHandManState_ComboB_Start::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_bAttackLookAtLimit = true;


	if (m_bFourAttackEnd && m_iAttackCount == 0)
	{
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
		m_iAttackCount++;
	}

	if (m_bFourAttackEnd && m_iAttackCount > 0)
	{
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex , 43);
	}

	if(!m_bFourAttackEnd && m_iAttackCount > 2)
	{
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex , 43);
	}

	if (m_bOneAttackEnd || m_bOneAttackRun)
	{
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	}

	

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "BigHandManState: Idle -> OnStateStart" << endl;
#endif
#endif


}

void CBigHandManState_ComboB_Start::OnStateEnd()
{
	__super::OnStateEnd();


}

void CBigHandManState_ComboB_Start::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	

	if (m_bOneAttackEnd)
	{
		m_bOneAttackEnd = false;
		Get_OwnerCharacter().lock()->Change_State<CBigHandManState_ComboB_End>(0.05f);
	}
	if (m_bOneAttackRun)
	{
		m_bOneAttackRun = false;
		Get_OwnerCharacter().lock()->Change_State<CBigHandManState_ComboB_Loop>(0.05f);
	}

	
}

void CBigHandManState_ComboB_Start::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBigHandManState_ComboB_Start::Call_AnimationEnd, this, placeholders::_1);
}

void CBigHandManState_ComboB_Start::Free()
{

}

_bool CBigHandManState_ComboB_Start::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.3f)
	{
		m_bAttackLookAtLimit = false;
	}

	if (ComputeAngleWithPlayer() > 0.99f && m_bAttackLookAtLimit)
	{
		Rotation_TargetToLookDir();
	}

	if (m_iAttackCount > 3)
	{
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 72)
		{
			Get_OwnerCharacter().lock()->Change_State<CBigHandManState_ComboB_End>(0.05f);
			m_iAttackCount = 0;
			return true;
		}
	}


	if (m_bFourAttackEnd)
	{
		if (m_iAttackCount > 2)
		{
			if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 64)
			{
				Get_OwnerCharacter().lock()->Change_State<CBigHandManState_ComboB_Start>(0.05f);
				m_iAttackCount++;
				m_bFourAttackEnd = false;
				return true;
			}
		}
		else
		{
			if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() >= 64)
			{
				Get_OwnerCharacter().lock()->Change_State<CBigHandManState_ComboB_Start>(0.05f);
				m_iAttackCount++;
				return true;
			}
		}
		
	}




	return false;
}

