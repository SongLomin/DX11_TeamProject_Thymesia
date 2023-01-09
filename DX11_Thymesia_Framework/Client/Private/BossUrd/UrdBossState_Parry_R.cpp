#include "stdafx.h"
#include "BossUrd/UrdBossState_Parry_R.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"

GAMECLASS_C(CUrdBossState_Parry_R);
CLONE_C(CUrdBossState_Parry_R, CComponent)

HRESULT CUrdBossState_Parry_R::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Parry_R::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_Parry_R::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Parry_R|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_Parry_R::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Parry_R::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_Parry_R::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_Parry_R::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CBossStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
}

void CUrdBossState_Parry_R::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);


	Get_Owner().lock()->Get_Component<CUrdBossState_HurtS_FL>().lock()->Set_ParryZeroCount(0);
	Get_Owner().lock()->Get_Component<CUrdBossState_HurtS_FR>().lock()->Set_ParryZeroCount(0);

	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(2.f, 2.f, 2.f));

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_Parry_R::OnStateEnd()
{
	__super::OnStateEnd();

	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.f, 1.f, 1.f));
}



void CUrdBossState_Parry_R::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;
	

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepB>(0.05f);

}

void CUrdBossState_Parry_R::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_Parry_R::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Parry_R::Free()
{

}

_bool CUrdBossState_Parry_R::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.8f)
	{
		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepB>(0.05f);
		return true;
	}


	return false;
}

  