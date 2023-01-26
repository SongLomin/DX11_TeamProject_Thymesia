#include "stdafx.h"
#include "BossUrd/UrdBossState_StunLoop.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"
#include "PhysXController.h"


GAMECLASS_C(CUrdBossState_StunLoop);
CLONE_C(CUrdBossState_StunLoop, CComponent)

HRESULT CUrdBossState_StunLoop::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_StunLoop::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_StunLoop::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_StunLoop|BaseLayer");

}

void CUrdBossState_StunLoop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_StunLoop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_StunLoop::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_StunLoop::OnStateEnd()
{
	__super::OnStateEnd();

}

void CUrdBossState_StunLoop::Free()
{

}

_bool CUrdBossState_StunLoop::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (Get_OwnerMonster()->Get_BossExecutionStartOnOff())
	{
		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_VS_TakeExecution>(0.05f);
		return true;
	}

	return false;
}

