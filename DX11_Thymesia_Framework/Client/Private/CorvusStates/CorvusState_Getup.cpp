#include "stdafx.h"
#include "CorvusStates/CorvusState_Getup.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"

GAMECLASS_C(CCorvusState_Getup);
CLONE_C(CCorvusState_Getup, CComponent)

HRESULT CCorvusState_Getup::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_Getup::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_Getup::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_GetUp");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Getup::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_Getup::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_Getup::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_Getup::OnDisable()
{

}

void CCorvusState_Getup::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	
#endif
#endif

	
}

void CCorvusState_Getup::OnStateEnd()
{
	__super::OnStateEnd();

	
}

void CCorvusState_Getup::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_Getup::Free()
{
	
}

void CCorvusState_Getup::OnDestroy()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Getup::Call_AnimationEnd, this, placeholders::_1);
}

_bool CCorvusState_Getup::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;




	return false;
}

