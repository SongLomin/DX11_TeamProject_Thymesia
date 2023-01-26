#include "stdafx.h"
#include "CorvusStates/CorvusState_HurtFallDown.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "PhysXController.h"

GAMECLASS_C(CCorvusState_HurtFallDown);
CLONE_C(CCorvusState_HurtFallDown, CComponent)

HRESULT CCorvusState_HurtFallDown::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_HurtFallDown::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_HurtFallDown::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_HurtFallDown");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_HurtFallDown::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_HurtFallDown::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_HurtFallDown::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_HurtFallDown::OnDisable()
{

}

void CCorvusState_HurtFallDown::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pPhysXControllerCom.lock()->Enable_Gravity(false);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);


#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	
#endif
#endif
	

}

void CCorvusState_HurtFallDown::OnStateEnd()
{
	__super::OnStateEnd();

	
}

void CCorvusState_HurtFallDown::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_HurtFallDownEnd>();

}

void CCorvusState_HurtFallDown::OnDestroy()
{

	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_HurtFallDown::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_HurtFallDown::Free()
{
}

_bool CCorvusState_HurtFallDown::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;


	return false;
}


