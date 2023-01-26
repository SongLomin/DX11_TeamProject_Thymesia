#include "stdafx.h"
#include "BossUrd/UrdBossState_HurtM_FR.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"

GAMECLASS_C(CUrdBossState_HurtM_FR);
CLONE_C(CUrdBossState_HurtM_FR, CComponent)

HRESULT CUrdBossState_HurtM_FR::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_HurtM_FR::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_HurtM_FR::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_HurtM_FR|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_HurtM_FR::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_HurtM_FR::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_HurtM_FR::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_HurtM_FR::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_HurtM_FR::OnStateEnd()
{
	__super::OnStateEnd();

}



void CUrdBossState_HurtM_FR::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_HurtM_FR::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_HurtM_FR::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_HurtM_FR::Free()
{

}

_bool CUrdBossState_HurtM_FR::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

