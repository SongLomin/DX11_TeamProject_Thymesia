#include "stdafx.h"
#include "BossUrd/UrdBossState_WalkR.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"

GAMECLASS_C(CUrdBossState_WalkR);
CLONE_C(CUrdBossState_WalkR, CComponent)

HRESULT CUrdBossState_WalkR::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_WalkR::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_WalkR::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_WalkR|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_WalkR::Call_AnimationEnd, this);
}

void CUrdBossState_WalkR::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Rotation_TargetToLookDir();
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_WalkR::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_WalkR::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_WalkR::OnStateEnd()
{
	__super::OnStateEnd();

}



void CUrdBossState_WalkR::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_WalkR::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_WalkR::Call_AnimationEnd, this);
}

void CUrdBossState_WalkR::Free()
{

}

_bool CUrdBossState_WalkR::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

