#include "stdafx.h"
#include "CorvusStates/CorvusState_Die.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "UI_Landing.h"
#include "GameObject.h"


GAMECLASS_C(CCorvusState_Die);
CLONE_C(CCorvusState_Die, CComponent)

HRESULT CCorvusState_Die::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_Die::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_Die::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Corvus.ao|Corvus_SD_Dead_1");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Die::Call_AnimationEnd, this);
}

void CCorvusState_Die::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_Die::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_Die::OnDisable()
{

}

void CCorvusState_Die::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);
	


	weak_ptr<CUI_Landing> pLanding = GAMEINSTANCE->Get_GameObjects<CUI_Landing>(LEVEL_STATIC).front();

	pLanding.lock()->Call_Landing(CUI_Landing::LANDING_DEAD);


#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif
#endif


}

void CCorvusState_Die::OnStateEnd()
{
	__super::OnStateEnd();


}

void CCorvusState_Die::OnHit(weak_ptr<CCollider> pMyCollider, weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	// Do Nothing
}

void CCorvusState_Die::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_Die::Free()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Die::Call_AnimationEnd, this);
}

_bool CCorvusState_Die::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;


	return false;
}


