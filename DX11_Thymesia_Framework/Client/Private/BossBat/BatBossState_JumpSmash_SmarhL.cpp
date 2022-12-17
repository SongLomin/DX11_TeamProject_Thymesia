 #include "stdafx.h"
#include "BossBat/BatBossState_JumpSmash_SmarhL.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossBat/BatStates.h"
#include "PhysXController.h"

GAMECLASS_C(CBatBossState_JumpSmash_SmarhL);
CLONE_C(CBatBossState_JumpSmash_SmarhL, CComponent)

HRESULT CBatBossState_JumpSmash_SmarhL::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CBatBossState_JumpSmash_SmarhL::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CBatBossState_JumpSmash_SmarhL::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_BossBat_NEW_V1.ao|BossBat_JumpSmashL");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CBatBossState_JumpSmash_SmarhL::Call_AnimationEnd, this);
}

void CBatBossState_JumpSmash_SmarhL::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CBatBossState_JumpSmash_SmarhL::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_bAttackLookAtLimit)
	{
		TurnAttack(fTimeDelta);

	}
	if(m_bTurn)
	{
		JumpLookOffsetLookAt();
	}

	Check_AndChangeNextState();
}


void CBatBossState_JumpSmash_SmarhL::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pPhysXControllerCom.lock()->Enable_Gravity(false);

	JumpLookOffsetLookAt();

	m_bAttackLookAtLimit = true;

	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	
}	

void CBatBossState_JumpSmash_SmarhL::OnStateEnd()
{
	__super::OnStateEnd();

	m_pPhysXControllerCom.lock()->Enable_Gravity(false);


}


void CBatBossState_JumpSmash_SmarhL::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CBatBossState_Idle>(0.05f);
}

void CBatBossState_JumpSmash_SmarhL::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CBatBossState_JumpSmash_SmarhL::Call_AnimationEnd, this);
}

void CBatBossState_JumpSmash_SmarhL::Free()
{
}

_bool CBatBossState_JumpSmash_SmarhL::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() == 177)
	{
		m_pPhysXControllerCom.lock()->Enable_Gravity(true);
	}




	return false;
}

