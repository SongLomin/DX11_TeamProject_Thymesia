#include "stdafx.h"
#include "NorMonState/NorMonState_Petrol.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"
#include "PhysXController.h"



GAMECLASS_C(CNorMonState_Petrol);
CLONE_C(CNorMonState_Petrol, CComponent)



HRESULT CNorMonState_Petrol::Initialize_Prototype()
{
	__super::Initialize_Prototype();
	return S_OK;
}

HRESULT CNorMonState_Petrol::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	ZeroMemory(&m_fStartPosition, sizeof(_float4));

	return S_OK;
}

void CNorMonState_Petrol::Start()
{
	__super::Start();

	m_vecPatrolPosition.resize(3, {});

	XMStoreFloat3(&m_vecPatrolPosition[0], XMLoadFloat4(&m_fStartPosition) + XMVectorSet(4.f, 0.f, 0.1f, 0.f));
	XMStoreFloat3(&m_vecPatrolPosition[1], XMLoadFloat4(&m_fStartPosition) + XMVectorSet(8.f, 0.f, 0.f, 0.f));
	XMStoreFloat3(&m_vecPatrolPosition[2], XMLoadFloat4(&m_fStartPosition) + XMVectorSet(-4.f, 0.f, 0.2f, 0.f));

	switch (m_eMonType)
	{
	case Client::MONSTERTYPE::AXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_WalkF|BaseLayer|Armatu");
		break;
	case Client::MONSTERTYPE::KNIFEWOMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_WalkF");
		break;
	case Client::MONSTERTYPE::SKULL:
		break;
	case Client::MONSTERTYPE::GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_WalkF");
		break;
	case Client::MONSTERTYPE::ENHANCE_GARDENER:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_Gardener01_Base01.ao|Gardener_WalkF");
		break;
	case Client::MONSTERTYPE::SHIELDAXEMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_WalkF|BaseLayer|Armatu");
		break;
	case Client::MONSTERTYPE::SKULLSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|LArmor_Shield_WalkF");
		break;
	case Client::MONSTERTYPE::SKULLSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV0_02.ao|HArmorLV1_Halberds_WalkF");
		break;
	case Client::MONSTERTYPE::ARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|LArmor_Shield_WalkF");
		break;
	case Client::MONSTERTYPE::WEAKARMORSHIELDMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|LArmor_Shield_WalkF");
		break;
	case Client::MONSTERTYPE::ARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LArmorLV1_01.ao|HArmorLV1_Halberds_WalkF");
		break;
	case Client::MONSTERTYPE::WEAKARMORSPEARMAN:
		m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_HArmorTypeLV1_01.ao|HArmorLV1_Halberds_WalkF");
		break;

	}

	//m_pModelCom.lock()->CallBack_AnimationEnd += bind(&CNorMonState_Petrol::Call_AnimationEnd, this, placeholders::_1);
}

void CNorMonState_Petrol::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_bTurnCheck)
	{
		switch (m_iCorssResult)
		{
		case 1:
		{
			_float fTurnRifgtValue = 1.57f / 1.333f;
			m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * fTurnRifgtValue * 1.f);
		}
			break;
		case -1:
		{
			_float fTurnLeftValue = 1.57f / 1.333f;
			m_pTransformCom.lock()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * fTurnLeftValue * -1.f);
			
		}
		break;
		}

	}
	if (!m_bTurnCheck)
	{
		m_fCurrentSpeed += m_fAccel * fTimeDelta;
		m_fCurrentSpeed = min(m_fMaxSpeed, m_fCurrentSpeed);
		PxControllerFilters Filters;
		m_pPhysXControllerCom.lock()->MoveWithRotation({ 0.f, 0.f, m_fCurrentSpeed * fTimeDelta }, 0.f, fTimeDelta, Filters, nullptr, m_pTransformCom);
	}



	m_pModelCom.lock()->Play_Animation(fTimeDelta);


}

void CNorMonState_Petrol::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	Check_AndChangeNextState();
}



void CNorMonState_Petrol::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: RunStart -> Walk_F" << endl;
#endif
#endif

	m_pModelCom.lock()->Set_AnimationSpeed(1.5f);

	
}

void CNorMonState_Petrol::OnStateEnd()
{
	__super::OnStateEnd();

	m_pModelCom.lock()->Set_AnimationSpeed(1.f);
}



void CNorMonState_Petrol::OnEventMessage(_uint iArg)
{
	__super::OnEventMessage(iArg);
	if ((_uint)EVENT_TYPE::ON_RESET_OBJ == iArg)
	{
		m_iPatrolCount = 0;
		m_iCorssResult = 0;
	}


}

_float CNorMonState_Petrol::Cul_DotResult()
{
	_vector vCurMonsterPos = m_pTransformCom.lock()->Get_Position();
	vCurMonsterPos.m128_f32[1] = 0.f;
	_vector vGoalPos = XMLoadFloat3(&m_vecPatrolPosition[m_iPatrolCount]);
	vGoalPos.m128_f32[1] = 0.f;
	_vector vMonsterToGoalDirectionVector = XMVector3Normalize(vGoalPos - vCurMonsterPos);
	_vector vMyLookVector = m_pTransformCom.lock()->Get_State(CTransform::STATE_LOOK);
	vMyLookVector.m128_f32[1] = 0.f;
	vMyLookVector = XMVector3Normalize(vMyLookVector);

	_float fDot = XMVectorGetY((XMVector3Dot(vMonsterToGoalDirectionVector, vMyLookVector)));

	return fDot;
}

//void CNorMonState_Petrol::Call_AnimationEnd(_uint iEndAnimIndex)
//{
//	if (!Get_Enable())
//		return;
//
//	
//	Get_OwnerCharacter().lock()->Change_State<CNorMonState_Petrol>(0.1f);
//}
//
//void CNorMonState_Petrol::OnDestroy()
//{
//	m_pModelCom.lock()->CallBack_AnimationEnd -= bind(&CNorMonState_Petrol::Call_AnimationEnd, this, placeholders::_1);
//}

void CNorMonState_Petrol::Free()
{

}

_bool CNorMonState_Petrol::Check_AndChangeNextState()
{

	if (!Check_Requirement())
		return false;

	_float fDistance = Get_DistanceWithPlayer();
	_float4 MonsterPosion;
	XMStoreFloat4(&MonsterPosion, Get_OwnerMonster()->Get_WorldPosition());
	_vector vMonsterPos = XMLoadFloat4(&MonsterPosion);
	_vector vOtherPos = XMLoadFloat3(&m_vecPatrolPosition[m_iPatrolCount]);

	_float fMonDistance = XMVector3Length(vMonsterPos - vOtherPos).m128_f32[0];

	if (fDistance <= 3.f)
	{
		Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_RunCheck(true);
		Get_Owner().lock()->Get_Component<CNorMonState_Idle>().lock()->Set_CloseToRun(true);
		Get_Owner().lock()->Get_Component<CNorMonState_Idle>().lock()->Set_IdleType(1);
		Get_Owner().lock()->Get_Component<CNorMonState_Idle>().lock()->Set_ChanegePatrol(false);
		Get_OwnerCharacter().lock()->Change_State<CNorMonState_Idle>(0.05f);
		return true;
	}

	

	if (m_iPatrolCount < 2)
	{
		if (!m_bTurnCheck)
			Get_OwnerMonster()->Get_Transform()->LookAt2D(vOtherPos);

		if (fMonDistance <= 0.05f + DBL_EPSILON)
		{
			++m_iPatrolCount;
			m_bTurnCheck = true;
			m_iCorssResult = Compute_DirectionToOtherPosition(m_vecPatrolPosition[m_iPatrolCount]);
			int i = 0;
			//여서 이게아니고 왼쪽오른쪽판단이먼저임 판단하고 가야됨
		}
	}
	else
	{
		m_iPatrolCount = 0;
		m_iCorssResult = Compute_DirectionToOtherPosition(m_vecPatrolPosition[m_iPatrolCount]);
	}


	if (Cul_DotResult() > 0.94f && m_bTurnCheck)
	{

		if (m_iPatrolCount >= m_vecPatrolPosition.size())  
			return false;

		Get_OwnerMonster()->Get_Transform()->LookAt2D(XMLoadFloat3(&m_vecPatrolPosition[m_iPatrolCount]));
		m_bTurnCheck = false;
	}


	
	//현재위치에서 빼주고 그 디스턴스 구해서 그디스턴스가 엡실론보다작아지면 다음거 턴매커니즘하고 다시것고 0 -> 1 -> 2 가고 다시 0반복 

	


	return false;
}

