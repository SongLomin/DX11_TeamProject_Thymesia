#include "stdafx.h"
#include "CorvusStates/CorvusState_ShortHealing.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "Status_Player.h"

GAMECLASS_C(CCorvusState_ShortHealing);
CLONE_C(CCorvusState_ShortHealing, CComponent)

HRESULT CCorvusState_ShortHealing::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_ShortHealing::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_ShortHealing::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_Healing_Little");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_ShortHealing::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_ShortHealing::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_ShortHealing::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_ShortHealing::OnDisable()
{

}

void CCorvusState_ShortHealing::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG_COUT_
	
#endif // _DEBUG_COUT_
	
	Weak_StaticCast<CStatus_Player>(m_pStatusCom).lock()->Use_Potion();
	//Weak_StaticCast<CStatus_Player>(m_pStatusCom).lock()->Heal_Player(300.f);
}

void CCorvusState_ShortHealing::OnStateEnd()
{
	__super::OnStateEnd();
	

}

void CCorvusState_ShortHealing::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_ShortHealing::Free()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_ShortHealing::Call_AnimationEnd, this, placeholders::_1);
}

_bool CCorvusState_ShortHealing::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	return false;
}


