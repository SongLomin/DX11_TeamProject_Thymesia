#include "stdafx.h"
#include "CorvusStates/CorvusState_CheckPointLoop.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"

GAMECLASS_C(CCorvusState_CheckPointLoop);
CLONE_C(CCorvusState_CheckPointLoop, CComponent)

HRESULT CCorvusState_CheckPointLoop::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_CheckPointLoop::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_CheckPointLoop::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_ArchiveSitLoop");

}

void CCorvusState_CheckPointLoop::Tick(_float fTimeDelta)
{
	CPlayerStateBase::Tick(fTimeDelta);

	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_CheckPointLoop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_CheckPointLoop::OnDisable()
{

}

void CCorvusState_CheckPointLoop::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	
#endif
#endif

}

void CCorvusState_CheckPointLoop::OnStateEnd()
{
	__super::OnStateEnd();

}



void CCorvusState_CheckPointLoop::Free()
{
	
}

_bool CCorvusState_CheckPointLoop::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	return false;
}

