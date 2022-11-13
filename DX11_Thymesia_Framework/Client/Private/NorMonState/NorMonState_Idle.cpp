#include "stdafx.h"
#include "NorMonState/NorMonState_Idle.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"


GAMECLASS_C(CNorMonState_Idle);
CLONE_C(CNorMonState_Idle, CComponent)

HRESULT CNorMonState_Idle::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_Idle::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_Idle::Start()
{
	__super::Start();

	if (m_eNorMonType == NORMONSTERTYPE::AXEMAN)
	{
		switch (m_eNorMonIdleType)
		{
		case Client::NORMONSTERIDLETYPE::NORIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("DemoM02_Idle1");
			break;
		case Client::NORMONSTERIDLETYPE::SITIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("LV1Villager_M_Sit_Idle");
			break;
		case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("LV1Villager_M_SP_Idle1");
			break;
		}
	}

	if (m_eNorMonType == NORMONSTERTYPE::KNIFEWOMAN)
	{
		switch (m_eNorMonIdleType)
		{
		case Client::NORMONSTERIDLETYPE::NORIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("LV1Villager_F_Idle");
			break;
		case Client::NORMONSTERIDLETYPE::SITIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("DemoM02_Idle1");
			break;
		case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("DemoM02_Idle1");
			break;
		}
	}

	
	
	

}

void CNorMonState_Idle::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_Idle::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CNorMonState_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	cout << "LuxiyaState: RunStart -> OnStateStart" << endl;
#endif


}

void CNorMonState_Idle::OnStateEnd()
{
	__super::OnStateEnd();


}


void CNorMonState_Idle::Free()
{

}

_bool CNorMonState_Idle::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	//_float fDistance = Get_DistanceWithPlayer();




	
}

