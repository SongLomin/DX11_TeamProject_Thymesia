#include "stdafx.h"
#include "BossUrd/UrdBossState_StepB.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"

GAMECLASS_C(CUrdBossState_StepB);
CLONE_C(CUrdBossState_StepB, CComponent)

HRESULT CUrdBossState_StepB::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_StepB::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_StepB::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_StepB|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_StepB::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_StepB::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_StepB::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_StepB::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(2.5f, 2.5f, 2.5f));

	Rotation_TargetToLookDir();

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex , 4);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	
}	

void CUrdBossState_StepB::OnStateEnd()
{
	__super::OnStateEnd();

	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.f, 1.f, 1.f));
}



void CUrdBossState_StepB::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_StepB::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CBossStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
}

void CUrdBossState_StepB::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_StepB::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_StepB::Free()
{

}

_bool CUrdBossState_StepB::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	//만약 스킬그게트루인상태에서는 바로 칼꽂게

	if (Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Get_SkillStart())
	{
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.8f)
		{
			int iRand = rand() % 4;
			switch (iRand)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Skill01>(0.05f);
				Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SkillStart(false);
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Skill03_L>(0.05f);
				Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SkillStart(false);
				break;
			case 2:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Skill03_R>(0.05f);
				Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SkillStart(false);
				break;
			case 3:
				Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Skill01>(0.05f);
				Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_SkillStart(false);
				break;
			}
			return true;
		}
		
	}
	else
	{
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.8f)
		{
			Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
			return true;
		}
	}

	

	return false;
}

