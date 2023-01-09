#include "stdafx.h"
#include "BossUrd/UrdBossState_WalkB.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"

GAMECLASS_C(CUrdBossState_WalkB);
CLONE_C(CUrdBossState_WalkB, CComponent)

HRESULT CUrdBossState_WalkB::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_WalkB::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_WalkB::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_WalkB|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_WalkB::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_WalkB::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_WalkB::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_WalkB::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_WalkB::OnStateEnd()
{
	__super::OnStateEnd();

}



void CUrdBossState_WalkB::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_WalkB::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_WalkB::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_WalkB::Free()
{

}

_bool CUrdBossState_WalkB::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

