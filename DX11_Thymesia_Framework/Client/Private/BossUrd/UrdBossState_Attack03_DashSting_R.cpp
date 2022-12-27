#include "stdafx.h"
#include "BossUrd/UrdBossState_Attack03_DashSting_R.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"

GAMECLASS_C(CUrdBossState_Attack03_DashSting_R);
CLONE_C(CUrdBossState_Attack03_DashSting_R, CComponent)

HRESULT CUrdBossState_Attack03_DashSting_R::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Attack03_DashSting_R::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_Attack03_DashSting_R::Start()
{
	__super::Start();
	                                                          
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Attack04_DashSting_R|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_Attack03_DashSting_R::Call_AnimationEnd, this);
}

void CUrdBossState_Attack03_DashSting_R::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_Attack03_DashSting_R::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_Attack03_DashSting_R::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_Attack03_DashSting_R::OnStateEnd()
{
	__super::OnStateEnd();

}



void CUrdBossState_Attack03_DashSting_R::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_Attack03_DashSting_R::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_Attack03_DashSting_R::Call_AnimationEnd, this);
}

void CUrdBossState_Attack03_DashSting_R::Free()
{

}

_bool CUrdBossState_Attack03_DashSting_R::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

