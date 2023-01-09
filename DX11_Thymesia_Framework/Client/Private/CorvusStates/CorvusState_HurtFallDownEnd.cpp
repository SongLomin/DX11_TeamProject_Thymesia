#include "stdafx.h"
#include "CorvusStates/CorvusState_HurtFallDownEnd.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "PhysXCollider.h"
#include "PhysXCharacterController.h"


GAMECLASS_C(CCorvusState_HurtFallDownEnd);
CLONE_C(CCorvusState_HurtFallDownEnd, CComponent)

HRESULT CCorvusState_HurtFallDownEnd::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_HurtFallDownEnd::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_HurtFallDownEnd::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_HurtFallDown_End");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_HurtFallDownEnd::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_HurtFallDownEnd::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_HurtFallDownEnd::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_HurtFallDownEnd::OnDisable()
{

}

void CCorvusState_HurtFallDownEnd::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	
#endif
#endif
	

}

void CCorvusState_HurtFallDownEnd::OnStateEnd()
{
	__super::OnStateEnd();

	
}

void CCorvusState_HurtFallDownEnd::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;


	m_pPhysXControllerCom.lock()->Enable_Gravity(true);

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_HurtFallDownEnd::OnDestroy()
{

	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_HurtFallDownEnd::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_HurtFallDownEnd::Free()
{
}

_bool CCorvusState_HurtFallDownEnd::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;


	return false;
}


