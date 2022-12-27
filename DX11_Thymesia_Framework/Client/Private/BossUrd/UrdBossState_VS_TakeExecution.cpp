#include "stdafx.h"
#include "BossUrd/UrdBossState_VS_TakeExecution.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"

GAMECLASS_C(CUrdBossState_VS_TakeExecution);
CLONE_C(CUrdBossState_VS_TakeExecution, CComponent)

HRESULT CUrdBossState_VS_TakeExecution::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_VS_TakeExecution::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_VS_TakeExecution::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_VS_TakeExecution|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_VS_TakeExecution::Call_AnimationEnd, this);
}

void CUrdBossState_VS_TakeExecution::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_VS_TakeExecution::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_VS_TakeExecution::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_VS_TakeExecution::OnStateEnd()
{
	__super::OnStateEnd();

}



void CUrdBossState_VS_TakeExecution::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_VS_TakeExecution::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_VS_TakeExecution::Call_AnimationEnd, this);
}

void CUrdBossState_VS_TakeExecution::Free()
{

}

_bool CUrdBossState_VS_TakeExecution::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

