#include "stdafx.h"
#include "CorvusStates/StateExecution/CorvusState_BigHandman_Execution.h"
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

GAMECLASS_C(CCorvusState_BigHandman_Execution	);
CLONE_C(CCorvusState_BigHandman_Execution, CComponent)

HRESULT CCorvusState_BigHandman_Execution::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_BigHandman_Execution::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_BigHandman_Execution::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_VSLV2Villager01_M_Execution");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_BigHandman_Execution::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_BigHandman_Execution::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_BigHandman_Execution::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_BigHandman_Execution::OnDisable()
{

}

void CCorvusState_BigHandman_Execution::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);


	m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_BigHandman_Execution::Call_NextAnimationKey, this, placeholders::_1);


	//GET_SINGLE(CGameManager)->Start_Cinematic(m_pModelCom, "camera",XMMatrixIdentity());
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	
#endif
#endif

}

void CCorvusState_BigHandman_Execution::OnStateEnd()
{
	__super::OnStateEnd();
	//GET_SINGLE(CGameManager)->End_Cinematic();
	m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey -= bind(&CCorvusState_BigHandman_Execution::Call_NextAnimationKey, this, placeholders::_1);
}

void CCorvusState_BigHandman_Execution::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	
	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();
}

void CCorvusState_BigHandman_Execution::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;
}

void CCorvusState_BigHandman_Execution::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	CPlayerStateBase::OnHit(pMyCollider, pOtherCollider, In_eHitType, In_fDamage);
}

void CCorvusState_BigHandman_Execution::OnEventMessage(weak_ptr<CBase> pArg)
{
	//m_pTargetObject = Weak_Cast<CGameObject>(pArg);
	//Weak_Cast<CMonster>(m_pTargetObject).lock()->Change_State<CNorMonState_Die>();
}

void CCorvusState_BigHandman_Execution::Free()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_BigHandman_Execution::Call_AnimationEnd, this, placeholders::_1);
}

_bool CCorvusState_BigHandman_Execution::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	return false;
}

