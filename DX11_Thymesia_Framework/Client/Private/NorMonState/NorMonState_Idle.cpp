#include "stdafx.h"
#include "NorMonState/NorMonState_Idle.h"
#include "Model.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Player.h"
//#include "BehaviorBase.h"
#include "Animation.h"
#include "AIStateBase.h"
#include "NorMonStateS.h"
#include "Character.h"
#include "Collider.h"
#include "Attack_Area.h"



GAMECLASS_C(CNorMonState_Idle);
CLONE_C(CNorMonState_Idle, CComponent)

void CNorMonState_Idle::Set_MonIdleType(NORMONSTERIDLETYPE IDLETYPE)
{
	m_eNorMonIdleType = IDLETYPE;
}

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
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|DemoM02_Idle1|BaseLayer|Armature|Arm");
			break;
		case Client::NORMONSTERIDLETYPE::SITIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_Sit_Idle|BaseLayer|Arm");
			break;
		case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_SP_Idle1|BaseLayer|Arm");
			break;
		}
	}

	if (m_eNorMonType == NORMONSTERTYPE::KNIFEWOMAN)
	{
		switch (m_eNorMonIdleType)
		{
		case Client::NORMONSTERIDLETYPE::NORIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_Idle");
			break;
		case Client::NORMONSTERIDLETYPE::SITIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_Sit_Idle");
			break;
		case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
			m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("SK_C_LV0Villager_F.ao|LV1Villager_F_SP_Idle1");
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
	
	
	//TODO �����ȵſ� ȭ����
	//if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CNorMonState_Awake>().lock())
	//{
	//	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_IdleGeneral|BaseLayer");
	//}
	//
	//if (Get_OwnerCharacter().lock()->Get_PreState().lock() == Get_Owner().lock()->Get_Component<CNorMonState_SitToIdle>().lock())
	//{
	//	m_iAnimIndex = m_pModelCom.lock()->Get_IndexFromAnimName("Armature|Armature|Armature|Armature|LV1Villager_M_IdleGeneral|BaseLayer");
	//}

	Check_AndChangeNextState();

}

void CNorMonState_Idle::OnHit(weak_ptr<CCollider> pOtherCollider, const HIT_TYPE& In_eHitType, const _float& In_fDamage)
{
	//__super::OnHit(pOtherCollider, In_eHitType, In_fDamage);

	if (pOtherCollider.lock()->Get_CollisionLayer() == (_uint)COLLISION_LAYER::PLAYER_ATTACK)
	{
		_vector vMyPosition = m_pTransformCom.lock()->Get_State(CTransform::STATE_TRANSLATION);

		//�¾����� �÷��̾ �ٶ󺸴� �ü� ó��
		weak_ptr<CAttackArea> pAttackArea = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner());

		if (!pAttackArea.lock())
			return;

		_vector vOtherColliderPosition = Weak_Cast<CAttackArea>(pOtherCollider.lock()->Get_Owner()).lock()->
			Get_ParentObject().lock()->
			Get_Component<CTransform>().lock()->
			Get_State(CTransform::STATE_TRANSLATION);

		/*_vector vOtherColliderPosition = Weak_Cast<CWeapon>(pOtherCollider.lock()->Get_Owner()).lock()->
			Get_ParentObject().lock()->
			Get_Component<CTransform>().lock()->
			Get_State(CTransform::STATE_TRANSLATION);*/

		_vector vSameHeightOtherColliderPosition = vOtherColliderPosition;
		vSameHeightOtherColliderPosition.m128_f32[1] = vMyPosition.m128_f32[1];

		m_pTransformCom.lock()->LookAt(vSameHeightOtherColliderPosition);

		//bool bRandom = (_bool)(rand() % 2);

		//������ ����
		//m_pStatusCom.lock()->Add_Damage(In_fDamage);
		//GAMEINSTANCE->Get_GameObjects<CDamageUI>(LEVEL::LEVEL_STATIC).front().lock()->Add_DamageText(vMyPosition, In_fDamage, bRandom);

		//GAMEINSTANCE->Get_GameObjects<CMonsterHpBar>(LEVEL::LEVEL_STATIC).front().lock()->OnHit(m_pOwner);
		//GAMEINSTANCE->Get_GameObjects<CComboTimer>(LEVEL::LEVEL_STATIC).front().lock()->Update_Combo();

		GET_SINGLE(CGameManager)->Get_CurrentPlayer().lock()->Set_TargetMonster(Get_OwnerMonster());

		//Play_OnHitEffect();

		//���� ���¿� ���� �ִϸ��̼� ����

		//if (m_pStatusCom.lock()->Is_Dead())
		//{
		//	Get_OwnerMonster()->Change_State<CMonster1State_Death>();
		//}

		//else if (In_eHitType == HIT_TYPE::NORMAL_HIT)
		//{
		//	Get_OwnerMonster()->Change_State<CMonster1State_Hit>();
		//	GET_SINGLE(CGameManager)->Add_Shaking(SHAKE_DIRECTION::LOOK, 0.15f, 0.2f);
		//}
		//
		//else if (In_eHitType == HIT_TYPE::DOWN_HIT)
		//{
		//	Get_OwnerMonster()->Change_State<CMonster1State_HitDown>(0.1f);
		//	GET_SINGLE(CGameManager)->Add_Shaking(SHAKE_DIRECTION::RIGHT, 0.25f, 0.3f);
		//}


		if (In_eHitType == HIT_TYPE::LEFT_HIT)
		{
			Get_OwnerMonster()->Change_State<CNorMonState_HurtL>();
		}

		else if (In_eHitType == HIT_TYPE::RIGHT_HIT)
		{
			Get_OwnerMonster()->Change_State<CNorMonState_HurtR>();
		}
	}


}

void CNorMonState_Idle::OnStateStart(const _float& In_fAnimationBlendTime)
{
	__super::OnStateStart(In_fAnimationBlendTime);

	m_pModelCom.lock()->Set_CurrentAnimation(m_iAnimIndex);

#ifdef _DEBUG
	#ifdef _DEBUG_COUT_
		cout << "NorMonState: IdleIDLEIDLE -> OnStateStart" << endl;
#endif
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


	_float fPToMDistance = Get_DistanceWithPlayer(); // �÷��̾�� ���� �Ÿ�
	_float fMToMDistance = GetStartPositionToCurrentPositionDir(); // ���ͽ�ŸƮ�����ǰ� �������� ������ ������ �Ÿ�

	if (fMToMDistance >= 20.f) // �Ÿ�20���ٸ־����� �ٸ��� �ٲ��� ���ư��Ա� 30���ٸ־����� Ʈ����
	{
		m_iIdleType = 3;
	}

	// 1. ���̵���º��� �˻�  �ٵ��̋��� �Ÿ��� ������������� 
	// ù��°�ξ��̵������ ������ ��ģ���� �������� 
	if (fPToMDistance <= 3.f && m_iIdleType == 0)
	{

		switch (m_eNorMonType)
		{
		case Client::NORMONSTERTYPE::AXEMAN:
			switch (m_eNorMonIdleType)
			{
			case Client::NORMONSTERIDLETYPE::NORIDLE:
				TurnMechanism();
				break;
			case Client::NORMONSTERIDLETYPE::SITIDLE:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_SitToIdle>(0.05f);
				break;
			case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Awake>(0.05f);
				break;
			}
			break;
		case Client::NORMONSTERTYPE::KNIFEWOMAN:
			switch (m_eNorMonIdleType)
			{
			case Client::NORMONSTERIDLETYPE::NORIDLE:
				TurnMechanism();
				break;
			case Client::NORMONSTERIDLETYPE::SITIDLE:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_SitToIdle>(0.05f);
				break;
			case Client::NORMONSTERIDLETYPE::FIDGETIDLE:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Awake>(0.05f);
				break;
			}
			break;
		case Client::NORMONSTERTYPE::SKULL:
			break;
		case Client::NORMONSTERTYPE::GARDENER:
			break;
		}	
			m_iIdleType = 1;
			m_bCloseToRun = true;
			return true;
	
	}

	// 2.�系�� ������ �ٽþ��̵�ο� ����´� ������ NORIDLE������
	// ���̵�Ÿ������ ���԰� �̶� �ϰ˻��ѹ� ���ؾ��� 90���� ���������̰ų�
	// 90����ū�����ϋ� �˻縦�ؾ��ϱ⿡ ���� �⺻���̵鵵 ����̾ƴ� 
	// �⺻���̵��̸� ����������� 
	if (m_bCloseToRun && m_iIdleType == 1)
	{

		switch (m_eNorMonType)
		{
		case Client::NORMONSTERTYPE::AXEMAN:
			TurnMechanism();
			break;
		case Client::NORMONSTERTYPE::KNIFEWOMAN:
			TurnMechanism();
			break;
		case Client::NORMONSTERTYPE::SKULL:
			break;
		case Client::NORMONSTERTYPE::GARDENER:
			break;

		}


		m_iIdleType = 2;
		m_bClosePlayerCheck = true;
		return true;
	}
	//�̷��� ���ִ°Ŵ� �ϰ˻縦�߰� ����λ��°� �������� 90����ŭ���������� (�װź�����������)
	//������ ��������ؼ� 90����ũ�� �ٽ��ϰ˻��ϰ� �ƴϸ� �����ڵ�ΰ��ߵ�

	if (fPToMDistance <= 3.f && m_iIdleType == 2 )
	{

		switch (m_eNorMonType)
		{
		case Client::NORMONSTERTYPE::AXEMAN:
				if (ComputeAngleWithPlayer() <= 0.f)
				{
					TurnMechanism();
				}
				else
				{
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_ClosePlayer(true);
					m_bClosePlayerCheck = true;
					m_bCloseToRun = true;
					int iRunORWalk = rand() % 2;
					switch (iRunORWalk)
					{
					case 0:
						Get_OwnerCharacter().lock()->Change_State<CNorMonState_Run>(0.05f);
						break;
					case 1:
						Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_F>(0.05f);
						break;
					}
				}
			break;
		case Client::NORMONSTERTYPE::KNIFEWOMAN:
				if (ComputeAngleWithPlayer() <= 0.f)
				{
					TurnMechanism();
				}
				else
				{
					Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_ClosePlayer(true);
					m_bClosePlayerCheck = true;
					m_bCloseToRun = true;
					int iRunORWalk = rand() % 2;
					switch (iRunORWalk)
					{
					case 0:
						Get_OwnerCharacter().lock()->Change_State<CNorMonState_Run>(0.05f);
						break;
					case 1:
						Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_F>(0.05f);
						break;
					}
				}
			break;
		case Client::NORMONSTERTYPE::SKULL:
			break;
		case Client::NORMONSTERTYPE::GARDENER:
			break;
		}
		
		return true;
	}

	if (m_bCloseToRun && m_iIdleType == 2 && m_bClosePlayerCheck)
	{
		Get_Owner().lock()->Get_Component<CNorMonState_Run>().lock()->Set_ClosePlayer(true);
		int iRunORWalk = rand() % 2;

		switch (m_eNorMonType)
		{
		case Client::NORMONSTERTYPE::AXEMAN:
			switch (iRunORWalk)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Run>(0.05f);
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_F>(0.05f);
				break;
			}
			break;
		case Client::NORMONSTERTYPE::KNIFEWOMAN:
			switch (iRunORWalk)
			{
			case 0:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Run>(0.05f);
				break;
			case 1:
				Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_F>(0.05f);
				break;
			}
			break;
		case Client::NORMONSTERTYPE::SKULL:
			break;
		case Client::NORMONSTERTYPE::GARDENER:
			break;
		}

		return true;
	}


	if (m_iIdleType == 3)
	{
		Get_OwnerCharacter().lock()->Change_State<CNorMonState_Walk_B>(0.05f);
		return true;
	}
	
	return false;
}

