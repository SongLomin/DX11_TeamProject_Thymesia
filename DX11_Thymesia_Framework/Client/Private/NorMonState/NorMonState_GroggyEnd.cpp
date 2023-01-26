#include "stdafx.h"
#include "NorMonState/NorMonState_GroggyEnd.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"
#include "Status_Monster.h"


GAMECLASS_C(CNorMonState_GroggyEnd);
CLONE_C(CNorMonState_GroggyEnd, CComponent)

HRESULT CNorMonState_GroggyEnd::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_GroggyEnd::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CNorMonState_GroggyEnd::Start()
{
	__super::Start();

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_HurtStunEnd|BaseLayer|");
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV2Villager01_F_HurtStunEnd");
		break;
	case Client::MONSTERTYPE::SKULL:
		break;
	case Client::MONSTERTYPE::GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_HurtStunEnd");
		break;
	case Client::MONSTERTYPE::ENHANCE_GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_HurtStunEnd");
		break;
	case Client::MONSTERTYPE::SHIELDAXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_HurtStunEnd|BaseLayer|");
		break;
	case Client::MONSTERTYPE::SKULLSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|LArmor_Shield_HurtStunEnd");
		break;
	case Client::MONSTERTYPE::SKULLSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|HArmorLV1_Halberds_HurtStunEnd");
		break;
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|LArmor_Shield_HurtStunEnd");
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|LArmor_Shield_HurtStunEnd");
		break;
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|HArmorLV1_Halberds_HurtStunEnd");
		break;
	case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|HArmorLV1_Halberds_HurtStunEnd");
		break;
	}

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_GroggyEnd::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_GroggyEnd::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_GroggyEnd::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();


}

void CNorMonState_GroggyEnd::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		GET_SINGLE(CGameManager)->Remove_Layer(OBJECT_LAYER::GROOGYMOSNTER, m_pOwner);
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		GET_SINGLE(CGameManager)->Remove_Layer(OBJECT_LAYER::GROOGYMOSNTER, m_pOwner);
		break;
	case Client::MONSTERTYPE::GARDENER:
		GET_SINGLE(CGameManager)->Remove_Layer(OBJECT_LAYER::GROOGYMOSNTER, m_pOwner);
		break;
	case Client::MONSTERTYPE::SHIELDAXEMAN:
		GET_SINGLE(CGameManager)->Remove_Layer(OBJECT_LAYER::GROOGYMOSNTER, m_pOwner);
		break;
	case Client::MONSTERTYPE::ENHANCE_GARDENER:
		GET_SINGLE(CGameManager)->Remove_Layer(OBJECT_LAYER::GROOGYMOSNTER, m_pOwner);
		break;
	case Client::MONSTERTYPE::SKULLSHIELDMAN:
		GET_SINGLE(CGameManager)->Remove_Layer(OBJECT_LAYER::GROOGYMOSNTER, m_pOwner);
		break;
	case Client::MONSTERTYPE::SKULLSPEARMAN:
		GET_SINGLE(CGameManager)->Remove_Layer(OBJECT_LAYER::GROOGYMOSNTER, m_pOwner);
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

void CNorMonState_GroggyEnd::OnStateEnd()
{
	__super::OnStateEnd();


}


void CNorMonState_GroggyEnd::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_GroggyEnd::Call_AnimationEnd, this, placeholders::_1);
}


void CNorMonState_GroggyEnd::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
}



_bool CNorMonState_GroggyEnd::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

void CNorMonState_GroggyEnd::Free()
{

}