#include "stdafx.h"
#include "CorvusStates/CorvusState_FeatherAttack.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"

GAMECLASS_C(CCorvusState_FeatherAttack);
CLONE_C(CCorvusState_FeatherAttack, CComponent)

HRESULT CCorvusState_FeatherAttack::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_FeatherAttack::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_FeatherAttack::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_SD_TD2Thorw2");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_FeatherAttack::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_FeatherAttack::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_FeatherAttack::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_FeatherAttack::OnDisable()
{

}

void CCorvusState_FeatherAttack::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	
#endif
#endif
	m_pModelCom.lock()->Set_AnimationSpeed(0.5f);
	
}

void CCorvusState_FeatherAttack::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}

void CCorvusState_FeatherAttack::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_FeatherAttack::Free()
{
	
}

void CCorvusState_FeatherAttack::OnDestroy()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_FeatherAttack::Call_AnimationEnd, this, placeholders::_1);
}

_bool CCorvusState_FeatherAttack::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;




	return false;
}

