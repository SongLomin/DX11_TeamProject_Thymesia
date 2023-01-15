#include "stdafx.h"
#include "CorvusStates/StateExecution/CorvusState_NorMob_Execution.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "Monster.h"
#include "NorMonStates.h"

GAMECLASS_C(CCorvusState_NorMob_Execution);
CLONE_C(CCorvusState_NorMob_Execution, CComponent)

HRESULT CCorvusState_NorMob_Execution::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_NorMob_Execution::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_NorMob_Execution::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_StunExecute_StartL_L");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_NorMob_Execution::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_NorMob_Execution::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_NorMob_Execution::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_NorMob_Execution::OnDisable()
{

}

void CCorvusState_NorMob_Execution::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_NorMob_Execution::Call_NextAnimationKey, this, placeholders::_1);

	_flag TalentEffectFlags = Get_OwnerPlayer()->Check_RequirementForTalentEffects();

	if (TalentEffectFlags & (_flag)TALENT_EFFECT_FLAG::HEALING_EXELCUTION_LV2)
	{
		GET_SINGLE(CGameManager)->Get_CurrentPlayer_Status().lock()->Heal_PlayerFromMaxHP(0.4f);
		GET_SINGLE(CGameManager)->Get_CurrentPlayer_Status().lock()->MPHeal_PlayerFromMaxMP(0.4f);
	}
	else if (TalentEffectFlags & (_flag)TALENT_EFFECT_FLAG::HEALING_EXELCUTION_LV1)
	{
		GET_SINGLE(CGameManager)->Get_CurrentPlayer_Status().lock()->Heal_PlayerFromMaxHP(0.2f);
		GET_SINGLE(CGameManager)->Get_CurrentPlayer_Status().lock()->MPHeal_PlayerFromMaxMP(0.2f);
	}
	//GET_SINGLE(CGameManager)->Start_Cinematic(m_pModelCom, "camera",XMMatrixIdentity());
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	
#endif
#endif

}

void CCorvusState_NorMob_Execution::OnStateEnd()
{
	__super::OnStateEnd();
	//GET_SINGLE(CGameManager)->End_Cinematic();
	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CCorvusState_NorMob_Execution::Call_NextAnimationKey, this, placeholders::_1);
}

void CCorvusState_NorMob_Execution::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	
	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();
}

void CCorvusState_NorMob_Execution::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;
}

void CCorvusState_NorMob_Execution::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CPlayerStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
}

void CCorvusState_NorMob_Execution::OnEventMessage(weak_ptr<CBase> pArg)
{
	m_pTargetObject = Weak_Cast<CGameObject>(pArg);
	Weak_Cast<CMonster>(m_pTargetObject).lock()->Change_State<CNorMonState_Die>();
}

void CCorvusState_NorMob_Execution::Free()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_NorMob_Execution::Call_AnimationEnd, this, placeholders::_1);
}

_bool CCorvusState_NorMob_Execution::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	//if (Check_RequirementUltimateState())
	//{
	//	Rotation_NearToLookDir();
	//	Get_OwnerPlayer()->Change_State<CNorMonState_UltimateSkill>();
	//	return true;
	//}
	//
	//if (Check_RequirementAttackState())
	//{
	//	if (!Rotation_InputToLookDir())
	//		Rotation_NearToLookDir();
	//
	//	Get_OwnerPlayer()->Change_State<CNorMonState_Attack>();
	//	Get_OwnerPlayer()->Get_Component<CNorMonState_Attack>().lock()->Play_AttackWithIndex(0);
	//	return true;
	//}

	return false;
}

