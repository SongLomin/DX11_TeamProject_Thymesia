#include "stdafx.h"
#include "BossUrd/UrdBossState_WalkFL.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"

GAMECLASS_C(CUrdBossState_WalkFL);
CLONE_C(CUrdBossState_WalkFL, CComponent)

HRESULT CUrdBossState_WalkFL::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_WalkFL::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_WalkFL::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_WalkFL|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_WalkFL::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_WalkFL::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_WalkFL::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_WalkFL::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_WalkFL::OnStateEnd()
{
	__super::OnStateEnd();

}



void CUrdBossState_WalkFL::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_WalkFL::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_WalkFL::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_WalkFL::Free()
{

}

_bool CUrdBossState_WalkFL::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

