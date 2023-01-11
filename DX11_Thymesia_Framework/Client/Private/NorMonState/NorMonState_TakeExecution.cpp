#include "stdafx.h"
#include "NorMonState/NorMonState_TakeExecution.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"
#include "Status_Monster.h"
#include "PhysXController.h"


GAMECLASS_C(CNorMonState_TakeExecution);
CLONE_C(CNorMonState_TakeExecution, CComponent)

HRESULT CNorMonState_TakeExecution::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_TakeExecution::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_TakeExecution::Start()
{
	__super::Start();

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|LArmor_VS_TakeExecution_02");
		break;
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|HArmorLV1_Halberds_VS_TakeExecution");
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|LArmor_VS_TakeExecution_01");
		break;
	case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|HArmorLV1_Halberds_VS_TakeExecution");
		break;
	}

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_TakeExecution::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_TakeExecution::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_TakeExecution::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();


}

void CNorMonState_TakeExecution::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	GET_SINGLE(CGameManager)->Remove_Layer(OBJECT_LAYER::GROOGYMOSNTER, m_pOwner);

	//Rotation_TargetToLookDir();

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		m_pPhysXControllerCom.lock()->Set_Enable(false);
		break;
	case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		m_pPhysXControllerCom.lock()->Set_Enable(false);
		break;
	default:
		break;
	}

	
	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	m_pOwner.lock()->Get_ComponentByType<CStatus_Monster>().lock()->Restart();


#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	cout << "NorMonState: Stop -> StopStop" << endl;
#endif
#endif


}

void CNorMonState_TakeExecution::OnStateEnd()
{
	__super::OnStateEnd();


}


void CNorMonState_TakeExecution::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_TakeExecution::Call_AnimationEnd, this, placeholders::_1);
}


void CNorMonState_TakeExecution::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}



_bool CNorMonState_TakeExecution::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_fAnimRatio() >= 0.5f)
	{
		m_pPhysXControllerCom.lock()->Set_Enable(true);
	}


	return false;
}

void CNorMonState_TakeExecution::Free()
{

}