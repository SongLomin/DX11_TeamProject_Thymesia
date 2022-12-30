#include "stdafx.h"
#include "BossUrd/UrdBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"

GAMECLASS_C(CUrdBossState_Step_Idle);
CLONE_C(CUrdBossState_Step_Idle, CComponent)

HRESULT CUrdBossState_Step_Idle::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Step_Idle::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_Step_Idle::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Idle|BaseLayer");

}

void CUrdBossState_Step_Idle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_Step_Idle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_Step_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_Step_Idle::OnStateEnd()
{
	__super::OnStateEnd();

}


void CUrdBossState_Step_Idle::Free()
{

}

_bool CUrdBossState_Step_Idle::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_iStepCount == 0)
	{
		_int iStepCount = ComputeDirectionToPlayer();

		switch (iStepCount)
		{
		case 1://¿À¸¥ÂÊ
			Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFL45>(0.05f);
			m_iStepCount = 1;
			return true;
			break;
		case -1:// ¿ÞÂÊ
			Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFR45>(0.05f);
			m_iStepCount = 2;
			return true;
			break;
		}
	}
	else if (m_iStepCount == 1)
	{
		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFR45>(0.05f);
		m_iStepCount = 3;
		return true;
	}
	else if (m_iStepCount == 2)
	{
		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepFL45>(0.05f);
		m_iStepCount = 4;
		return true;
	}
	else if (m_iStepCount == 3)
	{
		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack03_DashSting_L>(0.05f);
		m_iStepCount = 0;
		return true;
	}
	else if (m_iStepCount == 4)
	{
		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Attack03_DashSting_R>(0.05f);
		m_iStepCount = 0;
		return true;
	}

	


	return false;
}

