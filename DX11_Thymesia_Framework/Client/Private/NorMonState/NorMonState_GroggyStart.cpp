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
	}

	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_GroggyStart::Call_AnimationEnd, this);

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

	GET_SINGLE(CGameManager)->Register_Layer(OBJECT_LAYER::GROOGYMOSNTER, m_pOwner);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

	_float3 vPosition;
	XMStoreFloat3(&vPosition, m_pOwner.lock()->Get_Transform()->Get_Position() + XMVectorSet(0.f, 1.f, 0.f, 0.f));
	GAMEINSTANCE->Set_RadialBlur(0.3f, vPosition);
	GAMEINSTANCE->Set_Chromatic(0.1f);
	m_vShakingOffSet = { 0.f, 1.f, 0.f };
	GET_SINGLE(CGameManager)->Add_Shaking(XMLoadFloat3(&m_vShakingOffSet), 0.3f, 1.f, 9.f, 0.25f);

	Get_OwnerMonster()->Set_RimLightDesc(2.f, { 0.5f,1.f,0.9f }, 1.f);

#ifdef _DEBUG_COUT_
	cout << "NorMonState: Stop -> StopStop" << endl;
#endif
}

void CNorMonState_GroggyStart::OnStateEnd()
{
	__super::OnStateEnd();
}


void CNorMonState_GroggyStart::OnDestroy()
{
	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_GroggyStart::Call_AnimationEnd, this);
}


void CNorMonState_GroggyStart::Call_AnimationEnd()
{
	if (!Get_Enable())
		return;

	Get_OwnerCharacter().lock()->Change_State<CNorMonState_GroggyLoop>(0.05f);
}



_bool CNorMonState_GroggyStart::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;



	return false;
}

void CNorMonState_GroggyStart::Free()
{

}