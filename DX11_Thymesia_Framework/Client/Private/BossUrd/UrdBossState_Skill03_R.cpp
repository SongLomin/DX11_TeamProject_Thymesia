#include "stdafx.h"
#include "BossUrd/UrdBossState_Skill03_R.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"

GAMECLASS_C(CUrdBossState_Skill03_R);
CLONE_C(CUrdBossState_Skill03_R, CComponent)

HRESULT CUrdBossState_Skill03_R::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Skill03_R::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_Skill03_R::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Skill03_R|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_Skill03_R::Call_AnimationEnd, this);
}

void CUrdBossState_Skill03_R::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bAtkPermitAndNoHurt)
	{
		TurnAttack(fTimeDelta * 2.f);
	}
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_Skill03_R::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_Skill03_R::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.5f, 1.5f, 1.5f));

	m_bAttackLookAtLimit = true;

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_Skill03_R::OnStateEnd()
{
	__super::OnStateEnd();

	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.f, 1.f, 1.f));
}



void CUrdBossState_Skill03_R::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;
	
	Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SkillStart(false);
	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_Skill03_R::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_Skill03_R::Call_AnimationEnd, this);
}

void CUrdBossState_Skill03_R::Free()
{

}

_bool CUrdBossState_Skill03_R::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.5f)
		m_bAttackLookAtLimit = false;

	if (ComputeAngleWithPlayer() > 0.99f && m_bAttackLookAtLimit)
		Rotation_TargetToLookDir();

	return false;
}

