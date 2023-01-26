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

GAMECLASS_C(CBatBossState_Idle);
CLONE_C(CBatBossState_Idle, CComponent)

HRESULT CBatBossState_Idle::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_Idle::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CBatBossState_Idle::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("BossBat_Idle");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_Idle::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_Idle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CBatBossState_Idle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}

void CBatBossState_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
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

void CBatBossState_Idle::OnStateEnd()
{
	__super::OnStateEnd();

}

void CBatBossState_Idle::Free()
{

}

void CBatBossState_Idle::Call_AnimationEnd(_uint iEndAnimIndex)
{

}

void CBatBossState_Idle::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_Idle::Call_AnimationEnd, this, placeholders::_1);
}

void CBatBossState_Idle::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if ((_uint)EVENT_TYPE::ON_RESET_OBJ == iArg)
	{
		m_bHeelScream = false;
		m_bChestCheck = false;
	}

}

_bool CBatBossState_Idle::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_bHeelScream) //Çï¸ðµåÀÏ¶§
	{
		Get_OwnerCharacter().lock()->Change_State<CBatBossState_HellIdle>(0.05f);
		return true;
	}
	else //Çï¸ðµå¾Æ´Ò‹š
	{
		if (m_bChestCheck)
		{
			Get_OwnerCharacter().lock()->Change_State<CBatBossState_ChargeIdle>(0.05f);
		}
		else
		{
			Get_OwnerCharacter().lock()->Change_State<CBatBossState_AttackIdle>(0.05f);
		}
		return true;
	}

	return false;
}

