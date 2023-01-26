#include "stdafx.h"
#include "CorvusStates/StateExecution/CorvusState_Armor_Execution_01.h"
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

GAMECLASS_C(CCorvusState_Armor_Execution_01);
CLONE_C(CCorvusState_Armor_Execution_01, CComponent)

HRESULT CCorvusState_Armor_Execution_01::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CCorvusState_Armor_Execution_01::Initialize(void* pArg)
{
	__super::Initialize(pArg);


	return S_OK;
}

void CCorvusState_Armor_Execution_01::Start()
{
	__super::Start();
	m_pModelCom = m_pOwner.lock()->Get_Component<CModel>();
	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Corvus_VSLV1Villager_M_Execution");
	m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CCorvusState_Armor_Execution_01::Call_AnimationEnd, this, placeholders::_1);
}

void CCorvusState_Armor_Execution_01::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


	m_pModelCom.lock()->Play_Animation(fTimeDelta);
}

void CCorvusState_Armor_Execution_01::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}

void CCorvusState_Armor_Execution_01::OnDisable()
{

}

void CCorvusState_Armor_Execution_01::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);


	//m_ThisStateAnimationCom = m_pModelCom.lock()->Get_CurrentAnimation();
	//m_ThisStateAnimationCom.lock()->CallBack_NextChannelKey += bind(&CCorvusState_Armor_Execution_01::Call_NextAnimationKey, this, placeholders::_1);


	//GET_SINGLE(CGameManager)->Start_Cinematic(m_pModelCom, "camera",XMMatrixIdentity());
#ifdef _DEBUG
#ifdef _DEBUG_COUT_
	
#endif
#endif

}

void CCorvusState_Armor_Execution_01::OnStateEnd()
{
	__super::OnStateEnd();

}

void CCorvusState_Armor_Execution_01::Call_AnimationEnd(_uint iEndAnimIndex)
{
	if (!Get_Enable())
		return;

	Get_OwnerPlayer()->Change_State<CCorvusState_Idle>();

}

void CCorvusState_Armor_Execution_01::Call_NextAnimationKey(const _uint& In_iKeyIndex)
{
	if (!Get_Enable())
		return;

	switch (In_iKeyIndex)
	{
	case 3:
		_float3 vPosition;
		XMStoreFloat3(&vPosition, m_pOwner.lock()->Get_Transform()->Get_Position() + XMVectorSet(0.f, 1.f, 0.f, 0.f));
		GAMEINSTANCE->Set_RadialBlur(0.2f, vPosition);
		GAMEINSTANCE->Set_Chromatic(0.1f);
		
		break;
	}
}

void CCorvusState_Armor_Execution_01::OnEventMessage(weak_ptr<CBase> pArg)
{

}

void CCorvusState_Armor_Execution_01::Free()
{
	
}

void CCorvusState_Armor_Execution_01::OnDestroy()
{
	if (m_pModelCom.lock())
		m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CCorvusState_Armor_Execution_01::Call_AnimationEnd, this, placeholders::_1);
}

_bool CCorvusState_Armor_Execution_01::Check_AndChangeNextState()
{
	if (!Check_Requirement())
		return false;

	//if (Check_RequirementUltimateState())
	//{
	//	Rotation_NearToLookDir();
	//	Get_OwnerPlayer()->Change_State<CNorMonState_UltimateSkill>();
	//	return true;
	//}
	//
	//if (Check_RequirementAttackState())
	//{
	//	if (!Rotation_InputToLookDir())
	//		Rotation_NearToLookDir();
	//
	//	Get_OwnerPlayer()->Change_State<CNorMonState_Attack>();
	//	Get_OwnerPlayer()->Get_Component<CNorMonState_Attack>().lock()->Play_AttackWithIndex(0);
	//	return true;
	//}




	return false;
}

