#include "stdafx.h"
#include "BossUrd/UrdBossState_WalkF.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"
#include "PhysXController.h"

GAMECLASS_C(CUrdBossState_WalkF);
CLONE_C(CUrdBossState_WalkF, CComponent)

HRESULT CUrdBossState_WalkF::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_WalkF::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_WalkF::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_WalkF|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_WalkF::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_WalkF::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	Rotation_TargetToLookDir();

	m_fCurrentSpeed += m_fAccel * fTimeDelta;
	m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);

	m_pModelCom.lock()->Play_Animation(fTimeDelta);

	PxControllerFilters Filters;
	m_pPhysXControllerCom.lock()->MoveWithRotation({ 0.f, 0.f, m_fCurrentSpeed * fTimeDelta }, 0.f, fTimeDelta, Filters, nullptr, m_pTransformCom);
}


void CUrdBossState_WalkF::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_WalkF::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CUrdBossState_Start>().lock())
	{
		m_bOnce = true;
		m_iRepeatAnimation = 2;
	}

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	
	
	
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "UrdState: Start -> OnStateStart" << endl;
#endif
#endif
	

}	


void CUrdBossState_WalkF::OnStateEnd()
{
	__super::OnStateEnd();

	m_iRepeatAnimation += 1;

}



void CUrdBossState_WalkF::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	//Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
}

void CUrdBossState_WalkF::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_WalkF::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_WalkF::Free()
{

}

_bool CUrdBossState_WalkF::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_iRepeatAnimation >= 2)
	{
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() > 0.95f)
		{
			Get_OwnerCharacter().lock()->Change_State<CUrdBossState_Idle>(0.05f);
			return true;
		}
	}


	



	return false;
}

