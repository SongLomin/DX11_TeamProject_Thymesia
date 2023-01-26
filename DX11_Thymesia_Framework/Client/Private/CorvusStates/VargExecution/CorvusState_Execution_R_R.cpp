#include "stdafx.h"
#include "CorvusStates/StateExecution/CorvusState_Execution_R_R.h"
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

GAMECLASS_C(CCorvusState_Execution_R_R);
CLONE_C(CCorvusState_Execution_R_R, CComponent)

HRESULT CCorvusState_Execution_R_R::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_Execution_R_R::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_Execution_R_R::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_StunExecute_StartR_R");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Execution_R_R::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_Execution_R_R::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_Execution_R_R::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_Execution_R_R::OnDisable()
{

}

void CCorvusState_Execution_R_R::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_Execution_R_R::Call_NextAnimationKey, this, placeholders::_1);


	//GET_SINGLE(CGameManager)->Start_Cinematic(m_pModelCom, "camera",XMMatrixIdentity());
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	
#endif
#endif

}

void CCorvusState_Execution_R_R::OnStateEnd()
{
	__super::OnStateEnd();

	m_bExcution = false;
	m_eExcutionTargetType = MONSTERTYPE::TYPE_END;
	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CCorvusState_Execution_R_R::Call_NextAnimationKey, this, placeholders::_1);
	
}

void CCorvusState_Execution_R_R::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();
}

void CCorvusState_Execution_R_R::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_iKeyIndex)
	{
	case 27:
		m_bExcution = true;
		break;
	}
        
	


}



void CCorvusState_Execution_R_R::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CPlayerStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
}

void CCorvusState_Execution_R_R::Free()
{
	
}

void CCorvusState_Execution_R_R::OnDestroy()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Execution_R_R::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_Execution_R_R::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);


	if (EVENT_TYPE::ON_BIGHANDMANEXECUTION == (EVENT_TYPE)iArg)
	{
		
		m_eExcutionTargetType = MONSTERTYPE::BIGHANDMAN;
	}

	if (EVENT_TYPE::ON_ARMOREXECUTIONSPEAR == (EVENT_TYPE)iArg)
	{
		m_eExcutionTargetType = MONSTERTYPE::ARMORSPEARMAN;
	}

	if (EVENT_TYPE::ON_URDEXECUTON == (EVENT_TYPE)iArg)
	{
		m_eExcutionTargetType = MONSTERTYPE::URD;
	}

	if ((_uint)EVENT_TYPE::ON_VARGEXECUTION == iArg)
	{
		m_eExcutionTargetType = MONSTERTYPE::VARG;
	}

	if ((_uint)EVENT_TYPE::ON_JOKEREXECUTION == iArg)
	{
		m_eExcutionTargetType = MONSTERTYPE::JOKER;
	}

}

_bool CCorvusState_Execution_R_R::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;



	
	if (m_bExcution)
	{
		switch (m_eExcutionTargetType)
		{
		case Client::MONSTERTYPE::ARMORSPEARMAN:
		case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
			GET_SINGLE(CGameManager)->OnEventMessageForLayer(OBJECT_LAYER::MONSTER, EVENT_TYPE::ON_ARMOREXECUTIONSTART);
			Get_OwnerPlayer()->Change_State<CCorvusState_AromorLV1_NCamera_Execution>();
			break;
		case Client::MONSTERTYPE::JOKER:
			GET_SINGLE(CGameManager)->OnEventMessageForLayer(OBJECT_LAYER::ELITEMONSTER, EVENT_TYPE::ON_JOKEREXECUTIONSTART);
			Get_OwnerPlayer()->Change_State<CCorvusState_Joker_Execution>();
			break;
		case Client::MONSTERTYPE::BIGHANDMAN:
			GET_SINGLE(CGameManager)->OnEventMessageForLayer(OBJECT_LAYER::ELITEMONSTER, EVENT_TYPE::ON_BIGHANDMANEXECUTIONSTART);
			Get_OwnerPlayer()->Change_State<CCorvusState_BigHandman_Execution>();
			break;
		case Client::MONSTERTYPE::VARG:
			GET_SINGLE(CGameManager)->OnEventMessageForLayer(OBJECT_LAYER::BOSSMONSTER, EVENT_TYPE::ON_BOSS_EXECUTIONSTART);
			Get_OwnerPlayer()->Change_State<CCorvusState_Varg_Execution>();
			break;
		case Client::MONSTERTYPE::URD:
			GET_SINGLE(CGameManager)->OnEventMessageForLayer(OBJECT_LAYER::BOSSMONSTER, EVENT_TYPE::ON_BOSS_EXECUTIONSTART);
			Get_OwnerPlayer()->Change_State<CCorvusState_Urd_Execution>();
			break;
		case Client::MONSTERTYPE::TYPE_END:
			DEBUG_ASSERT;
			break;
		default:
			DEBUG_ASSERT;
			break;
		}

		return true;
	}
		
	return false;
}

