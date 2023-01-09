#include "stdafx.h"
#include "BossUrd/UrdBossState_TurnL.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"

GAMECLASS_C(CUrdBossState_TurnL);
CLONE_C(CUrdBossState_TurnL, CComponent)

HRESULT CUrdBossState_TurnL::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_TurnL::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_TurnL::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_WalkF|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_TurnL::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_TurnL::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	_float fTurnValue = 1.57f / 0.683f;

	m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * fTurnValue * -2.f);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_TurnL::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_TurnL::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "VargState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_TurnL::OnStateEnd()
{
	__super::OnStateEnd();

}



void CUrdBossState_TurnL::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_TurnL::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_TurnL::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_TurnL::Free()
{

}

_bool CUrdBossState_TurnL::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (ComputeAngleWithPlayer() > 0.94f)
	{
		Rotation_TargetToLookDir();
		Get_Owner().lock()->Get_Component<CUrdBossState_Idle>().lock()->Set_TurnCheck(false);
		Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
		return true;
	}

	return false;
}

