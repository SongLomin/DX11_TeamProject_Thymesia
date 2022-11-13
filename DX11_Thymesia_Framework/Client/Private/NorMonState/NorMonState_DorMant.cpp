#include "stdafx.h"
#include "NorMonState/NorMonState_DorMant.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"


GAMECLASS_C(CNorMonState_DorMant);
CLONE_C(CNorMonState_DorMant, CComponent)

HRESULT CNorMonState_DorMant::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_DorMant::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_DorMant::Start()
{
	__super::Start();
	switch (m_eNorMonType)
	{
	case Client::NORMONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("DemoM02_Idle1");
		break;
	case Client::NORMONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("LV1Villager_F_Idle");
		break;
	case Client::NORMONSTERTYPE::SKULL:
		break;
	case Client::NORMONSTERTYPE::GARDENER:
		break;
	}



}

void CNorMonState_DorMant::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_DorMant::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CNorMonState_DorMant::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	cout << "LuxiyaState: RunStart -> OnStateStart" << endl;
#endif


}

void CNorMonState_DorMant::OnStateEnd()
{
	__super::OnStateEnd();


}


void CNorMonState_DorMant::Free()
{

}

_bool CNorMonState_DorMant::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	_float fDistance = Get_DistanceWithPlayer();





}

