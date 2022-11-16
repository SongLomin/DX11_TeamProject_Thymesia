#include "stdafx.h"
#include "NorMonState/NorMonState_TurnL90.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateBase.h"
#include "Character.h"


GAMECLASS_C(CNorMonState_TurnL90);
CLONE_C(CNorMonState_TurnL90, CComponent)

HRESULT CNorMonState_TurnL90::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_TurnL90::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_TurnL90::Start()
{
	__super::Start();

	if (m_eNorMonType == NORMONSTERTYPE::AXEMAN)
	{
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|DemoM02_TurnL90|BaseLayer");
	}

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_TurnL90::Call_AnimationEnd, this);
}

void CNorMonState_TurnL90::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_TurnL90::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Turn_ToThePlayer(fTimeDelta);

	Check_AndChangeNextState();
}




void CNorMonState_TurnL90::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	cout << "LuxiyaState: RunStart -> OnStateStart" << endl;
#endif


}

void CNorMonState_TurnL90::OnStateEnd()
{
	__super::OnStateEnd();

	
}


void CNorMonState_TurnL90::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_TurnL90::Call_AnimationEnd, this);
}

void CNorMonState_TurnL90::Free()
{

}

_bool CNorMonState_TurnL90::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	return false;

}

void CNorMonState_TurnL90::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;
	
	Get_Owner().lock()->Get_Component<CNorMonState_Idle>().lock()->Set_MonIdleType(NORMONSTERIDLETYPE::NORIDLE);
	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}



