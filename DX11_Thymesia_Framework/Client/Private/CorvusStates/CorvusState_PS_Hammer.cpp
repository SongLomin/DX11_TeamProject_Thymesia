#include "stdafx.h"
#include "CorvusStates/CorvusState_PS_Hammer.h"
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

GAMECLASS_C(CCorvusState_PS_Hammer);
CLONE_C(CCorvusState_PS_Hammer, CComponent)

HRESULT CCorvusState_PS_Hammer::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_PS_Hammer::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_PS_Hammer::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_PW_Hammer_B_Ver2");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_PS_Hammer::Call_AnimationEnd, this);
}

void CCorvusState_PS_Hammer::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_PS_Hammer::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_PS_Hammer::OnDisable()
{

}

void CCorvusState_PS_Hammer::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: RunStart -> OnStateStart" << endl;
#endif
#endif

}

void CCorvusState_PS_Hammer::OnStateEnd()
{
	__super::OnStateEnd();

}

void CCorvusState_PS_Hammer::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}



void CCorvusState_PS_Hammer::OnEventMessage(weak_ptr<CBase> pArg)
{

}

void CCorvusState_PS_Hammer::Free()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_PS_Hammer::Call_AnimationEnd, this);
}

_bool CCorvusState_PS_Hammer::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	return false;
}

