#include "stdafx.h"
#include "EliteMonState/Joker/JokerState_RunAtkLoop.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "JokerStates.h"
#include "PhysXController.h"

GAMECLASS_C(CJokerState_RunAttackLoop);
CLONE_C(CJokerState_RunAttackLoop, CComponent)

void CJokerState_RunAttackLoop::Call_NextKeyFrame(const _uint& In_KeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_KeyIndex)
	{
	case 13:
		GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.3f, 1.f, 9.f, 0.1f);
		break;
	case 27:
		GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.3f, 1.f, 9.f, 0.1f);
		break;
	}
}

HRESULT CJokerState_RunAttackLoop::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CJokerState_RunAttackLoop::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CJokerState_RunAttackLoop::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Joker_RunAttackLoop");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CJokerState_RunAttackLoop::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_RunAttackLoop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
	PxControllerFilters Filters;
	m_pPhysXControllerCom.lock()->MoveWithRotation({ 0.f, 0.f, m_fCurrentSpeed * fTimeDelta }, 0.f, fTimeDelta, Filters, nullptr, m_pTransformCom);
}


void CJokerState_RunAttackLoop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	TurnAttack(fTimeDelta);
	Check_AndChangeNextState();
}



void CJokerState_RunAttackLoop::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CJokerState_RunAttackStart>().lock() ||
		Get_OwnerCharacter().lock()->Get_CurState().lock() == Get_Owner().lock()->Get_Component<CJokerState_RunAttackLoop>().lock())
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex, 16);
	else
		m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	m_pThisAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey +=
		bind(&CJokerState_RunAttackLoop::Call_NextKeyFrame, this, placeholders::_1);

#ifdef _DEBUG_COUT_
	cout << "VargState: Idle -> OnStateStart" << endl;
#endif // _DEBUG_COUT_
}

void CJokerState_RunAttackLoop::OnStateEnd()
{
	__super::OnStateEnd();
	m_pThisAnimationCom.lock()->CallBack_NextChannelKey -=
		bind(&CJokerState_RunAttackLoop::Call_NextKeyFrame, this, placeholders::_1);
}

void CJokerState_RunAttackLoop::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CJokerState_RunAttackLoop>(0.05f);
}

void CJokerState_RunAttackLoop::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CJokerState_RunAttackLoop::Call_AnimationEnd, this, placeholders::_1);
}

void CJokerState_RunAttackLoop::Free()
{
}

_bool CJokerState_RunAttackLoop::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	weak_ptr<CPlayer> pCurrentPlayer = GET_SINGLE(CGameManager)->Get_CurrentPlayer();

	if (!pCurrentPlayer.lock())
		return false;

	_float fPToMDistance(Get_DistanceWithPlayer()); // 플레이어와 몬스터 거리

	if (fPToMDistance <= 2.f)
	{
		Get_OwnerCharacter().lock()->Change_State<CJokerState_RunAtkEnd>(0.05f);
		return true;
	}

	return false;
}

