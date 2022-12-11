#include "stdafx.h"
#include "NorMonState/NorMonState_GroggyLoop.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"

GAMECLASS_C(CNorMonState_GroggyLoop);
CLONE_C(CNorMonState_GroggyLoop, CComponent)

HRESULT CNorMonState_GroggyLoop::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_GroggyLoop::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	m_StateFlag |= (_uint)STATE_FLAG::EXECUTABLE;

	return S_OK;
}

void CNorMonState_GroggyLoop::Start()
{
	__super::Start();

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_HurtStunLoop|BaseLayer");
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV2Villager01_F_HurtStunLoop");
		break;
	case Client::MONSTERTYPE::SKULL:
		break;
	case Client::MONSTERTYPE::GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_HurtStunLoop");
		break;
	case Client::MONSTERTYPE::ELITEGARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_HurtStunLoop");
		break;
	case Client::MONSTERTYPE::SHIELDAXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_HurtStunLoop|BaseLayer");
		break;
	}



	m_fLoopEndTime = 0.f;

}

void CNorMonState_GroggyLoop::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fLoopEndTime += fTimeDelta;

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_GroggyLoop::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();


}



void CNorMonState_GroggyLoop::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: Stop -> StopStop" << endl;
#endif
#endif


}

void CNorMonState_GroggyLoop::OnStateEnd()
{
	__super::OnStateEnd();
	
}





_bool CNorMonState_GroggyLoop::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;


	if (m_fLoopEndTime >= 7.f)
	{
		Get_OwnerCharacter().lock()->Change_State<CNorMonState_GroggyEnd>(0.05f);
		m_fLoopEndTime = 0.f; 
		return true;
	}


	return false;
}

void CNorMonState_GroggyLoop::Free()
{

}