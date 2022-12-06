#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_Magician.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "Player.h"
#include "CorvusStates/CorvusStates.h"
#include "GameManager.h"
#include "Monster.h"
#include "NorMonStates.h"

GAMECLASS_C(CCorvusState_PS_Magician);
CLONE_C(CCorvusState_PS_Magician, CComponent)

HRESULT CCorvusState_PS_Magician::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS_Magician::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_PS_Magician::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_PW_Magician_A");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_Magician::Call_AnimationEnd, this);
}

void CCorvusState_PS_Magician::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_PS_Magician::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_PS_Magician::OnDisable()
{

}

void CCorvusState_PS_Magician::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif
#endif

}

void CCorvusState_PS_Magician::OnStateEnd()
{
	__super::OnStateEnd();

}

void CCorvusState_PS_Magician::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}



void CCorvusState_PS_Magician::OnEventMessage(weak_ptr<CBase> pArg)
{

}

void CCorvusState_PS_Magician::Free()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_Magician::Call_AnimationEnd, this);
}

_bool CCorvusState_PS_Magician::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	return false;
}

