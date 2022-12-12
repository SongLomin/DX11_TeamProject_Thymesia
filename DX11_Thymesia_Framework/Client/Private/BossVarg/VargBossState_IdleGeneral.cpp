#include "stdafx.h"
#include "BossVarg/VargBossState_IdleGeneral.h"
#include "VargBossStateBase.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "VargStates.h"

GAMECLASS_C(CVargBossState_IdleGeneral);
CLONE_C(CVargBossState_IdleGeneral, CComponent)

HRESULT CVargBossState_IdleGeneral::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CVargBossState_IdleGeneral::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	return S_OK;
}

void CVargBossState_IdleGeneral::Start()
{
	__super::Start();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Varg.ao|Varg_IdleGeneral");
}

void CVargBossState_IdleGeneral::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CVargBossState_IdleGeneral::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	CBossStateBase::Rotation_TargetToLookDir();
	this->Check_AndChangeNextState();
}



void CVargBossState_IdleGeneral::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: RunStart -> OnStateStart" << endl;
#endif // _DEBUG_COUT_
#endif// _DEBUG

	m_pModelCom.lock()->Set_AnimationSpeed(1.5f);
}

void CVargBossState_IdleGeneral::OnStateEnd()
{
	__super::OnStateEnd();
	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}



void CVargBossState_IdleGeneral::Free()
{
}

_bool CVargBossState_IdleGeneral::Check_AndChangeNextState()
{
	if (!CBossStateBase::Check_Requirement())
		return false;

	_float fPToMDistance(CBossStateBase::Get_DistanceWithPlayer()); // 플레이어와 몬스터 거리

	if (fPToMDistance <= 30.f)
	{
		CStateBase::Get_OwnerCharacter().lock()->Change_State<CVargBossState_Start>(0.05f);
		return true;
	}

	return false;
}

