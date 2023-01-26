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

void CBigHandManState_ComboB_Start::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	switch (In_KeyIndex)
	{
	case 49:
	{
		_matrix OwnerWorldMatrix = m_pOwner.lock()->Get_Transform()->Get_WorldMatrix();
		_vector vShakingOffset = XMVectorSet(0.f, -1.f, 0.f, 0.f);
		vShakingOffset = XMVector3TransformNormal(vShakingOffset, OwnerWorldMatrix);
		GET_SINGLE(CGameManager)->Add_Shaking(vShakingOffset, 0.3f, 1.f, 9.f, 0.4f);
	}
		return;
	}
}

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
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex , 38);

	}

	if(!m_bFourAttackEnd && m_iAttackCount > 2)
	{
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex , 38);

	}

	if (m_bOneAttackEnd || m_bOneAttackRun)
	{
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	}

	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CBigHandManState_ComboB_Start::Call_NextKeyFrame, this, placeholders::_1);
	
	
}

void CBigHandManState_ComboB_Start::OnStateEnd()
{
	__super::OnStateEnd();
	m_pModelCom.lock()->Set_AnimationSpeed(1.f);

	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CBigHandManState_ComboB_Start::Call_NextKeyFrame, this, placeholders::_1);
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

