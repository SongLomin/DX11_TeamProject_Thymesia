#include "stdafx.h"
#include "BossUrd/UrdBossState_StunStart.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"
#include "PhysXController.h"

GAMECLASS_C(CUrdBossState_StunStart);
CLONE_C(CUrdBossState_StunStart, CComponent)

HRESULT CUrdBossState_StunStart::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_StunStart::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_StunStart::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_StunStart|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_StunStart::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_StunStart::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_StunStart::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_StunStart::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	Get_OwnerMonster()->Set_RimLightDesc(6.f, { 0.5f,1.f,0.9f }, 1.f);
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_StunStart::OnStateEnd()
{
	__super::OnStateEnd();

}



void CUrdBossState_StunStart::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StunLoop>(0.05f);
}

void CUrdBossState_StunStart::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_StunStart::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_StunStart::Free()
{

}

_bool CUrdBossState_StunStart::Check_AndChangeNextState()
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

