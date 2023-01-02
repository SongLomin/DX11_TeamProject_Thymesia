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

GAMECLASS_C(CUrdBossState_Walk_Idle);
CLONE_C(CUrdBossState_Walk_Idle, CComponent)

HRESULT CUrdBossState_Walk_Idle::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Walk_Idle::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_Walk_Idle::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Idle|BaseLayer");

	
}

void CUrdBossState_Walk_Idle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_Walk_Idle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_Walk_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_Walk_Idle::OnStateEnd()
{
	__super::OnStateEnd();

}



void CUrdBossState_Walk_Idle::Free()
{

}

_bool CUrdBossState_Walk_Idle::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	int iRand = rand() % 2;

	switch (iRand)
	{
	case 0:
		Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_WalkStart(false);
		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_WalkL>(0.05f);
		return true;
		break;
	case 1:
		Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_WalkStart(false);
		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_WalkR>(0.05f);
		return true;

	}

	return false;
}

