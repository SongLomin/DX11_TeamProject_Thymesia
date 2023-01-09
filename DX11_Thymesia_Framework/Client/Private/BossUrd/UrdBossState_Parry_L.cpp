#include "stdafx.h"
#include "BossUrd/UrdBossState_Parry_L.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "Character.h"
#include "BossUrd/UrdStates.h"

GAMECLASS_C(CUrdBossState_Parry_L);
CLONE_C(CUrdBossState_Parry_L, CComponent)

HRESULT CUrdBossState_Parry_L::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CUrdBossState_Parry_L::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CUrdBossState_Parry_L::Start()
{
	__super::Start();

	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Urd_Parry_L|BaseLayer");

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CUrdBossState_Parry_L::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Parry_L::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}


void CUrdBossState_Parry_L::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CUrdBossState_Parry_L::OnStateStart(const _float& In_fAnimationBlendTime)
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


void CUrdBossState_Parry_L::OnStateEnd()
{
	__super::OnStateEnd();

	Weak_StaticCast<CUrd>(Get_OwnerCharacter()).lock()->Set_MoveScale(_float3(1.f, 1.f, 1.f));

}



void CUrdBossState_Parry_L::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CUrdBossState_StepB>(0.05f);
}

void CUrdBossState_Parry_L::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CUrdBossState_Parry_L::Call_AnimationEnd, this, placeholders::_1);
}

void CUrdBossState_Parry_L::Free()
{

}

_bool CUrdBossState_Parry_L::Check_AndChangeNextState()
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

