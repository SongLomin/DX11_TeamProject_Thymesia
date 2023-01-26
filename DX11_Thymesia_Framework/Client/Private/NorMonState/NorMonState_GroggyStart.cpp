#include "stdafx.h"
#include "NorMonState/NorMonState_GroggyStart.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"
#include "Monster.h"
#include "PhysXController.h"


GAMECLASS_C(CNorMonState_GroggyStart);
CLONE_C(CNorMonState_GroggyStart, CComponent)

HRESULT CNorMonState_GroggyStart::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_GroggyStart::Initialize(void* pArg)
{
	__super::Initialize(pArg);
	m_StateFlag |= (_uint)STATE_FLAG::EXECUTABLE;

	return S_OK;
}

void CNorMonState_GroggyStart::Start()
{
	__super::Start();
	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_HurtStunStart|BaseLaye");
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV2Villager01_F_HurtStunStart");
		break;
	case Client::MONSTERTYPE::SKULL:
		break;
	case Client::MONSTERTYPE::GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_HurtStunStart");
		break;
	case Client::MONSTERTYPE::ENHANCE_GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_HurtStunStart");
		break;
	case Client::MONSTERTYPE::SHIELDAXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_HurtStunStart|BaseLaye");
		break;
	case Client::MONSTERTYPE::SKULLSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|LArmor_Shield_HurtStunStart");
		break;
	case Client::MONSTERTYPE::SKULLSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|HArmorLV1_Halberds_HurtStunStart");
		break;
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|LArmor_Shield_HurtStunStart");
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|LArmor_Shield_HurtStunStart");
		break;
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|HArmorLV1_Halberds_HurtStunStart");
		break;
	case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|HArmorLV1_Halberds_HurtStunStart");
		break;
	}

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_GroggyStart::Call_AnimationEnd, this, placeholders::_1);

}

void CNorMonState_GroggyStart::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//Turn_Transform(fTimeDelta);
	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CNorMonState_GroggyStart::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();


}



void CNorMonState_GroggyStart::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::GROOGYMOSNTER, m_pOwner);
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::GROOGYMOSNTER, m_pOwner);
		break;
	case Client::MONSTERTYPE::GARDENER:
		GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::GROOGYMOSNTER, m_pOwner);
		break;
	case Client::MONSTERTYPE::SHIELDAXEMAN:
		GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::GROOGYMOSNTER, m_pOwner);
		break;
	case Client::MONSTERTYPE::ENHANCE_GARDENER:
		GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::GROOGYMOSNTER, m_pOwner);
		break;
	case Client::MONSTERTYPE::SKULLSHIELDMAN:
		GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::GROOGYMOSNTER, m_pOwner);
		break;
	case Client::MONSTERTYPE::SKULLSPEARMAN:
		GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::GROOGYMOSNTER, m_pOwner);
		break;
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::GROOGYMOSNTER, m_pOwner);
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::GROOGYMOSNTER, m_pOwner);
		break;


	}



	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	_float3 vPosition;
	XMStoreFloat3(&vPosition, m_pOwner.lock()->Get_Transform()->Get_Position() + XMVectorSet(0.f, 1.f, 0.f, 0.f));
	GAMEINSTANCE->Set_RadialBlur(0.3f, vPosition);
	GAMEINSTANCE->Set_Chromatic(0.1f);
	m_vShakingOffSet = { 0.f, 1.f, 0.f };
	GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.3f, 1.f, 9.f, 0.25f);

	Get_OwnerMonster()->Set_RimLightDesc(21.f, { 0.5f,1.f,0.9f }, 1.f);

#ifdef _DEBUG_COUT_
	cout << "NorMonState: Stop -> StopStop" << endl;
#endif
}

void CNorMonState_GroggyStart::OnStateEnd()
{

	m_bNextStatepause = false;
	__super::OnStateEnd();
}


void CNorMonState_GroggyStart::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);

	if (!Get_Enable())
		return;

	if ((_uint)EVENT_TYPE::ON_ARMOREXECUTIONSTART == iArg)
	{
		m_bNextStatepause = true;
		m_iCurrentKeyIndex = m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex();
		m_iCurrentKeyIndex += 1;
		//Get_OwnerCharacter().lock()->Change_State<CNorMonState_TakeExecution>(0.05f);
	}

}

void CNorMonState_GroggyStart::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_GroggyStart::Call_AnimationEnd, this, placeholders::_1);
}


void CNorMonState_GroggyStart::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CNorMonState_GroggyLoop>(0.05f);
}



_bool CNorMonState_GroggyStart::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	
	if (m_bNextStatepause)
	{
		if (m_pModelCom.lock()->Get_CurrentAnimation().lock()->Get_CurrentChannelKeyIndex() > m_iCurrentKeyIndex)
		{
			Get_OwnerCharacter().lock()->Change_State<CNorMonState_TakeExecution>(0.05f);
			return true;
		}
	}


	return false;
}

void CNorMonState_GroggyStart::Free()
{

}