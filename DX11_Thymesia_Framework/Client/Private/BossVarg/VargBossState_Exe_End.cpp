#include "stdafx.h"
#include "BossVarg/VargBossState_Exe_End.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

GAMECLASS_C(CVargBossState_Exe_End);
CLONE_C(CVargBossState_Exe_End, CComponent)

HRESULT CVargBossState_Exe_End::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_Exe_End::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CVargBossState_Exe_End::Start()
{
	__super::Start();


	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_TakeExecution_Loop");



}

void CVargBossState_Exe_End::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_Exe_End::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);



	Check_AndChangeNextState();
}



void CVargBossState_Exe_End::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CBossStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
}

void CVargBossState_Exe_End::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	Weak_Cast<CVarg>(m_pOwner).lock()->Set_EyeTrailEnable(false);
	GET_SINGLE(CGameManager)->UnUse_EffectGroup("Varg_Eye", GET_SINGLE(CGameManager)->Get_StoredEffectIndex("Varg_Eye"));

	if (Check_RequirementIsTargeted())
		GET_SINGLE(CGameManager)->Release_Focus();

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Exe_End -> OnStateStart" << endl;
#endif // _DEBUG_COUT_
#endif // _DEBUG


}

void CVargBossState_Exe_End::OnStateEnd()
{
	__super::OnStateEnd();

	Get_OwnerMonster()->Release_Monster();
}



void CVargBossState_Exe_End::Free()
{

}

_bool CVargBossState_Exe_End::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

