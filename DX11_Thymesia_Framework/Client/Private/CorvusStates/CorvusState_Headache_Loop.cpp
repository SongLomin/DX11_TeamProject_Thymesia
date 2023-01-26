#include "stdafx.h"
#include "CorvusStates/CorvusState_Headache_Loop.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"

GAMECLASS_C(CCorvusState_Headache_Loop);
CLONE_C(CCorvusState_Headache_Loop, CComponent)

HRESULT CCorvusState_Headache_Loop::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_Headache_Loop::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_Headache_Loop::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_Headache1_Loop");
	//m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Headache_Loop::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_Headache_Loop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_Headache_Loop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_Headache_Loop::OnDisable()
{

}

void CCorvusState_Headache_Loop::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	
#endif
#endif

}

void CCorvusState_Headache_Loop::OnStateEnd()
{
	__super::OnStateEnd();

}

void CCorvusState_Headache_Loop::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Headache_End>();

}

void CCorvusState_Headache_Loop::Free()
{
	
}

void CCorvusState_Headache_Loop::OnDestroy()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Headache_Loop::Call_AnimationEnd, this, placeholders::_1);
}

_bool CCorvusState_Headache_Loop::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;


	return false;
}

