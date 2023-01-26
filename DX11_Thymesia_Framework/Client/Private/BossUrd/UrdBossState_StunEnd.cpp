#include "stdafx.h"
#include "BossUrd/UrdBossState_StunEnd.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"

GAMECLASS_C(CUrdBossState_StunEnd);
CLONE_C(CUrdBossState_StunEnd, CComponent)

HRESULT CUrdBossState_StunEnd::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_StunEnd::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_StunEnd::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_StunEnd|BaseLayer");
	
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_StunEnd::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_StunEnd::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_StunEnd::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_StunEnd::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_StunEnd::OnStateEnd()
{
	__super::OnStateEnd();

}



void CUrdBossState_StunEnd::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_StunEnd::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_StunEnd::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_StunEnd::Free()
{

}

_bool CUrdBossState_StunEnd::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

