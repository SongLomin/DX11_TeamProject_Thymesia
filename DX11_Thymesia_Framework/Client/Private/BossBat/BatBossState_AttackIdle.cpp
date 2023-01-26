#include "stdafx.h"
#include "BossBat/BatBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "PhysXController.h"

GAMECLASS_C(CBatBossState_AttackIdle);
CLONE_C(CBatBossState_AttackIdle, CComponent)

HRESULT CBatBossState_AttackIdle::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_AttackIdle::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_AttackIdle::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_Idle");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_AttackIdle::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_AttackIdle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_AttackIdle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CBatBossState_AttackIdle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_Atk_L01_2b>().lock()         ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_Atk_R01_2b>().lock()         ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_Bite_2>().lock()             ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_Charge>().lock()             ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_Car>().lock()                ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_JumpSmash_Chest>().lock()    ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_JumpSmash_ForwardL>().lock() ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_JumpSmash_SmarhL>().lock()   ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_SonicBoom>().lock()          ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_Hellscream>().lock()         ||
		Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CBatBossState_Sp>().lock())
	{
		m_bTurnCheck = true;
	}



	m_pPhysXControllerCom.lock()->Enable_Gravity(true);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
}

void CBatBossState_AttackIdle::OnStateEnd()
{
	__super::OnStateEnd();

}


void CBatBossState_AttackIdle::Free()
{

}

void CBatBossState_AttackIdle::Call_AnimationEnd(_uint iEndAnimIndex)
{

}

void CBatBossState_AttackIdle::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_AttackIdle::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_AttackIdle::OnEventMessage(_uint iArg)
{
      __super::OnEventMessage(iArg);
      
      if ((_uint)EVENT_TYPE::ON_RESET_OBJ == iArg)
      {
		  m_iAttackCount = 0;
      }	
}

_bool CBatBossState_AttackIdle::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	//6.5기준이로잡으면될듯

	_float fPToMDistance = Get_DistanceWithPlayer();

	if (fPToMDistance <= 12.f)
	{
		if (m_iAttackCount  >  2)
		{
			Get_OwnerCharacter().lock()->Change_State<CBatBossState_BackJump>(0.05f);
			Get_Owner().lock()->Get_Component<CBatBossState_HellIdle>().lock()->Set_ZeroAttackCount(0);
			m_iAttackCount = 0;
			return true;
		}
		else
		{
			int iRand = rand() % 4;

			switch (iRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CBatBossState_Bite_1>(0.05f);
				break;
			case 1:
				Check_CrossAttackState();
				break;
			case 2:
				Check_CrossAttackState();
				break;
			case 3:
				Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_SmarhL>(0.05f);
				break;
			}
		}

		
		return true;
	}
	else if (fPToMDistance > 12.f && fPToMDistance <= 16.f)
	{
		int iRand = rand() % 2;


		if (m_iAttackCount == 1)
		{
			Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_ForwardL>(0.05f);
			return true;
		}
		else
		{
			switch (iRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CBatBossState_Storm>(0.05f);
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CBatBossState_BackJump>(0.05f);
				break;
			}
		}
		
		return true;
	}
	else
	{
		int iRand = rand() % 2;

		switch (iRand)
		{
		case 0:
			Get_OwnerCharacter().lock()->Change_State<CBatBossState_JumpSmash_ForwardL>(0.05f);
			break;
		case 1:
			Get_OwnerCharacter().lock()->Change_State<CBatBossState_Car>(0.05f);
			break;
		}
		return true;
	}


	return false;
}

